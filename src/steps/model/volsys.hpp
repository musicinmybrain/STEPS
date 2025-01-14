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

/*
 *  Last Changed Rev:  $Rev$
 *  Last Changed Date: $Date$
 *  Last Changed By:   $Author$
 */

#pragma once

#include <cassert>
#include <map>
#include <string>
#include <vector>

#include "util/common.h"

////////////////////////////////////////////////////////////////////////////////

namespace steps {
namespace model {

////////////////////////////////////////////////////////////////////////////////

// Forward declarations.
class Model;
class Spec;
class Volsys;
class Reac;
class Diff;

// Auxiliary declarations.
typedef Volsys *                        VolsysP;
typedef std::map<std::string, VolsysP>  VolsysPMap;
typedef VolsysPMap::iterator            VolsysPMapI;
typedef VolsysPMap::const_iterator      VolsysPMapCI;

////////////////////////////////////////////////////////////////////////////////

/// Volume system
///
/// Container that collects reactions
/// involving a reactant enclosed in a compartment.
///
/// \warning Methods start with an underscore are not exposed to Python.

class Volsys
{

public:

    ////////////////////////////////////////////////////////////////////////
    // OBJECT CONSTRUCTION & DESTRUCTION
    ////////////////////////////////////////////////////////////////////////
    /// Constructor
    ///
    /// \param id ID of the volume system.
    /// \param model Pointer to the parent model object.
    Volsys(std::string const & id, Model * model);

    /// Destructor
    ~Volsys();

    ////////////////////////////////////////////////////////////////////////
    // VOLUME SYSTEM PROPERTIES
    ////////////////////////////////////////////////////////////////////////

    /// Return the volume system ID.
    ///
    /// \return A string of volume system ID.
    std::string getID() const
    { return pID; }

    /// Set or change the volume system ID.
    ///
    /// \param id The new volume system ID.
    void setID(std::string const & id);

    /// Return a pointer to the parent model.
    ///
    /// \return Pointer to the parent model.
    Model * getModel() const
    { return pModel; }

    ////////////////////////////////////////////////////////////////////////
    // OPERATIONS (EXPOSED TO PYTHON): REACTIONS
    ////////////////////////////////////////////////////////////////////////

    /// Get a reaction by its ID.
    ///
    /// \param id ID of the required reaction.
    /// \return Pointer to the reaction object.
    Reac * getReac(std::string const & id) const;

    /// Delete a reaction by its ID.
    ///
    /// \param id ID of the reaction to be deleted.
    void delReac(std::string const & id);

    /// Get all reactions stored in this volume system.
    ///
    /// \return A vector of pointers to the reaction objects
    ///         stored in the system.
    std::vector<Reac *> getAllReacs() const;

    ////////////////////////////////////////////////////////////////////////
    // OPERATIONS (EXPOSED TO PYTHON): DIFFUSION
    ////////////////////////////////////////////////////////////////////////

    /// Get a difussion by its ID.
    ///
    /// \param id ID of the required difussion.
    /// \return Pointer to the diffusion object.
    Diff * getDiff(std::string const & id) const;

    /// Delete a diffusion by its ID.
    ///
    /// \param id ID of the diffusion to be deleted.
    void delDiff(std::string const & id);

    /// Get all diffusions stored in this volume system.
    ///
    /// \return A vector of pointers to the diffusion objects
    ///         stored in the system.
    std::vector<Diff *> getAllDiffs() const;

    ////////////////////////////////////////////////////////////////////////
    // OPERATIONS (EXPOSED TO PYTHON): SPECIES
    ////////////////////////////////////////////////////////////////////////

    /// Get all species stored in this volume system.
    ///
    /// \return A vector of pointers to the species stored in the system.
    ///
    /// This method returns a list of all species involved in this volume system,
    /// no duplicate member is included.
    std::vector<Spec *> getAllSpecs() const;

