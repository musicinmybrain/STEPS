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


#ifndef STEPS_SOLVER_API_HPP
#define STEPS_SOLVER_API_HPP 1


// STL headers.
#include <string>
#include <limits>

// STEPS headers.
#include "geom/geom.hpp"
#include "model/model.hpp"
#include "rng/rng.hpp"
#include "util/common.h"
#include "util/vocabulary.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace steps {
namespace solver {

////////////////////////////////////////////////////////////////////////////////

// Forward declarations
class Statedef;

////////////////////////////////////////////////////////////////////////////////
/// API class for a solver.
///
/// The API class provides all APIs for each solver.
///
/// \warning Not every API in this class is implemented in the solver.
///          If a API is not implemented in a solver,
///          STEPS will throw an error message to the user.
/// \warning Methods start with underscore are not exposed to Python.
///////////////////////////////////////////////////////////////////////////////
class API
{
public:

    // Constants for describing E-Field solver choices
    enum EF_solver {
        EF_NONE = 0, // must be zero for API compatibility
        EF_DEFAULT = 1, // must be one for API compatibility
        EF_DV_BDSYS,
        EF_DV_PETSC,
    };

    /// Constructor
    ///
    /// \param m Pointer to the model.
    /// \param g Pointer to the geometry container.
    /// \param r Pointer to the random number generator.
    API(steps::model::Model *m, steps::wm::Geom *g, const rng::RNGptr &r);

    /// Destructor
    ///
    virtual ~API();

    ////////////////////////////////////////////////////////////////////////
    // SOLVER INFORMATION
    ////////////////////////////////////////////////////////////////////////

    /// Return the solver's name.
    virtual std::string getSolverName() const = 0;
    /// Return the solver's description.
    virtual std::string getSolverDesc() const = 0;
    /// Return the solver's author.
    virtual std::string getSolverAuthors() const = 0;
    /// Return the solver's email.
    virtual std::string getSolverEmail() const = 0;

    ////////////////////////////////////////////////////////////////////////
    // SOLVER CONTROLS
    ////////////////////////////////////////////////////////////////////////

    /// checkpoint simulator state to a file
    virtual void checkpoint(std::string const & file_name) = 0;

    /// restore simulator state from a file
    virtual void restore(std::string const & file_name) = 0;

    /// Reset the solver.
    virtual void reset() = 0;

    /// Run the solver until a given end time.
    ///
    /// \param endtime Time to end the solver.
    virtual void run(double endtime) = 0;

    /// Advance the solver a given time.
    ///
    /// \param adv Time to advance the solver
    virtual void advance(double adv);

    /// Run the solver for a step.
    virtual void step();

    /// Set DT of the numerical solver.
    ///
    /// \param dt Dt.
    virtual void setRk4DT(double dt);

    /// Set DT of the solver. Included for backwards compatibility but
    /// replaced by setRk4DT
    ///
    /// \param dt Dt.
    virtual void setDT(double dt);

    /// Set EField DT.
    ///
    /// \ param dt EField DT
    virtual void setEfieldDT(double efdt);

    virtual void setNSteps(uint nsteps);

    virtual void setTime(double time);

    virtual void setTemp(double temp);

    ////////////////////////////////////////////////////////////////////////
    // SOLVER STATE ACCESS:
    //      GENERAL
    ////////////////////////////////////////////////////////////////////////
    /// Return the time.
    virtual double getTime() const = 0;

    /// Return the DT of the numerical solver
    virtual double getRk4DT() const;

    /// Return the DT
    /// Replaced by getRk4DT, but included for backwards compatability
    virtual double getDT() const;

    /// Return the DT of the membrane potential solver
    /// \todo ask iain
    virtual double getEfieldDT() const;

    /// Return the simulation temperature
    virtual double getTemp() const;

    /// Return the A0
    /// \todo ask iain
    virtual double getA0() const;

    /// Return the number of steps.
    virtual uint getNSteps() const;

    ////////////////////////////////////////////////////////////////////////
    // SOLVER CONTROLS:
    //      COMPARTMENT
    ////////////////////////////////////////////////////////////////////////

    /// Returns the volume of compartment c (in m^3).
    ///
    /// \param c Name of the compartment.
    double getCompVol(std::string const & c) const;

    /// Sets the volume of compartment c.
    ///
    /// NOTE: this method may throw an exception if this does not make sense
    /// for a solver (e.g. a tetrahedral mesh-based solver).
    /// \param c Name of the compartment.
    /// \param vol Volume of the compartment.
    void setCompVol(std::string const & c, double vol);

    /// Returns the number of molecules of species s in compartment c.
    ///
    /// NOTE: in a mesh-based simulation, the total count is computed as
    /// the sum of the counts in all voxels of the compartment.
    /// \param c Name of the compartment.
    /// \param s Name of the species.
    double getCompCount(std::string const & c, std::string const & s) const;

    /// Sets the number of molecules of species s in compartment c.
    ///
    /// NOTE: in a mesh-based simulation, the total amount is equally divided
    /// over all voxels in the compartment (i.e. a uniform distribution).
    /// \param c Name of the compartment.
    /// \param s Name of the species.
    /// \param n Number of molecules of the species.
    void setCompCount(std::string const & c, std::string const & s, double n);

    /// Returns the amount (in mols) of species s in compartment c.
    ///
    /// NOTE: in a mesh-based simulation, the total amount is computed as
    /// the sum of the amounts in all voxels of the compartment.
    /// \param c Name of the compartment.
    /// \param s Name of the species.
    double getCompAmount(std::string const & c, std::string const & s) const;

    /// Set the amount (in mols) of species s in compartment c.
    ///
    /// NOTE: in a mesh-based simulation, the total amount is equally divided
    /// over all voxels in the compartment (i.e. a uniform distribution).
    /// \param c Name of the compartment.
    /// \param s Name of the species.
    /// \param a Amount of the species.
    void setCompAmount(std::string const & c, std::string const & s, double a);

    /// Returns the concentration (in molar units) of species s in compartment c.
    ///
    /// NOTE: in a mesh-based simulation, the overall concentration in a
    /// compartment is computed by taking the volume-weighted sum of the
    /// concentration in all voxels of the compartment.
    /// \param c Name of the compartment.
    /// \param s Name of the species.
    double getCompConc(std::string const & c, std::string const & s) const;

    /// Sets the concentration (in molar units) of species s in compartment c.
    ///
    /// NOTE: in a mesh-based simulation, this method changes the
    /// concentration to the same value in all voxels of the compartment.
    /// \param c Name of the compartment.
    /// \param s Name of the species.
    /// \param conc Concentration of the species.
    void setCompConc(std::string const & c, std::string const & s, double conc);

    /// Returns whether the concentration of species s in compartment c
    /// remains constant over time (unless changed explicitly).
    ///
    /// NOTE: in a mesh-based simulation, this method will only return true
    /// only if the species has been clamped in all voxels of the compartment.
    /// \param c Name of the compartment.
    /// \param s Name of the species.
    bool getCompClamped(std::string const & c, std::string const & s) const;

