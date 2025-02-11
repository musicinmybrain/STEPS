/*
 #################################################################################
#
#    STEPS - STochastic Engine for Pathway Simulation
#    Copyright (C) 2007-2022 Okinawa Institute of Science and Technology, Japan.
#    Copyright (C) 2003-2006 University of Antwerp, Belgium.
#    
#    See the file AUTHORS for details.
#    This file is part of STEPS.
#    
#    STEPS is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License version 2,
#    as published by the Free Software Foundation.
#    
#    STEPS is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#    GNU General Public License for more details.
#    
#    You should have received a copy of the GNU General Public License
#    along with this program. If not, see <http://www.gnu.org/licenses/>.
#
#################################################################################   

 */


#ifndef STEPS_MPI_TETOPSPLIT_TET_HPP
#define STEPS_MPI_TETOPSPLIT_TET_HPP 1

// STL headers.
#include <cassert>
#include <vector>

// logging
#include <easylogging++.h>

// STEPS headers.
#include "kproc.hpp"
#include "wmvol.hpp"
#include "util/common.h"
#include "solver/compdef.hpp"
#include "solver/types.hpp"
////////////////////////////////////////////////////////////////////////////////

namespace steps {
namespace mpi {
namespace tetopsplit {

////////////////////////////////////////////////////////////////////////////////

namespace smtos = steps::mpi::tetopsplit;

////////////////////////////////////////////////////////////////////////////////

// Forward declarations
class Tet;
class Diff;
class Tri;
class Reac;
class TetOpSplitP;

// Auxiliary declarations.
typedef Tet *                           TetP;
typedef std::vector<TetP>               TetPVec;
typedef TetPVec::iterator               TetPVecI;
typedef TetPVec::const_iterator         TetPVecCI;

////////////////////////////////////////////////////////////////////////////////

class Tet: public smtos::WmVol
{

public:

    ////////////////////////////////////////////////////////////////////////
    // OBJECT CONSTRUCTION & DESTRUCTION
    ////////////////////////////////////////////////////////////////////////

    Tet
      (
        tetrahedron_id_t idx,
        steps::solver::Compdef *cdef,
        double vol,
        double a0,
        double a1,
        double a2,
        double a3,
        double d0,
        double d1,
        double d2,
        double d3,
        tetrahedron_id_t tet0,
        tetrahedron_id_t tet1,
        tetrahedron_id_t tet2,
        tetrahedron_id_t tet3,
        int rank,
        int host_rank
      );
    ~Tet() override;

    ////////////////////////////////////////////////////////////////////////
    // CHECKPOINTING
    ////////////////////////////////////////////////////////////////////////
    /// checkpoint data
    void checkpoint(std::fstream & cp_file) override;

    /// restore data
    void restore(std::fstream & cp_file) override;

    ////////////////////////////////////////////////////////////////////////
    // SETUP
    ////////////////////////////////////////////////////////////////////////


    /// Set pointer to the next neighbouring tetrahedron.
    ///
    void setNextTet(uint i, smtos::Tet * t);

    /*
    /// Set pointer to the next neighbouring triangle.
    void setNextTri(uint i, smtos::Tri * t);
    */
    void setNextTri(uint i, smtos::Tri *t);

    // This method only asserts this method is not called on derived object
    void setNextTri(smtos::Tri *t) override;

    /// Create the kinetic processes -- to be called when all tetrahedrons
    /// and triangles have been fully declared and connected.
    ///
    void setupKProcs(smtos::TetOpSplitP * tex) override;


    ////////////////////////////////////////////////////////////////////////
    // SHAPE & CONNECTIVITY INFORMATION.
    ////////////////////////////////////////////////////////////////////////

    /// Get pointer to the next neighbouring triangle.
    ///
    inline smtos::Tri * nextTri(uint i) const
    {
        AssertLog(i < 4);
        return pNextTris[i];
    }

    /// Get pointer to the next neighbouring tetrahedron.
    ///
    inline smtos::Tet * nextTet(uint i) const noexcept
    { return pNextTet[i]; }

    /// Get the area of a boundary triangle.
    ///
    inline double area(uint i) const noexcept
    { return pAreas[i]; }

    /// Get the distance to the centroid of the next neighbouring
    /// tetrahedron.
    ///
    inline double dist(uint i) const noexcept
    { return pDist[i]; }

    /// Find the direction index towards a neighbor tetrahedron.
    ///
    int getTetDirection(tetrahedron_id_t tidx);

    ////////////////////////////////////////////////////////////////////////

    // Set whether a direction is a diffusion boundary
    void setDiffBndDirection(uint i);

    inline bool getDiffBndDirection(uint idx) const noexcept
    { return pDiffBndDirection[idx]; }


    smtos::Diff * diff(uint lidx) const;

    inline tetrahedron_id_t tet(uint t) const noexcept
    { return pTets[t]; }

    /////////////////////////// Dependency ////////////////////////////////
    // setup dependence for KProcs in this subvolume
    void setupDeps() override;

    // check if kp_lidx in this vol depends on spec_gidx in WMVol kp_container
    virtual bool KProcDepSpecTet(uint kp_lidx, WmVol* kp_container, uint spec_gidx) override;
    // check if kp_lidx in this vol depends on spec_gidx in Tri kp_container
    virtual bool KProcDepSpecTri(uint kp_lidx, Tri* kp_container, uint spec_gidx) override;

    ///////////////////////////////////////////////////////////////////

    void setCount(uint lidx, uint count, double period = 0.0) override;
	  void incCount(uint lidx, int inc, double period = 0.0, bool local_change = false) override;

	  inline double getPoolOccupancy(uint lidx) const override {
	    AssertLog(lidx < compdef()->countSpecs());
	    return pPoolOccupancy[lidx];
	  }
    double getLastUpdate(uint lidx) const override {
      AssertLog(lidx < compdef()->countSpecs());
      return pLastUpdate[lidx];
    }
	  void resetPoolOccupancy() override;

    std::vector<smtos::KProc*> const & getSpecUpdKProcs(uint slidx);

    ////////////////////////////////////////////////////////////
    void repartition(smtos::TetOpSplitP * tex, int rank, int host_rank) override;
    void setupBufferLocations();

    using super_type = smtos::WmVol;

private:

    ////////////////////////////////////////////////////////////////////////

    // Indices of neighbouring tetrahedra.
    std::array<tetrahedron_id_t, 4>     pTets;

    /*
    // Indices of neighbouring triangles.
    uint                                pTris[4];
    */

    /*
    /// POinters to neighbouring triangles
    smtos::Tri                         * pNextTri[4];
    */

    /// Pointers to neighbouring tetrahedra.
    smtos::Tet                         * pNextTet[4];

    double                              pAreas[4];
    double                              pDist[4];

    bool                                  pDiffBndDirection[4];


    /// Structure to store occupancy over the update period
    double                            * pPoolOccupancy;
    /// Structure to store time since last update, used to calculate occupancy
    double 							  *	pLastUpdate;

    /// location of where the change of this species is stored in  the solver buffer
    std::vector<uint>                   bufferLocations;
    // local kprocs update list when spec is changed
    std::vector<std::vector<smtos::KProc *>> localSpecUpdKProcs;

    ////////////////////////////////////////////////////////////////////////

};

////////////////////////////////////////////////////////////////////////////////

}
}
}

////////////////////////////////////////////////////////////////////////////////

#endif

// STEPS_MPI_TETOPSPLIT_TET_HPP

// END
