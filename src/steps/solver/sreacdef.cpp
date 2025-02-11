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

// STL headers.
#include <cassert>
#include <sstream>
#include <string>

// STEPS headers.
#include "sreacdef.hpp"
#include "patchdef.hpp"
#include "statedef.hpp"
#include "types.hpp"
#include "geom/patch.hpp"
#include "model/sreac.hpp"

#include "util/error.hpp"
// logging
#include <easylogging++.h>

namespace ssolver = steps::solver;
namespace smod = steps::model;

////////////////////////////////////////////////////////////////////////////////

ssolver::SReacdef::SReacdef(Statedef * sd, uint idx, steps::model::SReac * sr)
: pStatedef(sd)
, pIdx(idx)
, pName()
, pOrder()
, pKcst()
, pIlhs()
, pOlhs()
, pSlhs()
, pIrhs()
, pOrhs()
, pSrhs()
, pSetupdone(false)
, pSurface_surface(true)
, pSpec_I_DEP(nullptr)
, pSpec_S_DEP(nullptr)
, pSpec_O_DEP(nullptr)
, pSpec_I_LHS(nullptr)
, pSpec_S_LHS(nullptr)
, pSpec_O_LHS(nullptr)
, pSpec_I_RHS(nullptr)
, pSpec_S_RHS(nullptr)
, pSpec_O_RHS(nullptr)
, pSpec_I_UPD(nullptr)
, pSpec_S_UPD(nullptr)
, pSpec_O_UPD(nullptr)
, pSpec_I_UPD_Coll()
, pSpec_S_UPD_Coll()
, pSpec_O_UPD_Coll()
{

    AssertLog(pStatedef != nullptr);
    AssertLog(sr != nullptr);

    pName = sr->getID();
    pOrder = sr->getOrder();

    if (pOrder == 0)
    {
        std::ostringstream os;
        os << "Model contains zero-order surface reaction, which are not permitted. ";
        os << " Zero-order volume reaction may be used instead.";
        ArgErrLog(os.str());
    }

    pKcst = sr->getKcst();
    pIlhs = sr->getILHS();
    pOlhs = sr->getOLHS();
    pSlhs = sr->getSLHS();

    pIrhs = sr->getIRHS();
    pOrhs = sr->getORHS();
    pSrhs = sr->getSRHS();

    if (sr->getInner() == true) { pOrient = SReacdef::INSIDE;
    } else { pOrient = SReacdef::OUTSIDE;
}

    uint nspecs = pStatedef->countSpecs();
    if (nspecs == 0) { return; // Would be weird, but okay.
}
    pSpec_S_DEP = new int[nspecs];
    std::fill_n(pSpec_S_DEP, nspecs, DEP_NONE);
    pSpec_S_LHS = new uint[nspecs];
    std::fill_n(pSpec_S_LHS, nspecs, 0);
    if (pOrient == SReacdef::INSIDE)
    {
        pSpec_I_DEP = new int[nspecs];
        std::fill_n(pSpec_I_DEP, nspecs, DEP_NONE);
        pSpec_I_LHS = new uint[nspecs];
        std::fill_n(pSpec_I_LHS, nspecs, 0);
    }
    else
    {
        pSpec_O_DEP = new depT[nspecs];
        std::fill_n(pSpec_O_DEP, nspecs, DEP_NONE);
        pSpec_O_LHS = new uint[nspecs];
        std::fill_n(pSpec_O_LHS, nspecs, 0);
    }
    pSpec_I_RHS = new uint[nspecs];
    std::fill_n(pSpec_I_RHS, nspecs, 0);
    pSpec_S_RHS = new uint[nspecs];
    std::fill_n(pSpec_S_RHS, nspecs, 0);
    pSpec_O_RHS = new uint[nspecs];
    std::fill_n(pSpec_O_RHS, nspecs, 0);
    pSpec_I_UPD = new int[nspecs];
    std::fill_n(pSpec_I_UPD, nspecs, 0);
    pSpec_S_UPD = new int[nspecs];
    std::fill_n(pSpec_S_UPD, nspecs, 0);
    pSpec_O_UPD = new int[nspecs];
    std::fill_n(pSpec_O_UPD, nspecs, 0);

}

////////////////////////////////////////////////////////////////////////////////

ssolver::SReacdef::~SReacdef()
{
    if (pStatedef->countSpecs() > 0)
    {
        if (pOrient == SReacdef::INSIDE)
        {
            delete[] pSpec_I_DEP;
            delete[] pSpec_I_LHS;
        }
        else
        {
            delete[] pSpec_O_DEP;
            delete[] pSpec_O_LHS;

        }
        delete[] pSpec_S_DEP;
        delete[] pSpec_S_LHS;
        delete[] pSpec_I_RHS;
        delete[] pSpec_S_RHS;
        delete[] pSpec_O_RHS;
        delete[] pSpec_I_UPD;
        delete[] pSpec_S_UPD;
        delete[] pSpec_O_UPD;
    }
}