    /// Turns clamping of species s in compartment c on or off.
    ///
    /// NOTE: in a mesh based simulation, this method turns clamping on/off
    /// in all voxels of the compartment.
    /// \param c Name of the compartment.
    /// \param s Name of the species.
    /// \param b Flag to trun clamping of species on / off.
    void setCompClamped(std::string const & c, std::string const & s, bool b);

    // Returns the macroscopic reaction constant of reaction r in
    // compartment c.
    // Note: in a mesh-based simulation, the value is computed as the
    // volume-weighted sum of the reaction constants in all voxels of the
    // compartment.
    double getCompReacK(std::string const & c, std::string const & r) const;

    /// Sets the macroscopic reaction constant of reaction r in compartment c
    /// (units vary according to the order of the reaction).
    ///
    /// NOTE: in a mesh-based simulation, this method changes the reaction
    /// constant equally in all voxels of the compartment.
    /// \param c Name of the compartment.
    /// \param r Name of te reaction.
    /// \param kf Reaction constant.
    void setCompReacK(std::string const & c, std::string const & r, double kf);

    /// Returns whether reaction r in compartment c is active or not
    ///
    /// NOTE: in a mesh-based simulation, this method returns false only when
    /// the reaction has been inactivated in all voxels.
    /// \param c Name of the compartment.
    /// \param r Name of the reaction.
    bool getCompReacActive(std::string const & c, std::string const & r) const;

    /// Activate or inactivate reaction r in compartment c.
    ///
    /// NOTE: in a mesh-based simulation, activation/inactivation of a reaction
    /// turns it on/off in all voxels.
    /// \param c Name of the compartment.
    /// \param r Name of the reaction.
    /// \param a Flag to activate or deactivate the reaction.
    void setCompReacActive(std::string const & c, std::string const & r, bool a);

    /// Returns the diffusion constant of diffusion rule d in compartment c.
    ///
    /// \param c Name of the compartment.
    /// \param d Name of the diffusion.
    double getCompDiffD(std::string const & c, std::string const & d) const;

    /// Set the diffusion constant of diffusion rule d in compartment c.
    ///
    /// \param c Name of the compartment.
    /// \param d Name of the diffusion.
    /// \param dcst Rate constant of the diffusion.
    void setCompDiffD(std::string const & c, std::string const & d, double dcst);

    /// Returns whether diffusion rule d in compartment c is active or not.
    ///
    /// \param c Name of the compartment.
    /// \param d Name of the diffusion.
    bool getCompDiffActive(std::string const & c, std::string const & d) const;

    /// Activate or deactivate diffusion rule d in compartment c.
    ///
    /// \param c Name of the compartment.
    /// \param d Name of the diffusion.
    /// \param act Flag to activate or deactivate the diffusion.
    void setCompDiffActive(std::string const & c, std::string const & d, bool act);

    ////////////////////////////////////////////////////////////////////////

    /// Returns c_mu, the mesoscopic reaction constant of reaction r in
    /// compartment c.
    ///
    /// NOTE: in a mesh-based simulation, the mesoscopic reaction constant is
    /// computed as the sum of the mesoscopic constants in all voxels of the
    /// compartment.
    /// \param c Name of the compartment.
    /// \param r Name of the reaction.
    double getCompReacC(std::string const & c, std::string const & r) const;

    /// Returns h_mu, the distinct number of ways in which reaction r can
    /// occur in compartment c, by computing the product of its reactants.
    ///
    /// NOTE: in a mesh-based simulation, it returns the sum of the h_mu's
    /// over all voxels of the compartment. This can become a very large value.
    /// \param c Name of the compartment.
    /// \param r Name of the reaction.
    double getCompReacH(std::string const & c, std::string const & r) const;

    /// Returns the propensity, a_mu, of reaction r in compartment c.
    /// The propensity value gives the probability per unit time that this
    /// reaction will occur in the current state.
    ///
    /// NOTE: in a mesh-based simulation, a_mu is computed as the sum of the
    /// a_mu in all voxels of the compartment.
    /// \param c Name of the compartment.
    /// \param r Name of the reaction.
    double getCompReacA(std::string const & c, std::string const & r) const;

    /// Returns the extent of reaction r in compartment c.
    ///
    /// NOTE: in a mesh-based simulation, returns the sum of the reaction
    /// extents in all voxels of the compartment.
    /// \param c Name of the compartment.
    /// \param r Name of the reaction.
    unsigned long long getCompReacExtent(std::string const & c, std::string const & r) const;

    /// Resets the extent of reaction r in compartment c to zero.
    ///
    /// NOTE: in a mesh-based simulation, resets the extents of the reaction
    /// in all voxels of the compartment.
    /// \param c Name of the compartment.
    /// \param r Name of the reaction.
    void resetCompReacExtent(std::string const & c, std::string const & r);

    ////////////////////////////////////////////////////////////////////////
    // SOLVER CONTROLS:
    //      TETRAHEDRAL VOLUME ELEMENTS
    ////////////////////////////////////////////////////////////////////////

    /// Returns the volume of a tetrahedron (in m^3).
    ///
    /// \param tidx Index of the tetrahedron.
    double getTetVol(tetrahedron_id_t tidx) const;

    /// Set the volume of a tetrahedron (in m^3).
    ///
    /// \param tidx Index of the tetrahedron.
    /// \param vol Volume of the tetrahedron.
    void setTetVol(tetrahedron_id_t tidx, double vol);

    /// Returns whether species s is defined in a tetrahedral volume
    /// element (voxel).
    ///
    /// \param tidx Index of the tetrahedron.
    /// \param s Name of the species.
    bool getTetSpecDefined(tetrahedron_id_t tidx, std::string const & s) const;

    /// Returns the number of molecules of species s in a tetrahedral volume
    /// element (voxel).
    ///
    /// \param tidx Index of the tetrahedron.
    /// \param s Name of the species.
    double getTetCount(tetrahedron_id_t tidx, std::string const & s) const;

    /// Sets the number of molecules of species s in a voxel.
    ///
    /// \param tidx Index of the tetrahedron.
    /// \param s Name of the species.
    /// \param n Number of molecules of the species.
    void setTetCount(tetrahedron_id_t tidx, std::string const & s, double n);

    /// Returns the amount (in mols) of species s in a voxel.
    ///
    /// \param tidx Index of the tetrahedron.
    /// \param s Name of the species.
    double getTetAmount(tetrahedron_id_t tidx, std::string const & s) const;

    /// Sets the amount (in mols) of species s in a voxel.
    ///
    /// \param tidx Index of the tetrahedron.
    /// \param s Name of the species.
    /// \param m Amount of the species.
    void setTetAmount(tetrahedron_id_t tidx, std::string const & s, double m);

    /// Returns the concentration (in molar units) of species s in a voxel..
    ///
    /// \param tidx Index of the tetrahedron.
    /// \param s Name of the species.
    double getTetConc(tetrahedron_id_t tidx, std::string const & s) const;

    /// Sets the concentration (in molar units) of species s in a voxel.
    ///
    /// \param tidx Index of the tetrahedron.
    /// \param s Name of the species.
    /// \param c Concentration of the species.
    void setTetConc(tetrahedron_id_t tidx, std::string const & s, double c);

