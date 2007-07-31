////////////////////////////////////////////////////////////////////////////////
// STEPS - STochastic Engine for Pathway Simulation
// Copyright (C) 2005-2007 Stefan Wils. All rights reserved.
//
// $Id$
////////////////////////////////////////////////////////////////////////////////

// STL headers.
#include <cassert>

// STEPS headers.
#include <steps/common.h>
#include <steps/sim/swiginf/func_ssa.hpp>
#include <steps/sim/tetexact/state.hpp>

////////////////////////////////////////////////////////////////////////////////

void siBeginTetmeshDef(State * s)
{
}

////////////////////////////////////////////////////////////////////////////////

void siEndTetmeshDef(State *s)
{
	s->setupTetmesh();
}

////////////////////////////////////////////////////////////////////////////////

void siBeginTetDef(State * s, uint numtets)
{
}

////////////////////////////////////////////////////////////////////////////////

void siEndTetDef(State * s)
{
}

////////////////////////////////////////////////////////////////////////////////

uint siNewTet(State * s, uint cidx, double vol, 
	double a1, double a2, double a3, double a4,
	double d1, double d2, double d3, double d4)
{
	CompDef * cdef = s->def()->comp(cidx);
	return s->addTet(cdef, vol, a1, a2, a3, a4, d1, d2, d3, d4);
}

////////////////////////////////////////////////////////////////////////////////

void siBeginConnectDef(State * s)
{
}

////////////////////////////////////////////////////////////////////////////////

void siEndConnectDef(State * s)
{
}

////////////////////////////////////////////////////////////////////////////////

void siConnectTetTet(State * s, uint side, uint tidx1, uint tidx2)
{
	Tet * t1 = s->tet(tidx1);
	Tet * t2 = s->tet(tidx2);
	t1->setNextTet(side, t2);
}

////////////////////////////////////////////////////////////////////////////////

void siConnectTetTriInside(State * s, uint side, uint tetidx, uint triidx)
{
}

////////////////////////////////////////////////////////////////////////////////

void siConnectTetTriOutside(State * s, uint side, uint tetidx, uint triidx)
{
}

////////////////////////////////////////////////////////////////////////////////

// END
