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

#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

#include "comp.hpp"
#include "geom.hpp"

#include "model/surfsys.hpp"
#include "util/common.h"

////////////////////////////////////////////////////////////////////////////////

namespace steps {
namespace wm {

////////////////////////////////////////////////////////////////////////////////

// Forward declarations.

class Comp;
class Geom;
class Patch;

// Auxiliary declarations.
typedef Patch *                         PatchP;
typedef std::map<std::string, PatchP>   PatchPMap;
typedef PatchPMap::iterator             PatchPMapI;
typedef PatchPMap::const_iterator       PatchPMapCI;

typedef std::vector<PatchP>             PatchPVec;
typedef PatchPVec::iterator             PatchPVecI;
typedef PatchPVec::const_iterator       PatchPVecCI;

////////////////////////////////////////////////////////////////////////////////

/// Base class for patch objects.
///
///    A patch is a piece of 2D surface surrounding (part of) a 3D compartment.
///    This base class provides basic functionality and descriptive data that
///    is shared by all types of patches ('type' meaning different types of
///    geometric descriptions):
///
///        - Getting and setting a valid patch ID string, and handling
///          the interaction with the container object.
///
///        - Getting (and at least in this base class also setting) the total
///          area of the patch.
///
///        - The surface systems associated with the patches.
///
///        - References to inside/outside compartments.
///
///    This base class can be used directly with well-mixed solvers.
///
/// \warning Methods start with an underscore are not exposed to Python.

class Patch
{
public:

    ////////////////////////////////////////////////////////////////////////
    // OBJECT CONSTRUCTION & DESTRUCTION
    ////////////////////////////////////////////////////////////////////////

    /// Constructor
    ///
    /// \param id ID of the patch.
    /// \param container Pointer to the parent geometry container.
    /// \param icomp Pointer to the inner compartment.
    /// \param ocomp Pointer to the outer compartment.
    /// \param surfsys Pointer to the associated surface system.
    /// \param area Area of the patch.
    Patch(std::string id, steps::wm::Geom * container,
          steps::wm::Comp* icomp, steps::wm::Comp* ocomp = 0, double area = 0.0);

    /// Destructor
    virtual ~Patch();

    ////////////////////////////////////////////////////////////////////////
    // PATCH PROPERTIES (EXPOSED TO PYTHON)
    ////////////////////////////////////////////////////////////////////////

    /// Return the patch id.
    ///
    /// \return ID of the patch.
    inline std::string const & getID() const noexcept
    { return pID; }

    /// Set or change the patch id.
    ///
    /// \param id ID of the patch.
    void setID(std::string const & id);

    /// Return a pointer to the geometry container object.
    ///
    /// \return Pointer to the parent geometry container.
    inline steps::wm::Geom * getContainer() const noexcept
    { return pContainer; }

    /// Return the area of the patch.
    ///
    /// \return Area of the patch.
    inline double getArea() const noexcept
    { return pArea; }

    /// Set the area of the patch.
    ///
    /// \param area Area of the patch.
    virtual void setArea(double area);

    ////////////////////////////////////////////////////////////////////////
    // OPERATIONS (EXPOSED TO PYTHON): VOLUME SYSTEM
    ////////////////////////////////////////////////////////////////////////

    /// Add a surface system with name id.
    ///
    /// \param id ID of the surface system.
    void addSurfsys(std::string const & id);

    /// Get a surface system.
    ///
    /// \return List of the surface systems associated to the patch.
    inline std::set<std::string> const & getSurfsys() const noexcept
    {return pSurfsys; }

    /// Delete a surface system with name id.
    ///
    /// \param id ID of the surface system.
    void delSurfsys(std::string const & id);

    ////////////////////////////////////////////////////////////////////////
    // OPERATIONS (EXPOSED TO PYTHON): MODEL LINKING
    ////////////////////////////////////////////////////////////////////////
    /// Return all spec in the patch giving a model.
    std::vector<steps::model::Spec *>
    getAllSpecs(const steps::model::Model *model) const;

    /// Return all SReac in the patch giving a model.
    std::vector<steps::model::SReac *>
    getAllSReacs(const steps::model::Model *model) const;

    ////////////////////////////////////////////////////////////////////////
    // DATA ACCESS (EXPOSED TO PYTHON): COMPARTMENTS
    ////////////////////////////////////////////////////////////////////////

    /// Return the inner compartment.
    ///
    /// \return Pointer to the inner compartment.
    inline steps::wm::Comp * getIComp() const noexcept
    { return pIComp; }

    ///Return the outer compartment.
    ///
    /// \return Pointer to the outer compartment.
    inline steps::wm::Comp * getOComp() const noexcept
    { return pOComp; }

    ////////////////////////////////////////////////////////////////////////
    // INTERNAL (NON-EXPOSED) OPERATIONS: PATCHES
    ////////////////////////////////////////////////////////////////////////

    /// Set the inner compartment.
    ///
    /// \param icomp Pointer to the inner compartment.
    void _setIComp(steps::wm::Comp* icomp);

    /// Set the outer compartment.
    ///
    /// \param ocomp Pointer to the outer compartment.
    void _setOComp(steps::wm::Comp* ocomp);

    ////////////////////////////////////////////////////////////////////////
    // INTERNAL (NON-EXPOSED) OPERATIONS: DELETION
    ////////////////////////////////////////////////////////////////////////
    /// Self delete.
    ///
    /// Called if Python object deleted, or from del method in parent object.
    /// Will only be called once
    void _handleSelfDelete();

    ////////////////////////////////////////////////////////////////////////


    ////////////////////////////////////////////////////////////////////////

  protected:
    double pArea;

  private:

    ////////////////////////////////////////////////////////////////////////

    std::string                         pID;
    steps::wm::Geom                   * pContainer;
    steps::wm::Comp                   * pIComp{nullptr};
    steps::wm::Comp                   * pOComp{nullptr};
    std::set<std::string>               pSurfsys;

    ////////////////////////////////////////////////////////////////////////

};

////////////////////////////////////////////////////////////////////////////////

} // namespace wm
} // namespace steps