    /// Returns whether the concentration of species s in a voxel
    /// remains constant over time (unless changed explicitly).
    ///
    /// \param tidx Index of the tetrahedron.
    /// \param s Name of the species.
    bool getTetClamped(tetrahedron_id_t tidx, std::string const & s) const;

    /// Sets clamping of species s in a voxel on or off.
    ///
    /// \param tidx Index of the tetrahedron.
    /// \param s Name of the species.
    /// \param buf Flag to turn the clamping of species on or off.
    void setTetClamped(tetrahedron_id_t tidx, std::string const & s, bool buf);

    /// Returns the macroscopic reaction constant of reaction r in a voxel
    /// (units vary with order of reaction).
    ///
    /// \param tidx Index of the tetrahedron.
    /// \param r Name of the reaction.
    double getTetReacK(tetrahedron_id_t tidx, std::string const & r) const;

    /// Sets the macroscopic reaction constant of reaction r in a voxel.
    ///
    /// \param tidx Index of the tetrahedron.
    /// \param r Name of the reaction.
    /// \param kf Rate constant of the reaction.
    void setTetReacK(tetrahedron_id_t tidx, std::string const & r, double kf);

    /// Returns whether reaction r in a voxel is active or not
    ///
    /// \param tidx Index of the tetrahedron.
    /// \param r Name of the reaction.
    bool getTetReacActive(tetrahedron_id_t tidx, std::string const & r) const;

    /// Activates/deactivates reaction r in a voxel.
    ///
    /// \param tidx Index of the tetrahedron.
    /// \param r Name of the reaction.
    /// \param act Flag to activate or deactivate the reaction.
    void setTetReacActive(tetrahedron_id_t tidx, std::string const & r, bool act);

    /// Returns the diffusion constant of diffusion rule d in a voxel.
    ///
    /// \param tidx Index of the tetrahedron.
    /// \param d Name of the deffusion.
    /// \param direction_tet Tetrahedron index which specifies diffusion direction.
    double getTetDiffD(tetrahedron_id_t tidx, std::string const &d,
                       tetrahedron_id_t direction_tet) const;

    /// Sets the diffusion constant of diffusion rule d in a voxel.
    ///
    /// \param tidx Index of the tetrahedron.
    /// \param d Name of the diffusion.
    /// \param dk Rate constant of the diffusion.
    /// \param direction_tet Tetrahedron index which the diffusion towards.
        void setTetDiffD(
        tetrahedron_id_t tidx, std::string const &d, double dk,
        tetrahedron_id_t direction_tet);

    /// Returns whether diffusion rule d in a voxel is active or not.
    ///
    /// \param tidx Index of the tetrahedron.
    /// \param d Name of the diffusion.
    bool getTetDiffActive(tetrahedron_id_t tidx, std::string const & d) const;

    /// Activates/deactivates diffusion rule d in a voxel.
    ///
    /// \param tidx Index of the tetrahedron.
    /// \param d Name of the diffusion.
    /// \param act Flag to activate / deactivate the diffusion.
    void setTetDiffActive(tetrahedron_id_t tidx, std::string const & d, bool act);

    ////////////////////////////////////////////////////////////////////////

    /// Returns c_mu, the mesoscopic reaction constant of reaction r in
    /// a voxel
    ///
    /// \param tidx Index of the diffusion.
    /// \param r Name of the reaction.
    double getTetReacC(tetrahedron_id_t tidx, std::string const & r) const;

    /// Returns h_mu, the distinct number of ways in which reaction r can
    /// occur in a voxel, by computing the product of its reactants.
    ///
    /// \param tidx Index of the tetrahedron.
    /// \param Name of the reaction.
    double getTetReacH(tetrahedron_id_t tidx, std::string const & r) const;

    /// Returns the propensity, a_mu, of reaction r in a voxel.
    /// The propensity value gives the probability per unit time that this
    /// reaction will occur in the current state.
    ///
    /// \param tidx Index of the tetrahedron.
    /// \param r Name of the reaction.
    double getTetReacA(tetrahedron_id_t tidx, std::string const & r) const;

    /// Returns the propensity, a_mu of diffusion rule d in a voxel.
    ///
    /// \param tidx Index of the tetrahedron.
    /// \param d Name of the diffusion.
    double getTetDiffA(tetrahedron_id_t tidx, std::string const & d) const;

    ////////////////////////////////////////////////////////////////////////

    /// Returns the potential of tetrahedron in Volts.
    ///
    /// \param tidx Index of the tetrahedron.
    double getTetV(tetrahedron_id_t tidx) const;

    /// Set the potential of tetrahedron.
    ///
    /// \param tidx Index of the tetrahedron
    /// \param V Potential in volts.
    void setTetV(tetrahedron_id_t tidx, double v);

    /// Returns whether the potential of tetrahedron is clamped over time
    /// (unless changed explicitly)
    ///
    /// \param tidx Index of the tetrahedron
    bool getTetVClamped(tetrahedron_id_t tidx) const;

    /// Sets voltage clamp in tetrahedron.
    ///
    /// \param tidx Index of the tetrahedron
    /// \param cl Flag to turn the clamping on or off.
    void setTetVClamped(tetrahedron_id_t tidx, bool cl);


    ////////////////////////////////////////////////////////////////////////
    // SOLVER CONTROLS:
    //      PATCH
    ////////////////////////////////////////////////////////////////////////

    /// Returns the area of patch p (in m^2)
    ///
    /// \param p Name of the patch.
    double getPatchArea(std::string const & p) const;

    /// Sets the area of patch p.
    ///
    /// NOTE: this method may throw an exception if this does not make sense
    /// for a solver (e.g. a tetrahedral mesh-based solver).
    /// \param p Name of the patch.
    /// \param area Area of the patch.
    void setPatchArea(std::string const & p, double area);

    /// Returns the number of molecules of species s in patch p.
    ///
    /// NOTE: in a mesh-based simulation, the total count is computed as
    /// the sum of the counts in all triangles of the patch.
    /// \param p Name of the path.
    /// \param s Name of the species.
    double getPatchCount(std::string const & p, std::string const & s) const;

    /// Sets the number of molecules of species s in patch p.
    ///
    /// NOTE: in a mesh-based simulation, the total amount is equally divided
    /// over all triangles in the patch.
    /// \param p Name of the patch.
    /// \param s Name of the species.
    /// \param n Number of molecules of species.
    void setPatchCount(std::string const & p, std::string const & s, double n);

    /// Returns the amount (in mols) of species s in patch p.
    ///
    /// NOTE: in a mesh-based simulation, the total amount is computed as
    /// the sum of the amounts in all triangles of the patch.
    /// \param p Name of the patch.
    /// \param s Name of the species.
    double getPatchAmount(std::string const & p, std::string const & s) const;

    /// Sets the amount (in mols) of species s in patch p.
    ///
    /// NOTE: in a mesh-based simulation, the total amount is equally divided
    /// over all triangles in the patch.
    /// \param p Name of the patch.
    /// \param s Name of the species.
    /// \param a Amount of the species.
    void setPatchAmount(std::string const & p, std::string const & s, double a);