    ////////////////////////////////////////////////////////////////////////
    // INTERNAL (NON-EXPOSED) OPERATIONS: DELETION
    ////////////////////////////////////////////////////////////////////////

    /// Self delete.
    ///
    /// Called if Python object deleted, or from del method in parent object.
    /// Will only be called once.
    void _handleSelfDelete();

    ////////////////////////////////////////////////////////////////////////
    // INTERNAL (NON-EXPOSED) OPERATIONS: REACTIONS
    ////////////////////////////////////////////////////////////////////////

    /// Check if a reaction id is occupied.
    ///
    /// \param id ID of the reaction.
    void _checkReacID(std::string const & id) const;

    /// Change the id of a reaction from o to n.
    ///
    /// \param o Old id of the reaction.
    /// \param n New id of the reaction.
    void _handleReacIDChange(std::string const & o, std::string const & n);

    /// Add a reaction to the volume system.
    ///
    /// \param reac Pointer to the reaction.
    void _handleReacAdd(Reac * reac);

    /// Delete a reaction in the volume system.
    ///
    /// \param reac Pointer to the reaction.
    void _handleReacDel(Reac * reac);

    ////////////////////////////////////////////////////////////////////////
    // INTERNAL (NON-EXPOSED) OPERATIONS: DIFFUSION
    ////////////////////////////////////////////////////////////////////////
    /// Check if a diffusion id is occupied.
    ///
    /// \param id ID of the diffusion.
    void _checkDiffID(std::string const & id) const;

    /// Change the id of a diffusion from o to n.
    ///
    /// \param o Old id of the diffusion.
    /// \param n New id of the diffusion.
    void _handleDiffIDChange(std::string const & o, std::string const & n);

    /// Add a diffusion to the volume system.
    ///
    /// \param diff Pointer to the diffusion.
    void _handleDiffAdd(Diff * diff);

    /// Delete a diffusion in the volume system.
    ///
    /// \param diff Pointer to the diffusion.
    void _handleDiffDel(Diff * diff);

    ////////////////////////////////////////////////////////////////////////
    // INTERNAL (NON-EXPOSED): SOLVER HELPER METHODS
    ////////////////////////////////////////////////////////////////////////

    /// Count the reactions in the volume system.
    ///
    /// \return Number of reactions.
    inline uint _countReacs() const
    { return pReacs.size(); }

    /// Get a reaction with index lidx
    ///
    /// \param lidx Index of the reaction.
    /// \return Pointer to the reaction.
    Reac * _getReac(uint lidx) const;

    /// Get all species in the volume system.
    ///
    /// \return A list of pointers to the species.
    const std::map<std::string, Reac *> & _getAllReacs() const
    { return pReacs; }

    /// Count the diffusion rules in the volume system.
    ///
    /// \return Number of diffusion rules.
    inline uint _countDiffs() const
    { return pDiffs.size(); }

    /// Get a diffusion rule with index lidx.
    ///
    /// \param lidx Index of the diffusion rule.
    /// \return Pointer to the diffusion rule.
    Diff * _getDiff(uint lidx) const;

    /// Get all diffusion rules in the volume system.
    ///
    /// \return List of pointers to diffusion rules.
    const std::map<std::string, Diff *> & _getAllDiffs() const
    { return pDiffs; }

    ////////////////////////////////////////////////////////////////////////
    // INTERNAL (NON-EXPOSED): STEPS::MODEL OPERATIONS
    ////////////////////////////////////////////////////////////////////////

    /// Delete a species.
    ///
    /// \param Pointer to the species.
    void _handleSpecDelete(Spec * spec);

    ////////////////////////////////////////////////////////////////////////

private:

    ////////////////////////////////////////////////////////////////////////

    std::string                         pID;
    Model                             * pModel;
    std::map<std::string, Reac *>       pReacs;
    std::map<std::string, Diff *>       pDiffs;

    ////////////////////////////////////////////////////////////////////////

};

////////////////////////////////////////////////////////////////////////////////

} // namespace model
} // namespace steps
