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


// Standard library & STL headers.
#include <vector>

// STEPS headers.
#include "diffboundary.hpp"
#include "comp.hpp"

// logging
#include <easylogging++.h>
#include "util/error.hpp"
////////////////////////////////////////////////////////////////////////////////

namespace stex = steps::tetexact;
namespace ssolver = steps::solver;

////////////////////////////////////////////////////////////////////////////////

stex::DiffBoundary::DiffBoundary(steps::solver::DiffBoundarydef * dbdef)
: pDiffBoundarydef(dbdef)
{
    AssertLog(dbdef != nullptr);
}

////////////////////////////////////////////////////////////////////////////////

stex::DiffBoundary::~DiffBoundary() = default;

////////////////////////////////////////////////////////////////////////////////

void stex::DiffBoundary::checkpoint(std::fstream & /*cp_file*/)
{
    // reserve
}

////////////////////////////////////////////////////////////////////////////////

void stex::DiffBoundary::restore(std::fstream & /*cp_file*/)
{
    // reserve
}

////////////////////////////////////////////////////////////////////////////////

void stex::DiffBoundary::setComps(stex::Comp * compa, stex::Comp * compb)
{
    AssertLog(pSetComps == false);
    AssertLog(compa != nullptr);
    AssertLog(compb != nullptr);
    AssertLog(compa != compb);

    pCompA = compa;
    pCompB = compb;
    pSetComps = true;
}

////////////////////////////////////////////////////////////////////////////////

stex::Comp * stex::DiffBoundary::compA()
{
    AssertLog(pSetComps == true);
    return pCompA;
}

////////////////////////////////////////////////////////////////////////////////

stex::Comp * stex::DiffBoundary::compB()
{
    AssertLog(pSetComps == true);
    return pCompB;
}

////////////////////////////////////////////////////////////////////////////////

void stex::DiffBoundary::setTetDirection(tetrahedron_id_t tet, uint direction)
{
    AssertLog(direction < 4);

    pTets.push_back(tet);
    pTetDirection.push_back(direction);
}

////////////////////////////////////////////////////////////////////////////////

// END