    /// Returns whether the count of species s in patch p remains constant.
    /// over time (unless changed explicitly).
    ///
    /// NOTE: this method will only return true if the species has been
    /// clamped in all triangles in the patch.
    /// \param p Name of the patch.
    /// \param s Name of the species.
    bool getPatchClamped(std::string const & p, std::string const & s) const;

    /// Turns clamping of species in patch p on or off.
    ///
    /// NOTE: in a mesh-based simulation, this method turns clamping on/off
    /// in all triangles in the patch.
    /// \param p Name of the patch.
    /// \param s Name of the species.
    /// \param buf Flag to turn clamping of species on /off.
    void setPatchClamped(std::string const & p, std::string const & s, bool buf);

    /// Returns the macroscopic reaction constant of surface reaction r
    /// in patch p.
    ///
    /// NOTE: in a mesh-based simulation, the value is computed as the
    /// area-weighted sum of the reaction constants in all triangles of
    /// the patch.
    /// \param p Name of the patch.
    /// \param r Name of the reaction.
    double getPatchSReacK(std::string const & p, std::string const & r) const;

    /// Sets the macroscopic reaction constant of surface reaction r
    /// in patch p.
    ///
    /// NOTE: in a mesh-based simulation this method changes the reaction
    /// constant equally in all triangles of the patch.
    /// \param p Name of the patch.
    /// \param r Name of the reaction.
    /// \param kf Rate constant of the reaction.
    void setPatchSReacK(std::string const & p, std::string const & r, double kf);

    /// Returns whether surface reaction r in patch p is active or not.
    ///
    /// NOTE: in a mesh-based simulation, only returns false when the
    /// reaction has been inactivated in all triangles.
    /// \param p Name of the patch.
    /// \param r Name of the reaction.
    bool getPatchSReacActive(std::string const & p, std::string const & r) const;

    /// Activate or inactivate surface reaction r in patch p.
    ///
    /// NOTE: in a mesh-based simulation, activation/inactivation of a
    /// surface reaction turns it on/off in all triangles.
    /// \param p Name of the patch.
    /// \param r Name of the reaction.
    /// \param a Flag to activate / deactivate the reaction.
    void setPatchSReacActive(std::string const & p, std::string const & r, bool a);

    ////////////////////////////////////////////////////////////////////////

    /// Returns c_mu, the mesoscopic reaction constant of surface reaction r
    /// in patch p.
    ///
    /// NOTE: in a mesh_based simulation, the mesoscopic reaction constant
    /// is computed as the sum of the mesoscopic reaction constants from all
    /// triangles of the patch.
    /// \param p Name of the patch.
    /// \param r Name of the reacton.
    double getPatchSReacC(std::string const & p, std::string const & r) const;

    /// Returns h_mu, the distinct number of ways in which a surface reaction
    /// r can occur in patch p, by computing the product of its reactants.
    ///
    /// NOTE: in a mesh-based simulation, it returns the sum of the h_mu's
    /// over all triangles triangles of the patch.
    /// \param p Name of the patch.
    /// \param r Name of the reaction.
    double getPatchSReacH(std::string const & p, std::string const & r) const;

    /// Returns the propensity, a_mu of surface reaction r in patch p.
    ///
    /// This propensity value gives the probability per unit time that this
    /// surface reaction will occur in the current state.
    ///
    /// NOTE: in a mesh-based simulation, a_mu is computed as the sum of the
    /// a_mu in all triangles in the patch.
    /// \param p Name of the patch.
    /// \param r Name of the reaction.
    double getPatchSReacA(std::string const & p, std::string const & r) const;

    /// Returns the extent of surface reaction r in patch p.
    ///
    /// NOTE: in a mesh-based simulation, returns the sum of the extents in
    /// all triangles of the patch.
    /// \param p Name of the patch.
    /// \param r Name of the reaction.
    unsigned long long getPatchSReacExtent(std::string const & p, std::string const & r) const;

    /// Resets the extent of surface reaction r in patch p to zero.
    ///
    /// NOTE: in a mesh-based simulation, resets the extents of the
    /// surface reaction in all triangles of the patch.
    /// \param p Name of the patch.
    /// \param r Name of the reaction.
    void resetPatchSReacExtent(std::string const & p, std::string const & r);

    /// Returns whether voltage-dependent surface reaction vsr in patch p is active or not.
    ///
    /// NOTE: only returns false when the voltage-dependent surface
    /// reaction has been inactivated in all triangles.
    /// \param p Name of the patch.
    /// \param vsr Name of the voltage-dependent surface reaction.
    bool getPatchVDepSReacActive(std::string const & p, std::string const & vsr) const;

    /// Activate or inactivate voltage-dependent surface reaction vsr in patch p.
    ///
    /// NOTE: activation/inactivation of a voltage-dependent
    /// surface reaction turns it on/off in all triangles.
    /// \param p Name of the patch.
    /// \param vsr Name of the voltage-dependent surface reaction.
    /// \param a Flag to activate / deactivate the reaction.
    void setPatchVDepSReacActive(std::string const & p, std::string const & vsr, bool a);

    ////////////////////////////////////////////////////////////////////////
    // SOLVER STATE ACCESS:
    //      DIFFUSION BOUNDARIES
    ////////////////////////////////////////////////////////////////////////

    /// Activate or inactivate diffusion across a diffusion boundary for a species.
    ///
    /// \param db Name of the diffusion boundary.
    /// \param s Name of the species.
    /// \param act Bool to activate (true) or inactivate (false) diffusion.
    void setDiffBoundaryDiffusionActive(std::string const & db, std::string const & s, bool act);

    /// Returns whether diffusion is active across a diffusion boundary for a species.
    ///
    /// \param db Name of the diffusion boundary.
    /// \param s Name of the species.
    bool getDiffBoundaryDiffusionActive(std::string const & db, std::string const & s) const;

    /// Set the diffusion constant across a diffusion boundary.
    ///
    /// \param db Name of the diffusion boundary.
    /// \param s Name of the species.
    /// \param dcst diffusion constant.
    /// \param direction_comp ID of the compartment which the diffusion towards to.
    void setDiffBoundaryDcst(std::string const & db, std::string const & s, double dcst, std::string const & direction_comp = "");

    ////////////////////////////////////////////////////////////////////////
    // SOLVER STATE ACCESS:
    //      SURFACE DIFFUSION BOUNDARIES
    ////////////////////////////////////////////////////////////////////////

    /// Activate or inactivate diffusion across a surface diffusion boundary for a species.
    ///
    /// \param sdb Name of the surface diffusion boundary.
    /// \param s Name of the species.
    /// \param act Bool to activate (true) or inactivate (false) diffusion.
    void setSDiffBoundaryDiffusionActive(std::string const & sdb, std::string const & s, bool act);

    /// Returns whether diffusion is active across a surface diffusion boundary for a species.
    ///
    /// \param sdb Name of the surface diffusion boundary.
    /// \param s Name of the species.
    bool getSDiffBoundaryDiffusionActive(std::string const & sdb, std::string const & s) const;

