
#include "kproc_state.hpp"

#include <stdexcept>

#include <Omega_h_for.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/copy.hpp>
#include <boost/graph/graphviz.hpp>

#include "diffusions.hpp"
#include "reactions.hpp"
#include "mpi/dist/tetopsplit/definition/compdef.hpp"
#include "mpi/dist/tetopsplit/definition/statedef.hpp"
#include "util/vocabulary.hpp"

namespace steps {
namespace dist {
namespace kproc {

//------------------------------------------------------------------
template <typename NumMolecules>
KProcState::KProcState(const Statedef& statedef,
                       DistMesh& mesh,
                       MolState<NumMolecules>& mol_state,
                       bool use_rssa,
                       bool independent_kprocs)
    : reactions_(statedef, mesh, mol_state)
    , surface_reactions_(statedef, mesh, mol_state)
    , vdep_surface_reactions_(statedef, mesh, mol_state)
    , ghk_surface_reactions_(statedef, mesh, mol_state) {
    setupDependencies(use_rssa, independent_kprocs);
    // extract connected components of the Gibson-Bruck dependency graph
    setupGroups(independent_kprocs);
}

//------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
template <typename NumMolecules>
typename propensity_function_traits<NumMolecules>::value
KProcState::propensityFun() const {
  return [this](KProcID k_id, const MolState<NumMolecules> &mol_state) {
    switch (k_id.type()) {
    case KProcType::Reac:
      return reactions_.computeRate(mol_state, k_id.id());
    case KProcType::SReac:
      return surface_reactions_.computeRate(mol_state, k_id.id());
    case KProcType::VDepSReac:
      return vdep_surface_reactions_.computeRate(mol_state, k_id.id());
    case KProcType::GHKSReac:
      return ghk_surface_reactions_.computeRate(mol_state, k_id.id());
    case KProcType::Diff:
      throw std::logic_error("Unhandled kinetic process type: Diff");
    }
  };
}
#pragma GCC diagnostic pop

//------------------------------------------------------------------

template <typename KineticProcesses>
void KProcState::collateDependencies(const KineticProcesses &processes,
                                     DependenciesMap &dependency_map) const {
  for (const auto &process : processes) {
    for (const auto &dependency : process.getPropensityDependency()) {
      dependency_map[dependency].emplace_back(processes.getKProcType(),
                                              process.getIndex());
    }
  }
}

//------------------------------------------------------------------

template <typename KineticProcesses>
void KProcState::cacheDependencies(const KineticProcesses &processes,
                                   const DependenciesMap &dependency_map,
                                   dependencies_t &dependencies) {
  std::vector<std::set<KProcID>> unique_deps(processes.size());
  osh::Write<osh::LO> sizes(static_cast<osh::LO>(processes.size()));

  // compute dependencies in a temporary datastructure that avoid taking the
  // same dependency more than once
  for (const auto &process : processes) {
    const auto &mol_state_elements_to_update =
        process.getMolStateElementsUpdates();
    for (const auto &mol_state_element : mol_state_elements_to_update) {
      const auto &it = dependency_map.find(mol_state_element);
      if (it != dependency_map.end()) {
        const std::vector<KProcID> &deps = it->second;
        unique_deps[process.getIndex()].insert(deps.begin(), deps.end());
        sizes[static_cast<osh::LO>(process.getIndex())] =
            static_cast<osh::LO>(unique_deps[process.getIndex()].size());
      }
    }
  }

  dependencies.reshape(sizes);
  for (auto pid = 0u; pid < processes.size(); ++pid) {
    const auto &udeps = unique_deps[pid];
    std::transform(
        udeps.begin(), udeps.end(),
        dependencies[static_cast<osh::LO>(pid)].begin(),
        [](const KProcID &kid) { return static_cast<osh::LO>(kid.data()); });
  }
}

//------------------------------------------------------------------

std::ostream& KProcState::write_dependency_graph(std::ostream& ostr) const {
    // print dependency graph only if we have a filename
    kproc::KProcState::Graph grd;
    std::vector<std::string> labels;
    std::tie(grd, labels) = getDependenciesGraphAndLabels();
    boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::vecS> gd;
    boost::copy_graph(grd, gd);
    std::vector<unsigned> color(boost::num_vertices(grd));
    auto it = boost::make_iterator_property_map(color.begin(), get(boost::vertex_index, grd));
    boost::connected_components(grd, it);
    std::vector<std::string> col_vals{"red", "blue", "green", "yellow", "magenta", "grey"};
    auto node_fmt = [labels, color, col_vals](std::ostream& out, unsigned v) {
        out << " [label=\"" << labels[v] << "\"]" << std::endl;
        out << " [color=" << col_vals[color[v] % col_vals.size()] << "]" << std::endl;
    };
    boost::write_graphviz(ostr, gd, node_fmt);
    return ostr;
}

//------------------------------------------------------------------

void KProcState::setupDependencies(bool use_rssa, bool independent_kprocs) {
  if (independent_kprocs || !use_rssa) {
    DependenciesMap dependency_map(
        reactions().size() + surfaceReactions().size() +
        vDepSurfaceReactions().size() + ghkSurfaceReactions().size());
    // collate kinetic processes dependencies
    collateAllDependencies(dependency_map);
    // caching the kinetic processes dependencies
    cacheDependencies(reactions(), dependency_map, reactions_dependencies_);
    cacheDependencies(surfaceReactions(), dependency_map,
                      surface_reactions_dependencies_);
    cacheDependencies(vDepSurfaceReactions(), dependency_map,
                      vdep_surface_reactions_dependencies_);
    cacheDependencies(ghkSurfaceReactions(), dependency_map,
                      ghk_surface_reactions_dependencies_);
  }
}

//------------------------------------------------------------------

KProcState::Graph KProcState::getDependenciesGraph(const Propensities<> &propensities) const {
  // propensities needed to extract a kproc index
  auto num_edges = [](const dependencies_t &deps) {
    return std::accumulate(deps.begin(), deps.end(), 0u,
                           [](size_t acc, const auto &x) {
                             return acc + static_cast<size_t>(x.size());
                           });
  };
  assert(reactions().size() + surfaceReactions().size() +
             vDepSurfaceReactions().size() + ghkSurfaceReactions().size() <=
         static_cast<size_t>(std::numeric_limits<osh::LO>::max()));
  const size_t tot_num_edges = num_edges(reactions_dependencies_) +
                               num_edges(surface_reactions_dependencies_) +
                               num_edges(vdep_surface_reactions_dependencies_) +
                               num_edges(ghk_surface_reactions_dependencies_);
  using Edge = std::pair<unsigned, unsigned>;
  std::vector<Edge> edges;
  edges.reserve(tot_num_edges);
  auto populate_edges = [&propensities, &edges](KProcType ty,
                                                const dependencies_t &deps) {
    for (osh::LO k = 0; k < deps.size(); ++k) {
      auto klo = propensities.ab(KProcID(ty, static_cast<unsigned>(k)));
      std::transform(
          deps[k].begin(), deps[k].end(), std::back_inserter(edges),
          [&](const auto dep) -> Edge {
            return {klo, propensities.ab(KProcID(static_cast<unsigned>(dep)))};
          });
    }
  };
  populate_edges(KProcType::Reac, reactions_dependencies_);
  populate_edges(KProcType::SReac, surface_reactions_dependencies_);
  populate_edges(KProcType::VDepSReac, vdep_surface_reactions_dependencies_);
  populate_edges(KProcType::GHKSReac, ghk_surface_reactions_dependencies_);

  return {edges.begin(), edges.end(),
          reactions().size() + surfaceReactions().size() +
              vDepSurfaceReactions().size() + ghkSurfaceReactions().size()};
}

//------------------------------------------------------------------

void KProcState::setupGroups(bool independent_kprocs) {
  // propensities needed to extract a kproc. index
  Propensities<> propensities;
  propensities.init(handledKProcsClassesAndSizes());
  // extract the number of kproc dependencies
  if (independent_kprocs) {
    auto graph = getDependenciesGraph(propensities);
    std::vector<unsigned> color(boost::num_vertices(graph));
    auto it = boost::make_iterator_property_map(
        color.begin(), get(boost::vertex_index, graph));
    size_t num_components = boost::connected_components(graph, it);

    std::vector<std::vector<osh::LO>> disjoint_k_procs_w(num_components);
    osh::Write<osh::LO> sizes(static_cast<osh::LO>(num_components), 0);
    for (size_t k = 0; k < color.size(); ++k) {
      sizes[static_cast<osh::LO>(color[k])]++;
      disjoint_k_procs_w[color[k]].push_back(
          static_cast<osh::LO>(propensities.kProcId(k).data()));
    }
    disjoint_kprocs_.reshape(sizes);
    for (size_t k = 0; k < num_components; ++k) {
      std::copy(disjoint_k_procs_w[k].begin(), disjoint_k_procs_w[k].end(),
                disjoint_kprocs_[static_cast<osh::LO>(k)].begin());
    }
  } else {
    disjoint_kprocs_.reshape({static_cast<osh::LO>(propensities.size())});
    size_t k = 0;
    for (auto it = disjoint_kprocs_[0].begin(); it != disjoint_kprocs_[0].end();
         it++, k++) {
      *it = static_cast<osh::LO>(propensities.kProcId(k).data());
    }
  }
}

//------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
template <typename NumMolecules>
const std::vector<MolStateElementID>& KProcState::updateMolStateAndOccupancy(
    MolState<NumMolecules>& mol_state,
    const osh::Real event_time,
    const KProcID& event) const {
    switch (event.type()) {
    case KProcType ::Reac:
        return reactions().updateMolStateAndOccupancy(mol_state, event.id(), event_time);
    case KProcType::SReac:
        return surfaceReactions().updateMolStateAndOccupancy(mol_state, event.id(), event_time);
    case KProcType::VDepSReac:
        return vDepSurfaceReactions().updateMolStateAndOccupancy(mol_state, event.id(), event_time);
    case KProcType::GHKSReac:
        return ghkSurfaceReactions().updateMolStateAndOccupancy(mol_state, event.id(), event_time);
    case KProcType::Diff:
        throw std::logic_error("Unhandled kinetic process: Diffusion");
    }
}

#pragma GCC diagnostic ignored "-Wreturn-type"
void KProcState::report(std::ostream &report_stream, KProcID kid) const {
  switch (kid.type()) {
  case KProcType ::Reac:
    return reactions().report(report_stream, kid.id());
  case KProcType::SReac:
    return surfaceReactions().report(report_stream, kid.id());
  case KProcType::VDepSReac:
    return vDepSurfaceReactions().report(report_stream, kid.id());
  case KProcType::GHKSReac:
    return ghkSurfaceReactions().report(report_stream, kid.id());
  case KProcType::Diff:
    throw std::logic_error("Unhandled kinetic process: Diffusion");
  }
}
#pragma GCC diagnostic pop

//------------------------------------------------------------------

std::pair<KProcState::Graph, std::vector<std::string>>
KProcState::getDependenciesGraphAndLabels() const {
  Propensities<> propensities;
  propensities.init(handledKProcsClassesAndSizes());
  std::vector<std::string> labels(propensities.size());
  std::transform(boost::make_counting_iterator(size_t{}),
                 boost::make_counting_iterator(propensities.size()),
                 labels.begin(), [&](auto x) {
                   std::ostringstream s;
                   report(s, propensities.kProcId(x));
                   return s.str();
                 });
  return {getDependenciesGraph(propensities), labels};
}

//--------------------------------------------------------

// explicit template instantiation definitions
template void
KProcState::collateDependencies(const Reactions &processes,
                                DependenciesMap &dependency_map) const;
template void
KProcState::collateDependencies(const SurfaceReactions &processes,
                                DependenciesMap &dependency_map) const;
template void
KProcState::collateDependencies(const VDepSurfaceReactions &processes,
                                DependenciesMap &dependency_map) const;
template void
KProcState::collateDependencies(const GHKSurfaceReactions &processes,
                                DependenciesMap &dependency_map) const;

template propensity_function_traits<osh::I32>::value
KProcState::propensityFun<osh::I32>() const;
template propensity_function_traits<osh::I64>::value
KProcState::propensityFun<osh::I64>() const;

template const std::vector<MolStateElementID>& KProcState::updateMolStateAndOccupancy(
    MolState<osh::I32>& mol_state,
    const osh::Real event_time,
    const KProcID& event) const;
template const std::vector<MolStateElementID>& KProcState::updateMolStateAndOccupancy(
    MolState<osh::I64>& mol_state,
    const osh::Real event_time,
    const KProcID& event) const;


template KProcState::KProcState(const Statedef& statedef,
                                DistMesh& mesh,
                                MolState<osh::I32>& mol_state,
                                bool use_rssa,
                                bool independent_kprocs);
template KProcState::KProcState(const Statedef& statedef,
                                DistMesh& mesh,
                                MolState<osh::I64>& mol_state,
                                bool use_rssa,
                                bool independent_kprocs);

} // namespace kproc
} // namespace dist
} // namespace steps
