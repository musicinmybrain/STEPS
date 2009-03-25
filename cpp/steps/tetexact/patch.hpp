////////////////////////////////////////////////////////////////////////////////
// STEPS - STochastic Engine for Pathway Simulation
// Copyright (C) 2005-2008 Stefan Wils. All rights reserved.
//
// This file is part of STEPS.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301, USA
//
//
////////////////////////////////////////////////////////////////////////////////

#ifndef STEPS_TETEXACT_PATCH_HPP
#define STEPS_TETEXACT_PATCH_HPP 1

// Autotools definitions.
#ifdef HAVE_CONFIG_H
#include <steps/config.h>
#endif

// STL headers.
#include <cassert>
#include <vector>

// STEPS headers.
#include <steps/common.h>
#include <steps/solver/patchdef.hpp>
//#include <steps/tetexact/kproc.hpp>
//#include <steps/tetexact/comp.hpp>
#include <steps/tetexact/tri.hpp>
#include <steps/solver/types.hpp>

////////////////////////////////////////////////////////////////////////////////

START_NAMESPACE(steps)
START_NAMESPACE(tetexact)

////////////////////////////////////////////////////////////////////////////////

NAMESPACE_ALIAS(steps::tetexact, stex);

////////////////////////////////////////////////////////////////////////////////

// Forward declarations.
class Patch;

// Auxiliary declarations.
typedef Patch *                         PatchP;
typedef std::vector<PatchP>             PatchPVec;
typedef PatchPVec::iterator             PatchPVecI;
typedef PatchPVec::const_iterator       PatchPVecCI;

////////////////////////////////////////////////////////////////////////////////

class Patch
{
public:

    ////////////////////////////////////////////////////////////////////////
    // OBJECT CONSTRUCTION & DESTRUCTION
    ////////////////////////////////////////////////////////////////////////

    Patch(steps::solver::Patchdef * patchdef);
    ~Patch(void);

    /// Checks whether Tri::patchdef() corresponds to this object's
    /// PatchDef. There is no check whether the Tri object has already
    /// been added to this Patch object before (i.e. no duplicate
    /// checking).
    ///
    void addTri(stex::Tri * tri);

    ////////////////////////////////////////////////////////////////////////

    inline void reset(void)
    { def()->reset(); }

    ////////////////////////////////////////////////////////////////////////
    // DATA ACCESS
    ////////////////////////////////////////////////////////////////////////

    inline steps::solver::Patchdef * def(void) const
    { return pPatchdef; }

    inline double area(void) const
    { return pArea; }

    inline double * pools(void) const
    { return def()->pools(); }

    void modCount(uint slidx, double count);


    inline uint countTris(void) const
    { return pTris.size(); }

    stex::Tri * pickTriByArea(double rand01) const;

    inline TriPVecCI bgnTri(void) const
    { return pTris.begin(); }
    inline TriPVecCI endTri(void) const
    { return pTris.end(); }

    ////////////////////////////////////////////////////////////////////////

private:

    ////////////////////////////////////////////////////////////////////////

    steps::solver::Patchdef           * pPatchdef;
    double                              pArea;

    TriPVec                             pTris;

};

////////////////////////////////////////////////////////////////////////////////

END_NAMESPACE(tetexact)
END_NAMESPACE(steps)

#endif
// STEPS_TETEXACT_PATCH_HPP

// END