    /// Set the diffusion constant across a surface diffusion boundary.
    ///
    /// \param sdb Name of the surface diffusion boundary.
    /// \param s Name of the species.
    /// \param dcst diffusion constant.
    /// \param direction_patch ID of the patch which the diffusion is towards to.
    void setSDiffBoundaryDcst(std::string const & sdb, std::string const & s, double dcst, std::string const & direction_patch = "");

    ////////////////////////////////////////////////////////////////////////
    // SOLVER CONTROLS:
    //      TRIANGULAR SURFACE ELEMENTS
    ////////////////////////////////////////////////////////////////////////

    /// Returns the area of the triangle (in m^2).
    ///
    /// \param tidx Index of the triangle.
    double getTriArea(triangle_id_t tidx) const;

    /// Set the area (in m^2) of the triangle.
    ///
    /// \param tidx Index of the triangle.
    /// \param area Area of teh triangle.
    void setTriArea(triangle_id_t tidx, double area);

    /// Returns whether species s is defined in a triangle
    ///
    /// \param tidx Index of the triangle.
    /// \param s Name of the species.
    bool getTriSpecDefined(triangle_id_t tidx, std::string const & s) const;

    /// Returns the number of molecules of species s in a triangle.
    ///
    /// \param tidx Index of the triangle.
    /// \param s Name of the species.
    double getTriCount(triangle_id_t tidx, std::string const & s) const;

    /// Sets the number of molecules of species s in a triangle.
    ///
    /// \param tidx Index of the triangle.
    /// \param s Name of the species.
    /// \param n Number of molecules of the species.
    void setTriCount(triangle_id_t tidx, std::string const & s, double n);

    /// Returns the amount (in mols) of species s in a triangle.
    ///
    /// \param tidx Index of the triangle.
    /// \param s Name of the species.
    double getTriAmount(triangle_id_t tidx, std::string const & s) const;

    /// Sets the amount (in mols) of species s in a triangle.
    ///
    /// \param tidx Index of the triangle.
    /// \param s Name of the species.
    /// \param m Amount of the species.
    void setTriAmount(triangle_id_t tidx, std::string const & s, double m);

    /// Returns whether the number of molecules of species s in a triangle
    /// remains constant over time (unless changed explicitly)
    ///
    /// \param tidx Index of the triangle.
    /// \param s name of the species.
    bool getTriClamped(triangle_id_t tidx, std::string const & s) const;

    /// Sets clamping of species s in a triangle on or off.
    ///
    /// \param tidx Index of the triangle.
    /// \param s name of the species.
    /// \param buf Flag to set clamping of species on /off.
    void setTriClamped(triangle_id_t tidx, std::string const & s, bool buf);

    /// Returns the macroscopic reaction constant of surface reaction r
    // in a triangle (units vary with order of reaction).
    ///
    /// \param tidx Index of the triangle.
    /// \param r name of the reaction.
    double getTriSReacK(triangle_id_t tidx, std::string const & r);

    /// Sets the macroscopic reaction constant of surface reaction r in
    // a triangle.
    ///
    /// \param tidx Index of the triangle.
    /// \param r name of the reaction.
    /// \param kf Rate constant of the reaction.
    void setTriSReacK(triangle_id_t tidx, std::string const & r, double kf);

    /// Returns whether surface reaction r in a triangle is active or not.
    ///
    /// \param tidx Index of the triangle.
    /// \param r name of the reaction.
    bool getTriSReacActive(triangle_id_t tidx, std::string const & r);

    /// Activates/inactivates surface reaction r in a triangle.
    ///
    /// \param tidx Index of the triangle.
    /// \param r name of the reaction.
    /// \param act Flag to activate / deactivate the reaction.
    void setTriSReacActive(triangle_id_t tidx, std::string const & r, bool act);

    ////////////////////////////////////////////////////////////////////////

    // Returns c_mu, the mesoscopic reaction constant of surface reaction r
    // in a triangle.
    ///
    /// \param tidx Index of the triangle.
    /// \param r name of the reaction.
    double getTriSReacC(triangle_id_t tidx, std::string const & r);

    // Returns h_mu, the distinct number of ways in which surface reaction r
    // can occur in a triangle, by computing the product of it's reactants.
    ///
    /// \param tidx Index of the triangle.
    /// \param r name of the reaction.
    double getTriSReacH(triangle_id_t tidx, std::string const & r);

    // Returns the propensity, a_mu, of surface reaction r in a triangle.
    // The propensity value gives the probability per unit time that this
    // surface reaction will occur in the current state.
    ///
    /// \param tidx Index of the triangle.
    /// \param r name of the reaction.
    double getTriSReacA(triangle_id_t tidx, std::string const & r);

    /// outdate function
    double getTriDiffD(triangle_id_t tidx, std::string const & d, uint direction_tri = std::numeric_limits<uint>::max());

    /// Returns the diffusion constant of diffusion rule d in a triangle.
    ///
    /// \param tidx Index of the triangle.
    /// \param d Name of the diffusion.
    /// \param direction_tet Triangle index which specifies diffusion direction.
    double getTriSDiffD(triangle_id_t tidx, std::string const & d, triangle_id_t direction_tri);

    /// outdated function
    void setTriDiffD(triangle_id_t tidx, std::string const & d, double dk,
                      triangle_id_t direction_tri);


    /// Sets the diffusion constant of diffusion rule d on a triangle.
    ///
    /// \param tidx Index of the triangle.
    /// \param d Name of the diffusion.
    /// \param dk Rate constant of the diffusion.
    /// \param direction_tri Triangle index which the diffusion towards
    void setTriSDiffD(triangle_id_t tidx, std::string const & d, double dk,
                     triangle_id_t direction_tri);
    ////////////////////////////////////////////////////////////////////////

    /// Returns the potential of triangle in Volts.
    ///
    /// \param tidx Index of the triangle.
    double getTriV(triangle_id_t tidx) const;

    /// Set the potential of triangle.
    ///
    /// \param tidx Index of the triangle
    /// \param V Potential in volts.
    void setTriV(triangle_id_t tidx, double v);

    /// Returns whether the potential of triangle is clamped over time
    /// (unless changed explicitly).
    ///
    /// \param tidx Index of the triangle
    bool getTriVClamped(triangle_id_t tidx) const;

    /// Sets voltage clamp in triangle.
    ///
    /// \param tidx Index of the triangle
    /// \param cl Flag to turn the clamping on or off.
    void setTriVClamped(triangle_id_t tidx, bool cl);

    /// Returns the ohmic current of triangle in amperes.
    ///
    /// \param tidx Index of the triangle.
    double getTriOhmicI(triangle_id_t tidx) const;

    /// Returns the ohmic current of triangle in amperes.
    ///
    /// \param tidx Index of the triangle.
    /// \param oc name of the ohmic current
    double getTriOhmicI(triangle_id_t tidx, std::string const & oc) const;

