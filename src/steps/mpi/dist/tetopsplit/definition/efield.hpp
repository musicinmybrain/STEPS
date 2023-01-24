#pragma once

#include <optional>
#include <utility>
#include <vector>

#ifdef USE_PETSC
#include <Omega_h_few.hpp>
#include <petscksp.h>
#endif  // USE_PETSC

#include "geom/dist/fwd.hpp"
#include "mpi/dist/tetopsplit/fwd.hpp"
#include "util/vocabulary.hpp"


namespace steps {
namespace dist {

//------------------------------------------------
/**
 * \brief A definition of an Ohmic current.
 */
struct OhmicCurrent {
  /**
   * \brief Ohmic current ctor.
   *
   * \param t_conductance conductance that generates the ohmic current, per unit
   * of the channel state, if channel state is specified and per unit surface
   * otherwise.
   * \param t_reversal_potential potential at which current reverses.
   * \param t_channel_state the channel state that enables the ohmic current. If
   * none is provided the ohmic current is enabled.
   */
  OhmicCurrent(osh::Real t_conductance,
               osh::Real t_reversal_potential,
               const std::optional<container::species_id>& t_channel_state)
      : conductance(t_conductance)
      , reversal_potential(t_reversal_potential)
      , channel_state(t_channel_state) {}

#ifdef USE_PETSC
  /** Boundary condition to get the ohmic current flowing through a triangle an split among the
   * vertexes
   *
   * In case channel_state is not active Avert is used as a quantity of how many "channels" are open
   *
   * check getTriCurrentOnVertex for additional info
   *
   * \tparam NumMolecules
   * \param b_id
   * \param mol_state
   * \param Avert
   * \param sim_time
   * \return
   */
  template <typename NumMolecules>
  PetscReal getTriBConVertex(const mesh::triangle_id_t& b_id,
                             const MolState<NumMolecules>& mol_state,
                             double Avert,
                             osh::Real sim_time) const;

  /** get tri current on vertex
   *
   * get the vertex portion of the ohmic current flowing through a triangle. It is 1/3 of the
   * current flowing through the triangle
   *
   * \tparam NumMolecules: required to have the average (over time) amount of open channels
   * \param potential_on_vertex: voltage on the particular vertex
   * \param b_id: triangle id
   * \param mol_state: channel counts
   * \param mesh: required if the channel_state is not specified
   * \param sim_time: required for occupancy
   * \return current
   */
  template <typename NumMolecules>
  PetscReal getTriCurrentOnVertex(osh::Real potential_on_vertex,
                                  const mesh::triangle_id_t& b_id,
                                  const MolState<NumMolecules>& mol_state,
                                  const DistMesh& mesh,
                                  osh::Real sim_time) const;
#endif  // USE_PETSC
  friend std::ostream& operator<<(std::ostream& os, OhmicCurrent const& m);

  const osh::Real conductance;
  const osh::Real reversal_potential;
  const std::optional<container::species_id> channel_state;
};

//------------------------------------------------

/**
 * \brief Goldman-Hodgkin-Katz current.
 *
 */
struct GHKCurrent {
  /**
   * \brief GHKCurrent ctor.
   *
   * \param t_ion_channel_state channel state that enables the flow of ions
   * \param t_ion_id the species that flows through the membrane
   * \param t_valence the valence of the species that flows through the membrane
   */
  GHKCurrent(model::species_name t_ion_channel_state,
             model::species_name t_ion_id, osh::I64 t_valence)
      : ion_channel_state(std::move(t_ion_channel_state)),
        ion_id(std::move(t_ion_id)), valence(t_valence) {}

  const model::species_name ion_channel_state;
  const model::species_name ion_id;
  const osh::I64 valence;

  friend std::ostream& operator<<(std::ostream& os, GHKCurrent const& m);
};

//------------------------------------------------

struct Channel {
  explicit Channel(std::vector<container::species_id> t_channel_states)
      : channel_states(std::move(t_channel_states)) {}

  void addOhmicCurrent(const OhmicCurrent &current) {
    if (current.channel_state) {
      if (std::find(channel_states.begin(), channel_states.end(),
                    *current.channel_state) == channel_states.end()) {
        std::logic_error(std::string("Ohmic current : Unknown channel state ") +
                         std::to_string(*current.channel_state));
      }
    }
    ohmic_currents.push_back(current);
  }

