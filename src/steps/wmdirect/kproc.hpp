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


#ifndef STEPS_WMDIRECT_KPROC_HPP
#define STEPS_WMDIRECT_KPROC_HPP 1


// STL headers.
#include <vector>
#include <fstream>

// STEPS headers.
#include "util/common.h"
#include "solver/types.hpp"
#include "solver/reacdef.hpp"
#include "solver/sreacdef.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace steps{
namespace wmdirect{

//Forward declaration
class Comp;
class Patch;
class KProc;

////////////////////////////////////////////////////////////////////////////////

typedef steps::wmdirect::KProc *        KProcP;
typedef std::vector<KProcP>             KProcPVec;
typedef KProcPVec::iterator             KProcPVecI;
typedef KProcPVec::const_iterator       KProcPVecCI;

////////////////////////////////////////////////////////////////////////////////

class KProc

{
public:

    ////////////////////////////////////////////////////////////////////////
    // OBJECT CONSTRUCTION & DESTRUCTION
    ////////////////////////////////////////////////////////////////////////

    KProc();
    virtual ~KProc();

    ////////////////////////////////////////////////////////////////////////
    // CHECKPOINTING
    ////////////////////////////////////////////////////////////////////////
    /// checkpoint data
    virtual void checkpoint(std::fstream & cp_file) = 0;

    /// restore data
    virtual void restore(std::fstream & cp_file) = 0;

    ////////////////////////////////////////////////////////////////////////
    // DATA ACCESS
    ////////////////////////////////////////////////////////////////////////

    /*
    static const int INACTIVATED = 1;

    inline bool active() const
    { return !(pFlags & INACTIVATED); }
    inline bool inactive() const
    { return (pFlags & INACTIVATED); }
    void setActive(bool active);

    inline uint flags() const
    { return pFlags; }
    */
    ////////////////////////////////////////////////////////////////////////

    uint schedIDX() const
    { return pSchedIDX; }

    void setSchedIDX(uint idx)
    { pSchedIDX = idx; }

    ////////////////////////////////////////////////////////////////////////
    // VIRTUAL INTERFACE METHODS
    ////////////////////////////////////////////////////////////////////////

    /// This function is called when all kproc objects have been created,
    /// allowing the kproc to pre-compute its SchedIDXVec.
    ///
    virtual void setupDeps() = 0;

    virtual bool depSpecComp(uint gidx, Comp * comp) = 0;
    virtual bool depSpecPatch(uint gidx, Patch * patch) = 0;

    /// Reset this Kproc.
    ///
    virtual void reset() = 0;

    // Recompute the Ccst for this KProc
    virtual void resetCcst() = 0;

    /// Compute the rate for this kproc (its propensity value).
    ///
    virtual double rate() const = 0;

    // Return the ccst for this kproc
    virtual double c() const = 0;

    // Return the h value for this kproc (number of available reaction channels)
    virtual double h() const = 0;

    /// Apply a single discrete instance of the kinetic process, returning
    /// a vector of kproc schedule indices that need to be updated as a
    /// result.
    ///
    virtual std::vector<uint> const & apply() = 0;

    virtual uint updVecSize() const = 0;

    ////////////////////////////////////////////////////////////////////////

    // Return a pointer to the corresponding Reacdef or SReacdef object
    // Separate methods to avoid makeing a base KProcdef class
    //
    virtual steps::solver::Reacdef * defr() const;
    virtual steps::solver::SReacdef * defsr() const;

    ////////////////////////////////////////////////////////////////////////

    inline unsigned long long getExtent() const noexcept {
        return rExtent;
    }

    inline void resetExtent() {
        rExtent = 0;
    }



protected:

    unsigned long long                  rExtent{0};

private:

    ////////////////////////////////////////////////////////////////////////
    /*
    uint                                pFlags;
    */
    uint                                pSchedIDX{0};

    ////////////////////////////////////////////////////////////////////////
};

////////////////////////////////////////////////////////////////////////////////

}
}

#endif
// STEPS_WMDIRECT_KPROC_HPP

// END

