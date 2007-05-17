////////////////////////////////////////////////////////////////////////////////
// STEPS - STochastic Engine for Pathway Simulation
// Copyright (C) 2005-2007 Stefan Wils. All rights reserved.
//
// $Id$
////////////////////////////////////////////////////////////////////////////////

#ifndef STEPS_SIM_SHARED_DOXYGEN_HPP
#define STEPS_SIM_SHARED_DOXYGEN_HPP 1

///
/// Classes CompDef, ReacDef, SpecDef and StateDef together define the
/// layout of a state. Unlike the Python classes in package steps.model,
/// this classes are designed for . This means they have little or no
/// flexibility to allow for changes to the state during simulation (i.e.
/// adding/deleting new species, reactions, compartments etc). They 
/// work largely on the basis of integer indices.
///
/// The order in which finalSetup() methods are called is as follows:
/// <OL>
///     <LI> SpecDef::finalSetup() </LI>
///     <LI> ReacDef::finalSetup() </LI>
///     <LI> CompDef::finalSetup() </LI>
/// </OL>
///

#endif
// STEPS_SIM_SHARED_DOXYGEN_HPP

// END