////////////////////////////////////////////////////////////////////////////////

void ssolver::SReacdef::checkpoint(std::fstream & /*cp_file*/)
{
    // reserve
}

////////////////////////////////////////////////////////////////////////////////

void ssolver::SReacdef::restore(std::fstream & /*cp_file*/)
{
    // reserve
}

////////////////////////////////////////////////////////////////////////////////

std::string const ssolver::SReacdef::name() const
{
    return pName;
}

////////////////////////////////////////////////////////////////////////////////

uint ssolver::SReacdef::order() const
{
    return pOrder;
}

////////////////////////////////////////////////////////////////////////////////

double ssolver::SReacdef::kcst() const
{
    return pKcst;
}

////////////////////////////////////////////////////////////////////////////////

void ssolver::SReacdef::setup()
{
    AssertLog(pSetupdone == false);


    if (outside()) { AssertLog(pIlhs.empty()); }
    else if (inside()) { AssertLog(pOlhs.empty()); }
    else AssertLog(false);

    for (auto const& ol: pOlhs) {
        pSurface_surface = false;
        uint sidx = pStatedef->getSpecIdx(ol);
        pSpec_O_LHS[sidx] += 1;
    }

    for (auto const& il: pIlhs) {
        pSurface_surface = false;
        uint sidx = pStatedef->getSpecIdx(il);
        pSpec_I_LHS[sidx] += 1;
    }

    for (auto const& sl: pSlhs) {
        uint sidx = pStatedef->getSpecIdx(sl);
        pSpec_S_LHS[sidx] += 1;
    }

    for (auto const& ir: pIrhs) {
        uint sidx = pStatedef->getSpecIdx(ir);
        pSpec_I_RHS[sidx] += 1;
    }

    for (auto const& sr: pSrhs) {
        uint sidx = pStatedef->getSpecIdx(sr);
        pSpec_S_RHS[sidx] += 1;
    }

    for (auto const& orh: pOrhs) {
        uint sidx = pStatedef->getSpecIdx(orh);
        pSpec_O_RHS[sidx] += 1;
    }

    // Now set up the update vector
    uint nspecs = pStatedef->countSpecs();
    // Deal with surface.
    for (uint i = 0; i < nspecs; ++i)
    {
        auto lhs = static_cast<int>(pSpec_S_LHS[i]);
        auto rhs = static_cast<int>(pSpec_S_RHS[i]);
        int aux = pSpec_S_UPD[i] = (rhs - lhs);
        if (lhs != 0) { pSpec_S_DEP[i] |= DEP_STOICH;
}
        if (aux != 0) pSpec_S_UPD_Coll.push_back(i);
    }

    // Deal with inside.
    for (uint i = 0; i < nspecs; ++i)
    {
        int lhs = (inside() ? static_cast<int>(pSpec_I_LHS[i]) : 0);
        auto rhs = static_cast<int>(pSpec_I_RHS[i]);
        int aux = pSpec_I_UPD[i] = (rhs - lhs);
        if (lhs != 0) { pSpec_I_DEP[i] |= DEP_STOICH;
}
        if (aux != 0) pSpec_I_UPD_Coll.push_back(i);
    }

    // Deal with outside.
    for (uint i = 0; i < nspecs; ++i)
    {
        int lhs = (outside() ? static_cast<int>(pSpec_O_LHS[i]) : 0);
        auto rhs = static_cast<int>(pSpec_O_RHS[i]);
        int aux = pSpec_O_UPD[i] = (rhs - lhs);
        if (lhs != 0) { pSpec_O_DEP[i] |= DEP_STOICH;
}
        if (aux != 0) pSpec_O_UPD_Coll.push_back(i);
    }

    pSetupdone = true;
}

////////////////////////////////////////////////////////////////////////////////

