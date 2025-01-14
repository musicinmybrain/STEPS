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
#include "comp.hpp"
#include "kproc.hpp"
#include "model/reac.hpp"
#include "tet.hpp"

// logging
#include <easylogging++.h>
#include "util/error.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace stex = steps::tetexact;
namespace ssolver = steps::solver;

////////////////////////////////////////////////////////////////////////////////

stex::Comp::Comp(steps::solver::Compdef * compdef)
: pCompdef(compdef)
, pVol(0.0)
, pTets()
{
    AssertLog(pCompdef != nullptr);
}

////////////////////////////////////////////////////////////////////////////////

stex::Comp::~Comp()
= default;

////////////////////////////////////////////////////////////////////////////////

void stex::Comp::checkpoint(std::fstream & /*cp_file*/)
{
    // reserve
}

////////////////////////////////////////////////////////////////////////////////

void stex::Comp::restore(std::fstream & /*cp_file*/)
{
    // reserve
}

////////////////////////////////////////////////////////////////////////////////

void stex::Comp::addTet(stex::WmVol * tet)
{
    AssertLog(tet->compdef() == def());
    pTets.push_back(tet);
    pVol += tet->vol();
}

////////////////////////////////////////////////////////////////////////////////

void stex::Comp::modCount(uint slidx, double count)
{
    AssertLog(slidx < def()->countSpecs());
    double newcount = (def()->pools()[slidx] + count);
    AssertLog(newcount >= 0.0);
    def()->setCount(slidx, newcount);
}

////////////////////////////////////////////////////////////////////////////////

stex::WmVol * stex::Comp::pickTetByVol(double rand01) const
{
    if (countTets() == 0) { return nullptr;
}
    if (countTets() == 1) return pTets[0];

    double accum = 0.0;
    double selector = rand01 * vol();
    WmVolPVecCI t_end = endTet();
    for (WmVolPVecCI t = bgnTet(); t != t_end; ++t)
    {
        accum += (*t)->vol();
        if (selector < accum) return (*t);
    }
    AssertLog(false);
    return nullptr;
}

////////////////////////////////////////////////////////////////////////////////

// END