  void addGHKCurrent(const GHKCurrent &ghk_current) {
    ghk_currents.push_back(ghk_current);
  }

  std::vector<container::species_id> channel_states;
  std::vector<OhmicCurrent> ohmic_currents;
  std::vector<GHKCurrent> ghk_currents;

  friend std::ostream& operator<<(std::ostream& os, Channel const& m);
};

/**
 * \brief Membrane definition.
 */
struct Membrane {
  using Stimulus = std::function<osh::Real(osh::Real)>;
  using Channels = std::map<model::channel_id, Channel>;

  /**
   * \brief Membrane ctor.
   *
   * \param patch patchid of the membrane
   * \param capacitance capacitance of the membrane
   */
  Membrane(const model::patch_id &patch, osh::Real capacitance)
      : patch_(patch), capacitance_(capacitance),
        current_([](auto) { return 0.0; }) {}

  void addChannel(const std::string& channel_name, const Channel& channel) {
      channels_.emplace(channel_name, channel);
  }

  void setStimulus(Stimulus stimulus) noexcept {
      current_ = std::move(stimulus);
  }

  const model::patch_id &getPatch() const noexcept { return patch_; }
  osh::Real capacitance() const noexcept { return capacitance_; }
  Channels &channels() noexcept { return channels_; }

  const Channels &channels() const noexcept { return channels_; }

  const Stimulus &stimulus() const noexcept { return current_; }

private:
  model::patch_id patch_;
  const osh::Real capacitance_;
  Channels channels_;
  Stimulus current_;
};

#ifdef USE_PETSC
/** Small object for preparing values to be inserted in i(), bc(), A0, and diag() in efield
 *
 * It is created with only the capacitance as contribution. Ohmic currents are optional and added
 * after construction
 *
 * Since this struct is related to triangles the numbers 3 and 9 are related to the number of
 * vertexes in a triangle
 */
struct TriMatAndVecs {
    /// local ids of the vertexes of the triangle in PETSc format
    std::array<PetscInt, 3> face_bf2vertsPETSc;
    /// triangle contribution to the stiffness matrix. Size: Nvertexes^2
    std::array<PetscReal, 3 * 3> triStiffnessPETSc;
    /// triangle contribution to the rhs relative to the ohmic currents
    std::array<PetscReal, 3> triBC;
    /// triangle contribution to the rhs relative to the current injections
    std::array<PetscReal, 3> triI;

    /// Ctor
    TriMatAndVecs(const osh::Few<osh::LO, 3>& face_bf2verts,
                  PetscReal tri_capacitance,
                  PetscReal tri_i)
        : face_bf2vertsPETSc{face_bf2verts[0], face_bf2verts[1], face_bf2verts[2]}
        , triStiffnessPETSc{tri_capacitance,
                            0.0,
                            0.0,
                            0.0,
                            tri_capacitance,
                            0.0,
                            0.0,
                            0.0,
                            tri_capacitance}
        , triBC{0.0, 0.0, 0.0}
        , triI{tri_i, tri_i, tri_i} {}


    /// Print: for debugging
    friend std::ostream& operator<<(std::ostream& os, const TriMatAndVecs& obj);
};


// explicit template instantiation declarations

extern template PetscReal OhmicCurrent::getTriBConVertex(const mesh::triangle_id_t& b_id,
                                                         const MolState<osh::I32>& mol_state,
                                                         double Avert,
                                                         osh::Real sim_time) const;
extern template PetscReal OhmicCurrent::getTriBConVertex(const mesh::triangle_id_t& b_id,
                                                         const MolState<osh::I64>& mol_state,
                                                         double Avert,
                                                         osh::Real sim_time) const;

extern template PetscReal OhmicCurrent::getTriCurrentOnVertex(osh::Real potential_on_vertex,
                                                              const mesh::triangle_id_t& b_id,
                                                              const MolState<osh::I32>& mol_state,
                                                              const DistMesh& mesh,
                                                              osh::Real sim_time) const;

extern template PetscReal OhmicCurrent::getTriCurrentOnVertex(osh::Real potential_on_vertex,
                                                              const mesh::triangle_id_t& b_id,
                                                              const MolState<osh::I64>& mol_state,
                                                              const DistMesh& mesh,
                                                              osh::Real sim_time) const;

#endif  // USE_PETSC

//------------------------------------------------

} // namespace dist
} // namespace steps