bool ssolver::SReacdef::reqInside() const
{
    AssertLog(pSetupdone == true);

    // This can be checked by seeing if DEP_I or RHS_I is non-zero
    // for any species.
    uint nspecs = pStatedef->countSpecs();
    for (uint i = 0; i < nspecs; ++i) { if (reqspec_I(i) == true) { return true;
}
}
    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool ssolver::SReacdef::reqOutside() const
{
    AssertLog(pSetupdone == true);

    // This can be checked by seeing if DEP_O or RHS_O is non-zero
    // for any species.
    uint nspecs = pStatedef->countSpecs();
    for (uint i = 0; i < nspecs; ++i) {
        if (reqspec_O(i) == true) { return true;
}
}
    return false;
}

////////////////////////////////////////////////////////////////////////////////

uint ssolver::SReacdef::lhs_I(uint gidx) const
{
    if (outside()) { return 0;
}
    AssertLog(gidx < pStatedef->countSpecs());
    return pSpec_I_LHS[gidx];
}

////////////////////////////////////////////////////////////////////////////////

uint ssolver::SReacdef::lhs_S(uint gidx) const
{
    AssertLog(gidx < pStatedef->countSpecs());
    return pSpec_S_LHS[gidx];
}

////////////////////////////////////////////////////////////////////////////////

uint ssolver::SReacdef::lhs_O(uint gidx) const
{
    if (inside()) { return 0;
}
    AssertLog(gidx < pStatedef->countSpecs());
    return pSpec_O_LHS[gidx];
}

////////////////////////////////////////////////////////////////////////////////

int ssolver::SReacdef::dep_I(uint gidx) const
{
    AssertLog(pSetupdone == true);
    AssertLog(gidx < pStatedef->countSpecs());
    if (outside()) { return DEP_NONE;
}
    return pSpec_I_DEP[gidx];
}

////////////////////////////////////////////////////////////////////////////////

int ssolver::SReacdef::dep_S(uint gidx) const
{
    AssertLog(pSetupdone == true);
    AssertLog(gidx < pStatedef->countSpecs());
    return pSpec_S_DEP[gidx];
}

////////////////////////////////////////////////////////////////////////////////

int ssolver::SReacdef::dep_O(uint gidx) const
{
    AssertLog(pSetupdone == true);
    AssertLog(gidx < pStatedef->countSpecs());
    if (inside()) { return DEP_NONE;
}
    return pSpec_O_DEP[gidx];
}

////////////////////////////////////////////////////////////////////////////////

uint ssolver::SReacdef::rhs_I(uint gidx) const
{
    AssertLog(gidx < pStatedef->countSpecs());
    return pSpec_I_RHS[gidx];
}

////////////////////////////////////////////////////////////////////////////////

uint ssolver::SReacdef::rhs_S(uint gidx) const
{
    AssertLog(gidx < pStatedef->countSpecs());
    return pSpec_S_RHS[gidx];
}

////////////////////////////////////////////////////////////////////////////////

uint ssolver::SReacdef::rhs_O(uint gidx) const
{
    AssertLog(gidx < pStatedef->countSpecs());
    return pSpec_O_RHS[gidx];
}

////////////////////////////////////////////////////////////////////////////////

int ssolver::SReacdef::upd_I(uint gidx) const
{
    AssertLog(pSetupdone == true);
    AssertLog(gidx < pStatedef->countSpecs());
    return pSpec_I_UPD[gidx];
}

////////////////////////////////////////////////////////////////////////////////

int ssolver::SReacdef::upd_S(uint gidx) const
{
    AssertLog(pSetupdone == true);
    AssertLog(gidx < pStatedef->countSpecs());
    return pSpec_S_UPD[gidx];
}

////////////////////////////////////////////////////////////////////////////////

int ssolver::SReacdef::upd_O(uint gidx) const
{
    AssertLog(pSetupdone == true);
    AssertLog(gidx < pStatedef->countSpecs());
    return pSpec_O_UPD[gidx];
}

////////////////////////////////////////////////////////////////////////////////

bool ssolver::SReacdef::reqspec_I(uint gidx) const
{
    AssertLog(pSetupdone == true);
    AssertLog(gidx < pStatedef->countSpecs());
    if (inside()) {
        if (pSpec_I_DEP[gidx] != DEP_NONE) { return true;
}
}
    if (pSpec_I_RHS[gidx] != 0) { return true;
}
    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool ssolver::SReacdef::reqspec_S(uint gidx) const
{
    AssertLog(pSetupdone == true);
    AssertLog(gidx < pStatedef->countSpecs());
    if (pSpec_S_DEP[gidx] != DEP_NONE) { return true;
}
    if (pSpec_S_RHS[gidx] != 0) { return true;
}
    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool ssolver::SReacdef::reqspec_O(uint gidx) const
{
    AssertLog(pSetupdone == true);
    AssertLog(gidx < pStatedef->countSpecs());
    if (outside()) {
        if (pSpec_O_DEP[gidx] != DEP_NONE) { return true;
}
}
    if (pSpec_O_RHS[gidx] != 0) { return true;
}
    return false;
}

////////////////////////////////////////////////////////////////////////////////

// END