    /// Returns the GHK current of triangle in amperes.
    ///
    /// \param tidx Index of the triangle.
    double getTriGHKI(triangle_id_t tidx) const;

    /// Returns the GHK current of triangle in amperes.
    ///
    /// \param tidx Index of the triangle.
    /// \param ghk name of the ghk current
    double getTriGHKI(triangle_id_t tidx, std::string const & ghk) const;

    /// Returns the current of a triangle in amperes from the last EField
    /// calculation step.
    ///
    /// \param tidx Index of the triangle
    double getTriI(triangle_id_t tidx) const;

    /// Gets current injection to triangle.
    /// \param tidx Index of the triangle
    double getTriIClamp(triangle_id_t tidx) const;

    /// Sets current injection to triangle.
    /// Will be assumed to be constant for one EField DT
    /// \param tidx Index of the triangle
    /// \param I Current in amperes.
    void setTriIClamp(triangle_id_t tidx, double i);

    /// Returns whether voltage-dependent surface reaction vsr in a triangle is active or not.
    ///
    /// \param tidx Index of the triangle.
    /// \param vsr name of the voltage-dependent surface reaction.
    bool getTriVDepSReacActive(triangle_id_t tidx, std::string const & vsr);

    /// Activates/inactivates voltage-dependent surface reaction vsr in a triangle.
    ///
    /// \param tidx Index of the triangle.
    /// \param vsr name of the voltage-dependent surface reaction.
    /// \param act Flag to activate / deactivate the reaction.
    void setTriVDepSReacActive(triangle_id_t tidx, std::string const & vsr, bool act);

    /// Set the specific capacitance of a triangle surface element.
    ///
    /// \param tidx Index of the triangle surface element
    /// \param cm Specific membrane capacitance (farad / m^2)
    void setTriCapac(triangle_id_t tidx, double cm);

    ////////////////////////////////////////////////////////////////////////
    // SOLVER CONTROL:
    //      VERTICES ELEMENTS
    ////////////////////////////////////////////////////////////////////////

    /// Returns the potential of vertex in Volts.
    ///
    /// \param vidx Index of the vertex.
    double getVertV(vertex_id_t vidx) const;

    /// Set the potential of vertex.
    ///
    /// \param vidx Index of the vertex
    /// \param V Potential in volts.
    void setVertV(vertex_id_t vidx, double v);

    /// Returns whether the potential of vertex is clamped over time
    /// (unless changed explicitly).
    ///
    /// \param vidx Index of the vertex
    bool getVertVClamped(vertex_id_t vidx) const;

    /// Sets voltage clamp in vertex.
    ///
    /// \param vidx Index of the vertex
    /// \param cl Flag to turn the clamping on or off.
    void setVertVClamped(vertex_id_t vidx, bool cl);

    /// Gets current injection to vertex.
    /// \param vidx Index of the vertex
    double getVertIClamp(vertex_id_t vidx) const;

    /// Sets current injection to vertex.
    /// Will be assumed to be constant for one EField DT
    /// \param vidx Index of the vertex
    /// \param I Current in amperes.
    void setVertIClamp(vertex_id_t vidx, double i);

    ////////////////////////////////////////////////////////////////////////
    // SOLVER CONTROL:
    //      MEMBRANES
    ////////////////////////////////////////////////////////////////////////

    /// Set the electric potential of the membrane, including all nodes
    /// in the conduction volume.
    /// \param m Name of the membrane
    /// \param v Potential (volts)
    void setMembPotential(std::string const & m, double v);

    /// Set the specific membrane capacitance of the membrane
    /// \param m Name of the membrane
    /// \param cm Specific membrane capacitance (farad / m^2)
    void setMembCapac(std::string const & m, double cm);

    /// Set the bulk electrical resistivity of the section of the mesh representing
    /// the volume conductor
    /// \param m Name of the membrane
    /// \param ro Electrical resistivity (ohm.m)
    void setMembVolRes(std::string const & m, double ro);

    /// Set the resistivity of the membrane
    /// \param m Name of the membrane
    /// \param ro membrane resistivity (ohm.m^2)
    /// \param vrev Reversal potential (Volts)
    void setMembRes(std::string const & m, double ro, double vrev);

    ////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////
    // DATA RECORDING:
    ////////////////////////////////////////////////////////////////////////
    /// Count the number of compartments
    uint getNComps() const;

    /// Count the number of patches
    uint getNPatches() const;

    /// Get a compartment's name by its index in the solver
    std::string getCompName(uint c_idx) const;

    /// Get a patch's name by its index in the solver
    std::string getPatchName(uint p_idx) const;

    /// Get number of species in a compartment
    uint getNCompSpecs(uint c_idx) const;

    /// Get number of species in a patch
    uint getNPatchSpecs(uint p_idx) const;

    /// Get species name of a compartment
    std::string getCompSpecName(uint c_idx, uint s_idx) const;

    /// Get species name of a patch
    std::string getPatchSpecName(uint p_idx, uint s_idx) const;


    ////////////////////////////////////////////////////////////////////////
    // Batch Data Access
    ////////////////////////////////////////////////////////////////////////

    /// Get species counts of a list of tetrahedrons
    virtual std::vector<double> getBatchTetCounts(const std::vector<index_t> &tets, std::string const &s) const;

    /// Get species counts of a list of triangles
    virtual std::vector<double> getBatchTriCounts(const std::vector<index_t> &tris, std::string const &s) const;

    /// Get species counts of a list of tetrahedrons
    virtual void getBatchTetCountsNP(const index_t *indices,
                                     size_t input_size,
                                     std::string const &s,
                                     double *counts,
                                     size_t output_size) const;

    /// Get species counts of a list of triangles
    virtual void getBatchTriCountsNP(const index_t *indices,
                                     size_t input_size,
                                     std::string const &s,
                                     double *counts,
                                     size_t output_size) const;

    ////////////////////////////////////////////////////////////////////////
    // ROI Data Access
    ////////////////////////////////////////////////////////////////////////

    /// Get species counts of a list of tetrahedrons
    virtual std::vector<double> getROITetCounts(const std::string& ROI_id, std::string const & s) const;

    /// Get species counts of a list of triangles
    virtual std::vector<double> getROITriCounts(const std::string& ROI_id, std::string const & s) const;

    /// Get species counts of a list of tetrahedrons
    virtual void getROITetCountsNP(const std::string& ROI_id, std::string const & s, double* counts, size_t output_size) const;

    /// Get species counts of a list of triangles
    virtual void getROITriCountsNP(const std::string& ROI_id, std::string const & s, double* counts, size_t output_size) const;

    virtual double getROIVol(const std::string& ROI_id) const;
    virtual double getROIArea(const std::string& ROI_id) const;

    virtual double getROICount(const std::string& ROI_id, std::string const & s) const;
    virtual void setROICount(const std::string& ROI_id, std::string const & s, double count);

    virtual double getROIAmount(const std::string& ROI_id, std::string const & s) const;
    virtual void setROIAmount(const std::string& ROI_id, std::string const & s, double);

    virtual double getROIConc(const std::string& ROI_id, std::string const & s) const;
    virtual void setROIConc(const std::string& ROI_id, std::string const & s, double conc);

