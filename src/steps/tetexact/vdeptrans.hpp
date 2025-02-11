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


#ifndef STEPS_TETEXACT_VDEPTRANS_HPP
#define STEPS_TETEXACT_VDEPTRANS_HPP 1

////////////////////////////////////////////////////////////////////////////////

// Standard library & STL headers.
#include <string>
#include <vector>

// STEPS headers.
#include "util/common.h"
#include "kproc.hpp"
#include "math/constants.hpp"
#include "solver/vdeptransdef.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace steps {
namespace tetexact {

////////////////////////////////////////////////////////////////////////////////

// Forward declarations.
class Tri;
class Tetexact;

////////////////////////////////////////////////////////////////////////////////

class VDepTrans
: public steps::tetexact::KProc
{

public:

    ////////////////////////////////////////////////////////////////////////
    // OBJECT CONSTRUCTION & DESTRUCTION
    ////////////////////////////////////////////////////////////////////////

    VDepTrans(steps::solver::VDepTransdef * vdtdef, steps::tetexact::Tri * tri);
    ~VDepTrans() override;

    ////////////////////////////////////////////////////////////////////////
    // CHECKPOINTING
    ////////////////////////////////////////////////////////////////////////
    /// checkpoint data
    void checkpoint(std::fstream & cp_file) override;

    /// restore data
    void restore(std::fstream & cp_file) override;

    ////////////////////////////////////////////////////////////////////////
    // VIRTUAL INTERFACE METHODS
    ////////////////////////////////////////////////////////////////////////

    void setupDeps() override;
    bool depSpecTet(uint gidx, steps::tetexact::WmVol * tet) override;
    bool depSpecTri(uint gidx, steps::tetexact::Tri * tri) override;
    void reset() override;

    double rate(steps::tetexact::Tetexact * solver) override;

    std::vector<KProc*> const & apply(const rng::RNGptr &rng, double dt, double simtime) override;

    inline uint updVecSize() const override
    { return static_cast<uint>(pUpdVec.size()); }

    ////////////////////////////////////////////////////////////////////////

private:

    ////////////////////////////////////////////////////////////////////////

    steps::solver::VDepTransdef       * pVDepTransdef;
    steps::tetexact::Tri              * pTri;
    std::vector<KProc*>                 pUpdVec;

    ////////////////////////////////////////////////////////////////////////

};

////////////////////////////////////////////////////////////////////////////////

}
}

////////////////////////////////////////////////////////////////////////////////

#endif

// STEPS_TETEXACT_VDEPTRANS_HPP

