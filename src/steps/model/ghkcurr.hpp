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

#include <cassert>
#include <string>
#include <vector>
#include <map>

#include "util/common.h"

////////////////////////////////////////////////////////////////////////////////

namespace steps {
namespace model {

////////////////////////////////////////////////////////////////////////////////

// Forward declarations.
class GHKcurr;
class Surfsys;
class Model;
class ChanState;
class Spec;

// Auxiliary declarations.
typedef GHKcurr *                            GHKcurrP;
typedef std::map<std::string, GHKcurrP>     GHKcurrPMap;
typedef GHKcurrPMap::iterator                 GHKcurrPMapI;
typedef GHKcurrPMap::const_iterator           GHKcurrPMapCI;
typedef std::vector<GHKcurrP>                 GHKcurrPVec;
typedef GHKcurrPVec::iterator                 GHKcurrPVecI;
typedef GHKcurrPVec::const_iterator           GHKcurrPVecCI;

// Added due to problem with default map in constructor.
typedef std::map<std::string, double> MyMap;

////////////////////////////////////////////////////////////////////////////////
/// GHK current.
/// Current through a channel based on the GHK flux equation.
/// The GHK flux equation contains a term for the channel permeability, not
/// conductance (since this is not constant with changes in concentrations),
/// however it is assumed that single-channel SLOPE conductance will be supplied, in
/// which case we need to know a lot of information about the conductance
/// measurement in order to calculate the permeability constant.
/// We need to know at time of measurement: 1) the valence of the ion (which will
/// come from the species object and checked not to be zero), 2) the membrane
/// potential, 3) the intra and extra-cellular concentrations of the ion and 4)
/// the temperature.
/// 2,3 and 4 are conveniently all doubles, and can be supplied in a dict (map in c++),
/// e.g.   Ca_curr.setGMeasInfo({'temp':6.3, 'iconc': 5e-6})
/// If this information is not supplied, these will be taken from the initial
/// conditions in the simulation itself.
/// This information will then be used to find the single-channel permeability
/// to be used during the simulation.

/// \warning Methods start with an underscore are not exposed to Python.

class GHKcurr
{

public:

    ////////////////////////////////////////////////////////////////////////
    // OBJECT CONSTRUCTION & DESTRUCTION
    ////////////////////////////////////////////////////////////////////////
    /// Constructor
    ///
    /// \param id ID of the ohmic current reaction.
    /// \param surfsys Pointer to the parent surface system.
    /// \param ion The ion species which carries the current.
    /// \param g Single channel conductance (in siemens).
    ///
    GHKcurr(std::string const & id, Surfsys * surfsys,
            ChanState * chanstate, Spec * ion, bool computeflux = true,
            double virtual_oconc = -1.0, double vshift = 0.0);

    /// Destructor
    ~GHKcurr();

    ////////////////////////////////////////////////////////////////////////
    // GHK CURRENT PROPERTIES
    ////////////////////////////////////////////////////////////////////////

    /// Return the GHK current ID.
    ///
    /// \return ID of the GHK current.
    const std::string& getID() const
    { return pID; }

    /// Set or change the GHK current ID.
    ///
    /// \param id ID of the GHK current.
    void setID(std::string const & id);

    /// Return a pointer to the parent surface system.
    ///
    /// \return Pointer to the surface system.
    Surfsys * getSurfsys() const
    { return pSurfsys; }

    /// Return a pointer to the parent model.
    ///
    /// \return Pointer to the parent model.
    Model * getModel() const
    { return pModel; }

    /// Return a pointer to the associated channel state.
    ///
    /// \return Pointer to the channel state.
    ChanState * getChanState() const
    { return pChanState; }

    /// Change the channel state.
    ///
    /// \param chanstate Channel state of the open state.
    void setChanState(ChanState * chanstate);

    /// Return a pointer to the ion.
    ///
    /// \return Pointer to the ion.
    Spec * getIon() const
    { return pIon; }

    /// Change the ion.
    ///
    /// \param ion Ion species.
    void setIon(Spec * ion);

    /*
    /// Return the channel conductance (in siemens) at the specified conditions.
    ///
    /// \return Channel conductance associated with GHK current.
    double getG() const
    { return pG; }

    /// Change the channel conductance at the specified conditions.
    ///
    /// \param g Conductance associated with ohmic current.
    void setG(double g);

     // To expose, or not to expose. That is the question.
    /// Return the calculated conductance information.
    ///
    /// \return Conductance measurement information.
    std::map<std::string, double> getGInfo() const;
    */

    /// Set or change the permeability measurement information.
    ///
    /// \param ginfo Permeability meaurement information.
    void setPInfo(double g, double V, double T, double oconc, double iconc);

    /// Directly set or change the single-channel permeability.
    ///
    /// \param ginfo Permeability.
    void setP(double p);

    ////////////////////////////////////////////////////////////////////////
    // INTERNAL (NON-EXPOSED) OPERATIONS: DELETION
    ////////////////////////////////////////////////////////////////////////
    /// Self delete.
    ///
    /// Called if Python object deleted, or from del method in parent object.
    /// Will only be called once
    void _handleSelfDelete();

    ////////////////////////////////////////////////////////////////////////
    // INTERNAL (NON-EXPOSED) OPERATIONS: CONDUCTANCE INFORMATION
    ////////////////////////////////////////////////////////////////////////
    /// Return whether user has supplied conductance information or not.
    ///
    /// \Return Conductance information supplied bool
    bool _infosupplied() const noexcept
    { return pInfoSupplied; }

    double _G() const;
    int _valence() const;
    double _V() const;
    double _temp() const;
    double _oconc() const;
    double _iconc() const;

    double _P() const;

    ////////////////////////////////////////////////////////////////////////
    // INTERNAL (NON-EXPOSED) OPERATIONS
    ////////////////////////////////////////////////////////////////////////
    // Real flux flag
    bool _realflux() const noexcept
    { return pRealFlux; }

    double _voconc() const noexcept
    { return pVirtual_conc; }

    double _vshift() const noexcept
    { return pVshift; }

    ////////////////////////////////////////////////////////////////////////

private:

    ////////////////////////////////////////////////////////////////////////

    std::string                         pID;
    Model                             * pModel{nullptr};
    Surfsys                           * pSurfsys;
    ChanState                         * pChanState;
    Spec                              * pIon;
    bool                                 pRealFlux;

    // std::map<std::string, double>       pGInfo;

    ////////////////////////////////////////////////////////////////////////
    // CONDUCTANCE MEASUREMENT INFORMATION
    ////////////////////////////////////////////////////////////////////////
    // The measured conductance
    double                              pG;
    // The ion valence. This comes from Spec object
    int                                 pValence;
    // The potential
    double                                 pV;
    // The temperature IN KELVIN
    double                                 pTemp;
    // The inner concentration in Molar units
    double                                 pInnerConc;
    // The outer concentration in Molar units
    double                                 pOuterConc;

    // The single-channel permeability, if we have it
    double                                 pP;

    // True if we have all conductance measurement information.
    // An exception should be thrown at def level if this info is missing.
    bool                                 pInfoSupplied;

    // The 'virtual outer-concentration'. If this is set to a positive number
    // then the outer concentration will be taken from this number,
    // allowing GHK currents on surface of mesh with no outer compartment
    double                                pVirtual_conc;

    // Allowing a 'voltage shift' for the calcuation as this is used in
    // some models
    double                                 pVshift;

    ////////////////////////////////////////////////////////////////////////

};

////////////////////////////////////////////////////////////////////////////////

} // namespace model
} // namespace steps