    virtual void setROIClamped(const std::string& ROI_id, std::string const & s, bool b);

    virtual void setROIReacK(const std::string& ROI_id, std::string const & r, double kf);
    virtual void setROISReacK(const std::string& ROI_id, std::string const & sr, double kf);
    virtual void setROIDiffD(const std::string& ROI_id, std::string const & d, double dk);

    virtual void setROIReacActive(const std::string& ROI_id, std::string const & r, bool a);
    virtual void setROISReacActive(const std::string& ROI_id, std::string const & sr, bool a);
    virtual void setROIDiffActive(const std::string& ROI_id, std::string const & d, bool act);
    virtual void setROIVDepSReacActive(const std::string& ROI_id, std::string const & vsr, bool a);

    virtual unsigned long long getROIReacExtent(const std::string& ROI_id, std::string const & r) const;
    virtual void resetROIReacExtent(const std::string& ROI_id, std::string const & r);

    virtual unsigned long long getROISReacExtent(const std::string& ROI_id, std::string const & sr) const;
    virtual void resetROISReacExtent(const std::string& ROI_id, std::string const & sr);

    virtual unsigned long long getROIDiffExtent(const std::string& ROI_id, std::string const & d) const;
    virtual void resetROIDiffExtent(const std::string& ROI_id, std::string const & s);

protected:

    ////////////////////////////////////////////////////////////////////////
    // SOLVER CONTROL:
    //      COMPARTMENT
    ////////////////////////////////////////////////////////////////////////

    virtual double _getCompVol(uint cidx) const = 0;
    virtual void _setCompVol(uint cidx, double vol);

    virtual double _getCompCount(uint cidx, uint sidx) const = 0;
    virtual void _setCompCount(uint cidx, uint sidx, double n) = 0;

    virtual double _getCompAmount(uint cidx, uint sidx) const = 0;
    virtual void _setCompAmount(uint cidx, uint sidx, double a) = 0;

    virtual double _getCompConc(uint cidx, uint sidx) const = 0;
    virtual void _setCompConc(uint cidx, uint sidx, double c) = 0;

    virtual bool _getCompClamped(uint cidx, uint sidx) const = 0;
    virtual void _setCompClamped(uint cidx, uint sidx, bool b) = 0;

    virtual double _getCompReacK(uint cidx, uint ridx) const = 0;
    virtual void _setCompReacK(uint cidx, uint ridx, double kf) = 0;

    virtual bool _getCompReacActive(uint cidx, uint ridx) const = 0;
    virtual void _setCompReacActive(uint cidx, uint ridx, bool a) = 0;

    virtual double _getCompDiffD(uint cidx, uint didx) const;
    virtual void _setCompDiffD(uint cidx, uint didx, double dcst);

    virtual bool _getCompDiffActive(uint cidx, uint didx) const;
    virtual void _setCompDiffActive(uint cidx, uint didx, bool act);

    ////////////////////////////////////////////////////////////////////////

    virtual double _getCompReacH(uint cidx, uint ridx) const;
    virtual double _getCompReacC(uint cidx, uint ridx) const;
    virtual long double _getCompReacA(uint cidx, uint ridx) const;

    virtual unsigned long long _getCompReacExtent(uint cidx, uint ridx) const;
    virtual void _resetCompReacExtent(uint cidx, uint ridx);

    ////////////////////////////////////////////////////////////////////////
    // SOLVER CONTROL:
    //      TETRAHEDRAL VOLUME ELEMENTS
    ////////////////////////////////////////////////////////////////////////

    virtual double _getTetVol(tetrahedron_id_t tidx) const;
    virtual void _setTetVol(tetrahedron_id_t tidx, double vol);

    virtual bool _getTetSpecDefined(tetrahedron_id_t tidx, uint sidx) const;

    virtual double _getTetCount(tetrahedron_id_t tidx, uint sidx) const;
    virtual void _setTetCount(tetrahedron_id_t tidx, uint sidx, double n);

    virtual double _getTetAmount(tetrahedron_id_t tidx, uint sidx) const;
    virtual void _setTetAmount(tetrahedron_id_t tidx, uint sidx, double m);

    virtual double _getTetConc(tetrahedron_id_t tidx, uint sidx) const;
    virtual void _setTetConc(tetrahedron_id_t tidx, uint sidx, double c);

    virtual bool _getTetClamped(tetrahedron_id_t tidx, uint sidx) const;
    virtual void _setTetClamped(tetrahedron_id_t tidx, uint sidx, bool buf);

    virtual double _getTetReacK(tetrahedron_id_t tidx, uint ridx) const;
    virtual void _setTetReacK(tetrahedron_id_t tidx, uint ridx, double kf);

    virtual bool _getTetReacActive(tetrahedron_id_t tidx, uint ridx) const;
    virtual void _setTetReacActive(tetrahedron_id_t tidx, uint ridx, bool act);

    virtual double _getTetDiffD(tetrahedron_id_t tidx, uint didx, tetrahedron_id_t direction_tet = boost::none) const;
    virtual void _setTetDiffD(tetrahedron_id_t tidx, uint didx, double dk, tetrahedron_id_t direction_tet = boost::none);

    virtual bool _getTetDiffActive(tetrahedron_id_t tidx, uint didx) const;
    virtual void _setTetDiffActive(tetrahedron_id_t tidx, uint didx, bool act);

    ////////////////////////////////////////////////////////////////////////

    virtual double _getTetReacH(tetrahedron_id_t tidx, uint ridx) const;
    virtual double _getTetReacC(tetrahedron_id_t tidx, uint ridx) const;
    virtual double _getTetReacA(tetrahedron_id_t tidx, uint ridx) const;

    virtual double _getTetDiffA(tetrahedron_id_t tidx, uint didx) const;

    ////////////////////////// ADDED FOR EFIELD ////////////////////////////

    virtual double _getTetV(tetrahedron_id_t tidx) const;
    virtual void _setTetV(tetrahedron_id_t tidx, double v);
    virtual bool _getTetVClamped(tetrahedron_id_t tidx) const;
    virtual void _setTetVClamped(tetrahedron_id_t tidx, bool cl);

    ////////////////////////////////////////////////////////////////////////
    // SOLVER CONTROL:
    //      PATCH
    ////////////////////////////////////////////////////////////////////////

    virtual double _getPatchArea(uint pidx) const = 0;
    virtual void _setPatchArea(uint pidx, double area);

    virtual double _getPatchCount(uint pidx, uint sidx) const = 0;
    virtual void _setPatchCount(uint pidx, uint sidx, double n) = 0;

    virtual double _getPatchAmount(uint pidx, uint sidx) const = 0;
    virtual void _setPatchAmount(uint pidx, uint sidx, double a) = 0;

    virtual bool _getPatchClamped(uint pidx, uint sidx) const = 0;
    virtual void _setPatchClamped(uint pidx, uint sidx, bool buf) = 0;

    virtual double _getPatchSReacK(uint pidx, uint ridx) const = 0;
    virtual void _setPatchSReacK(uint pidx, uint ridx, double kf) = 0;

    virtual bool _getPatchSReacActive(uint pidx, uint ridx) const = 0;
    virtual void _setPatchSReacActive(uint pidx, uint ridx, bool a) = 0;

    virtual bool _getPatchVDepSReacActive(uint pidx, uint vsridx) const;
    virtual void _setPatchVDepSReacActive(uint pidx, uint vsridx, bool a);

    ////////////////////////////////////////////////////////////////////////

    virtual double _getPatchSReacH(uint pidx, uint ridx) const;
    virtual double _getPatchSReacC(uint pidx, uint ridx) const;
    virtual double _getPatchSReacA(uint pidx, uint ridx) const;

    virtual unsigned long long _getPatchSReacExtent(uint pidx, uint ridx) const;
    virtual void _resetPatchSReacExtent(uint pidx, uint ridx);

    ////////////////////////////////////////////////////////////////////////
    // SOLVER STATE ACCESS:
    //      DIFFUSION BOUNDARIES
    ////////////////////////////////////////////////////////////////////////

    virtual void _setDiffBoundaryDiffusionActive(uint dbidx, uint sidx, bool act);
    virtual bool _getDiffBoundaryDiffusionActive(uint dbidx, uint sidx) const;
    virtual void _setDiffBoundaryDcst(uint dbidx, uint sidx, double dcst, uint direction_comp = std::numeric_limits<uint>::max());

    ////////////////////////////////////////////////////////////////////////
    // SOLVER STATE ACCESS:
    //      SURFACE DIFFUSION BOUNDARIES
    ////////////////////////////////////////////////////////////////////////

    virtual void _setSDiffBoundaryDiffusionActive(uint sdbidx, uint sidx, bool act);
    virtual bool _getSDiffBoundaryDiffusionActive(uint sdbidx, uint sidx) const;
    virtual void _setSDiffBoundaryDcst(uint sdbidx, uint sidx, double dcst, uint direction_patch = std::numeric_limits<uint>::max());

    ////////////////////////////////////////////////////////////////////////
    // SOLVER CONTROL:
    //      TRIANGULAR SURFACE ELEMENTS
    ////////////////////////////////////////////////////////////////////////

    virtual double _getTriArea(triangle_id_t tidx) const;
    virtual void _setTriArea(triangle_id_t tidx, double area);

    virtual bool _getTriSpecDefined(triangle_id_t tidx, uint sidx) const;

    virtual double _getTriCount(triangle_id_t tidx, uint sidx) const;
    virtual void _setTriCount(triangle_id_t tidx, uint sidx, double n);

    virtual double _getTriAmount(triangle_id_t tidx, uint sidx) const;
    virtual void _setTriAmount(triangle_id_t tidx, uint sidx, double m);

    virtual bool _getTriClamped(triangle_id_t tidx, uint sidx) const;
    virtual void _setTriClamped(triangle_id_t tidx, uint sidx, bool buf);

    virtual double _getTriSReacK(triangle_id_t tidx, uint ridx) const;
    virtual void _setTriSReacK(triangle_id_t tidx, uint ridx, double kf);

    virtual double _getTriSDiffD(triangle_id_t tidx, uint didx, triangle_id_t direction_tri) const;

    virtual void _setTriSDiffD(triangle_id_t , uint , double , triangle_id_t);

    virtual bool _getTriSReacActive(triangle_id_t tidx, uint ridx) const;
    virtual void _setTriSReacActive(triangle_id_t tidx, uint ridx, bool act);

    ////////////////////////////////////////////////////////////////////////

    virtual double _getTriSReacH(triangle_id_t tidx, uint ridx) const;
    virtual double _getTriSReacC(triangle_id_t tidx, uint ridx) const;
    virtual double _getTriSReacA(triangle_id_t tidx, uint ridx) const;

    ////////////////////////// ADDED FOR EFIELD ////////////////////////////

    virtual double _getTriV(triangle_id_t tidx) const;
    virtual void _setTriV(triangle_id_t tidx, double v);
    virtual bool _getTriVClamped(triangle_id_t tidx) const;
    virtual void _setTriVClamped(triangle_id_t tidx, bool cl);
    virtual double _getTriOhmicI(triangle_id_t tidx) const;
    virtual double _getTriOhmicI(triangle_id_t tidx, uint ocidx) const;
    virtual double _getTriI(triangle_id_t tidx) const;

    virtual double _getTriIClamp(triangle_id_t tidx) const;
    virtual void _setTriIClamp(triangle_id_t tidx, double i);

    virtual double _getTriGHKI(triangle_id_t tidx) const;
    virtual double _getTriGHKI(triangle_id_t tidx, uint ocidx) const;

    virtual bool _getTriVDepSReacActive(triangle_id_t tidx, uint vsridx) const;
    virtual void _setTriVDepSReacActive(triangle_id_t tidx, uint vsridx, bool act);

    virtual void _setTriCapac(triangle_id_t tidx, double cm);


    ////////////////////////////////////////////////////////////////////////
    // SOLVER CONTROL:
    //      VERTICES ELEMENTS
    ////////////////////////////////////////////////////////////////////////

    virtual double _getVertV(vertex_id_t vidx) const;
    virtual void _setVertV(vertex_id_t vidx, double v);

    virtual bool _getVertVClamped(vertex_id_t vidx) const;
    virtual void _setVertVClamped(vertex_id_t vidx, bool cl);

    virtual double _getVertIClamp(vertex_id_t vidx) const;
    virtual void _setVertIClamp(vertex_id_t vidx, double i);

    ////////////////////////////////////////////////////////////////////////
    // SOLVER CONTROL:
    //      MEMBRANES
    ////////////////////////////////////////////////////////////////////////

    virtual void _setMembPotential(uint  midx, double v);
    virtual void _setMembCapac(uint midx, double cm);
    virtual void _setMembVolRes(uint midx, double ro);
    virtual void _setMembRes(uint midx, double ro, double vrev);

    ////////////////////////////////////////////////////////////////////////

public:
    /// Return a reference of the Model object.
    inline steps::model::Model * model() const noexcept
    { return pModel; }

    /// Return a reference of the Geom object.
    inline steps::wm::Geom * geom() const noexcept
    { return pGeom; }

    /// Return a reference of the RNG object
    inline const steps::rng::RNGptr& rng() const noexcept
    { return pRNG; }

    /// Return a reference of the Statedef object.
    inline const steps::solver::Statedef& statedef() const noexcept
    { return *pStatedef; }

    /// Return a reference of the Statedef object.
    inline steps::solver::Statedef& statedef() noexcept
    { return *pStatedef; }


  ////////////////////////////////////////////////////////////////////////

private:

    ////////////////////////////////////////////////////////////////////////

    steps::model::Model *               pModel;

    steps::wm::Geom *                   pGeom;

    const steps::rng::RNGptr            pRNG;

    Statedef *                          pStatedef;

    ////////////////////////////////////////////////////////////////////////

};

////////////////////////////////////////////////////////////////////////////////

} // namespace solver
} // namespace steps

#endif
// STEPS_SOLVER_API_HPP

// END
