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
#include <algorithm>
#include <cassert>
#include <cmath>
#include <functional>
#include <iostream>

// STEPS headers.
#include "tri.hpp"
#include "ghkcurr.hpp"
#include "sdiff.hpp"
#include "sreac.hpp"
#include "tet.hpp"
#include "tetexact.hpp"
#include "vdepsreac.hpp"
#include "vdeptrans.hpp"
#include "math/constants.hpp"
#include "solver/ohmiccurrdef.hpp"
#include "solver/sreacdef.hpp"

// logging
#include <easylogging++.h>
#include "util/error.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace stex = steps::tetexact;
namespace ssolver = steps::solver;

////////////////////////////////////////////////////////////////////////////////

stex::Tri::Tri(triangle_id_t idx, steps::solver::Patchdef * patchdef, double area,
                double l0, double l1, double l2, double d0, double d1, double d2,
                tetrahedron_id_t tetinner, tetrahedron_id_t tetouter,
                triangle_id_t tri0, triangle_id_t tri1, triangle_id_t tri2)
: pIdx(idx)
, pPatchdef(patchdef)
, pInnerTet(nullptr)
, pOuterTet(nullptr)
, pTets()
, pNextTri()
, pArea(area)
, pLengths()
, pDist()
, pPoolCount(nullptr)
, pPoolFlags(nullptr)
, pKProcs()
, pECharge(nullptr)
, pECharge_last(nullptr)
, pECharge_accum(nullptr)
, pECharge_last_dt(0)
, pECharge_accum_dt(0)
, pOCchan_timeintg(nullptr)
, pOCtime_upd(nullptr)
{
    AssertLog(pPatchdef != nullptr);
    AssertLog(pArea > 0.0);

    AssertLog(l0 > 0.0 && l1 > 0.0 && l2 > 0.0);
    AssertLog(d0 >= 0.0 && d1 >= 0.0 && d2 >= 0.0);

    pTets[0] = tetinner;
    pTets[1] = tetouter;

    pTris[0] = tri0;
    pTris[1] = tri1;
    pTris[2] = tri2;

    pNextTri[0] = nullptr;
    pNextTri[1] = nullptr;
    pNextTri[2] = nullptr;

    pLengths[0] = l0;
    pLengths[1] = l1;
    pLengths[2] = l2;

    pDist[0] = d0;
    pDist[1] = d1;
    pDist[2] = d2;

    uint nspecs = pPatchdef->countSpecs();
    pPoolCount = new uint[nspecs];
    pPoolFlags = new uint[nspecs];
    std::fill_n(pPoolCount, nspecs, 0);
    std::fill_n(pPoolFlags, nspecs, 0);

    uint nghkcurrs = pPatchdef->countGHKcurrs();
    pECharge = new int[nghkcurrs];
    std::fill_n(pECharge, nghkcurrs, 0);

    pECharge_last = new int[nghkcurrs];
    std::fill_n(pECharge_last, nghkcurrs, 0);

    pECharge_accum = new int[nghkcurrs];
    std::fill_n(pECharge_accum, nghkcurrs, 0);

    uint nohmcurrs = pPatchdef->countOhmicCurrs();
    pOCchan_timeintg = new double[nohmcurrs];
    std::fill_n(pOCchan_timeintg, nohmcurrs, 0.0);
    pOCtime_upd = new double[nohmcurrs];
    std::fill_n(pOCtime_upd, nohmcurrs, 0.0);

    std::fill_n(pSDiffBndDirection, 3, false);
}

////////////////////////////////////////////////////////////////////////////////

stex::Tri::~Tri()
{
    delete[] pPoolCount;
    delete[] pPoolFlags;
    delete[] pECharge;
    delete[] pECharge_last;
    delete[] pECharge_accum;
    delete[] pOCchan_timeintg;
    delete[] pOCtime_upd;

    KProcPVecCI e = pKProcs.end();
    for (std::vector<stex::KProc *>::const_iterator i = pKProcs.begin();
         i != e; ++i) delete *i;
}

////////////////////////////////////////////////////////////////////////////////

void stex::Tri::checkpoint(std::fstream & cp_file)
{
    uint nspecs = patchdef()->countSpecs();
    cp_file.write(reinterpret_cast<char*>(pPoolCount), sizeof(uint) * nspecs);
    cp_file.write(reinterpret_cast<char*>(pPoolFlags), sizeof(uint) * nspecs);

    uint nghkcurrs = pPatchdef->countGHKcurrs();
    cp_file.write(reinterpret_cast<char*>(pECharge), sizeof(int) * nghkcurrs);
    cp_file.write(reinterpret_cast<char*>(pECharge_last), sizeof(int) * nghkcurrs);
    cp_file.write(reinterpret_cast<char*>(pECharge_accum), sizeof(int) * nghkcurrs);
    cp_file.write(reinterpret_cast<char*>(&pECharge_last_dt), sizeof(double));
    cp_file.write(reinterpret_cast<char*>(&pECharge_accum_dt), sizeof(double));

    uint nohmcurrs = pPatchdef->countOhmicCurrs();
    cp_file.write(reinterpret_cast<char*>(pOCchan_timeintg), sizeof(double) * nohmcurrs);
    cp_file.write(reinterpret_cast<char*>(pOCtime_upd), sizeof(double) * nohmcurrs);

    cp_file.write(reinterpret_cast<char*>(pSDiffBndDirection), sizeof(bool) * 3);

}

////////////////////////////////////////////////////////////////////////////////

void stex::Tri::restore(std::fstream & cp_file)
{
    uint nspecs = patchdef()->countSpecs();
    cp_file.read(reinterpret_cast<char*>(pPoolCount), sizeof(uint) * nspecs);
    cp_file.read(reinterpret_cast<char*>(pPoolFlags), sizeof(uint) * nspecs);

    uint nghkcurrs = pPatchdef->countGHKcurrs();
    cp_file.read(reinterpret_cast<char*>(pECharge), sizeof(int) * nghkcurrs);
    cp_file.read(reinterpret_cast<char*>(pECharge_last), sizeof(int) * nghkcurrs);
    cp_file.read(reinterpret_cast<char*>(pECharge_accum), sizeof(int) * nghkcurrs);
    cp_file.read(reinterpret_cast<char*>(&pECharge_last_dt), sizeof(double));
    cp_file.read(reinterpret_cast<char*>(&pECharge_accum_dt), sizeof(double));

    uint nohmcurrs = pPatchdef->countOhmicCurrs();
    cp_file.read(reinterpret_cast<char*>(pOCchan_timeintg), sizeof(double) * nohmcurrs);
    cp_file.read(reinterpret_cast<char*>(pOCtime_upd), sizeof(double) * nohmcurrs);

    cp_file.read(reinterpret_cast<char*>(pSDiffBndDirection), sizeof(bool) * 3);

}

////////////////////////////////////////////////////////////////////////////////

void stex::Tri::setInnerTet(stex::WmVol * t)
{
    pInnerTet = t;
}

////////////////////////////////////////////////////////////////////////////////

void stex::Tri::setOuterTet(stex::WmVol * t)
{
    pOuterTet = t;
}

////////////////////////////////////////////////////////////////////////////////

void stex::Tri::setSDiffBndDirection(uint i)
{
    AssertLog(i < 3);

    pSDiffBndDirection[i] = true;
}

////////////////////////////////////////////////////////////////////////////////

void stex::Tri::setNextTri(uint i, stex::Tri * t)
{
    AssertLog(i <= 2);

    pNextTri[i]= t;
}

////////////////////////////////////////////////////////////////////////////////

void stex::Tri::setupKProcs(stex::Tetexact * tex, bool efield)
{
    uint kprocvecsize = pPatchdef->countSReacs()+pPatchdef->countSurfDiffs();
    if (efield) {
        kprocvecsize += (pPatchdef->countVDepTrans() + pPatchdef->countVDepSReacs() + pPatchdef->countGHKcurrs());
}
    pKProcs.resize(kprocvecsize);

    uint j = 0;
    // Create surface reaction kprocs
    uint nsreacs = patchdef()->countSReacs();
    for (uint i=0; i < nsreacs; ++i)
    {
        ssolver::SReacdef * srdef = patchdef()->sreacdef(i);
        auto * sr = new SReac(srdef, this);
        AssertLog(sr != nullptr);
        pKProcs[j++] = sr;
        tex->addKProc(sr);
    }

    uint nsdiffs = patchdef()->countSurfDiffs();
    for (uint i=0; i < nsdiffs; ++i)
    {
        ssolver::Diffdef * sddef = patchdef()->surfdiffdef(i);
        auto * sd = new SDiff(sddef, this);
        AssertLog(sd != nullptr);
        pKProcs[j++] = sd;
        tex->addKProc(sd);
    }


    if (efield)
    {
        uint nvdtrans = patchdef()->countVDepTrans();
        for (uint i=0; i < nvdtrans; ++i)
        {
            ssolver::VDepTransdef * vdtdef = patchdef()->vdeptransdef(i);
            auto * vdt = new VDepTrans(vdtdef, this);
            AssertLog(vdt != nullptr);
            pKProcs[j++] = vdt;
            tex->addKProc(vdt);
        }

        uint nvdsreacs = patchdef()->countVDepSReacs();
        for (uint i=0; i < nvdsreacs; ++i)
        {
            ssolver::VDepSReacdef * vdsrdef = patchdef()->vdepsreacdef(i);
            auto * vdsr = new VDepSReac(vdsrdef, this);
            AssertLog(vdsr != nullptr);
            pKProcs[j++] = vdsr;
            tex->addKProc(vdsr);
        }

        uint nghkcurrs = patchdef()->countGHKcurrs();
        for (uint i=0; i < nghkcurrs; ++i)
        {
            ssolver::GHKcurrdef * ghkdef = patchdef()->ghkcurrdef(i);
            auto * ghk = new GHKcurr(ghkdef, this);
            AssertLog(ghk != nullptr);
            pKProcs[j++] = ghk;
            tex->addKProc(ghk);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void stex::Tri::reset()
{
    uint nspecs = patchdef()->countSpecs();
    std::fill_n(pPoolCount, nspecs, 0);
    std::fill_n(pPoolFlags, nspecs, 0);

    for (auto kproc: pKProcs) {
        kproc->reset();
    }

    uint nghkcurrs = pPatchdef->countGHKcurrs();
    std::fill_n(pECharge, nghkcurrs, 0);
    std::fill_n(pECharge_last, nghkcurrs, 0);
    std::fill_n(pECharge_accum, nghkcurrs, 0);
    pECharge_last_dt = 0;
    pECharge_accum_dt = 0;

    uint nohmcurrs = pPatchdef->countOhmicCurrs();
    std::fill_n(pOCchan_timeintg, nohmcurrs, 0.0);
    std::fill_n(pOCtime_upd, nohmcurrs, 0.0);
}

////////////////////////////////////////////////////////////////////////////////

void stex::Tri::resetECharge(double dt, double efdt, double t)
{
    uint nghkcurrs = pPatchdef->countGHKcurrs();

    for (uint i=0; i < nghkcurrs; ++i) {
        pECharge_accum[i] += pECharge[i];
    }
    pECharge_accum_dt += dt;

    if (pECharge_accum_dt >= efdt or
        (efdt - pECharge_accum_dt) <=
            std::numeric_limits<double>::epsilon() * t * 8) {
        // Swap arrays
        std::swap(pECharge_last, pECharge_accum);

        // reset accumulation array and dt values
        std::fill_n(pECharge_accum, nghkcurrs, 0);

        pECharge_last_dt = pECharge_accum_dt;
        pECharge_accum_dt = 0;
    }

    std::fill_n(pECharge, nghkcurrs, 0);
}

////////////////////////////////////////////////////////////////////////////////

void stex::Tri::resetOCintegrals()
{
    uint nocs = patchdef()->countOhmicCurrs();
    std::fill_n(pOCchan_timeintg, nocs, 0.0);
}

////////////////////////////////////////////////////////////////////////////////

void stex::Tri::incECharge(uint lidx, int charge)
{
    uint nghkcurrs = pPatchdef->countGHKcurrs();
    AssertLog(lidx < nghkcurrs);
    pECharge[lidx]+=charge;
}

////////////////////////////////////////////////////////////////////////////////

void stex::Tri::setCount(uint lidx, uint count)
{
    AssertLog(lidx < patchdef()->countSpecs());
    pPoolCount[lidx] = count;

    /* 16/01/10 IH: Counts no longer stored in patch object.
    // Now update the count in this tri's patch
    double diff = c - oldcount;
    double newcount = (patchdef()->pools()[lidx]) + diff;
    // Patchdef method will do the checking on the double argument (should be positive!)
    patchdef()->setCount(lidx, newcount);
    */
}

////////////////////////////////////////////////////////////////////////////////

void stex::Tri::incCount(uint lidx, int inc)
{
    AssertLog(lidx < patchdef()->countSpecs());
    pPoolCount[lidx] += inc;
}

////////////////////////////////////////////////////////////////////////////////

void stex::Tri::setOCchange(uint oclidx, uint slidx, double dt, double simtime)
{
    // NOTE: simtime is BEFORE the update has taken place

    AssertLog(oclidx < patchdef()->countOhmicCurrs());
    AssertLog(slidx < patchdef()->countSpecs());

    // A channel state relating to an ohmic current has changed it's
    // number.
    double integral = pPoolCount[slidx]*((simtime+dt) - pOCtime_upd[oclidx]);
    AssertLog(integral >= 0.0);

    pOCchan_timeintg[oclidx] += integral;
    pOCtime_upd[oclidx] = simtime+dt;
}

////////////////////////////////////////////////////////////////////////////////

void stex::Tri::setClamped(uint lidx, bool clamp)
{
    if (clamp) { pPoolFlags[lidx] |= CLAMPED;
    } else { pPoolFlags[lidx] &= ~CLAMPED;
}
}

////////////////////////////////////////////////////////////////////////////////

stex::SReac * stex::Tri::sreac(uint lidx) const
{
    AssertLog(lidx < patchdef()->countSReacs());
    return dynamic_cast<stex::SReac*>(pKProcs[lidx]);
}

////////////////////////////////////////////////////////////////////////////////
int stex::Tri::getTriDirection(triangle_id_t tidx)
{
    for (uint i = 0; i < 3; i++) {
        if (pTris[i] == tidx) {
            return i;
        }
    }
    return -1;
}

////////////////////////////////////////////////////////////////////////////////

stex::SDiff * stex::Tri::sdiff(uint lidx) const
{
    AssertLog(lidx < patchdef()->countSurfDiffs());
    return dynamic_cast<stex::SDiff*>(pKProcs[patchdef()->countSReacs() + lidx]);
}

////////////////////////////////////////////////////////////////////////////////

stex::VDepTrans * stex::Tri::vdeptrans(uint lidx) const
{
    AssertLog(lidx < patchdef()->countVDepTrans());
    return dynamic_cast<stex::VDepTrans*>(pKProcs[patchdef()->countSReacs() + patchdef()->countSurfDiffs() + lidx]);
}

////////////////////////////////////////////////////////////////////////////////

stex::VDepSReac * stex::Tri::vdepsreac(uint lidx) const
{
    AssertLog(lidx < patchdef()->countVDepSReacs());
    return dynamic_cast<stex::VDepSReac*>(pKProcs[patchdef()->countSReacs() + patchdef()->countSurfDiffs() + patchdef()->countVDepTrans() + lidx]);

}

////////////////////////////////////////////////////////////////////////////////

stex::GHKcurr * stex::Tri::ghkcurr(uint lidx) const
{
    AssertLog(lidx < patchdef()->countGHKcurrs());
    return dynamic_cast<stex::GHKcurr*>(pKProcs[patchdef()->countSReacs() + patchdef()->countSurfDiffs() + patchdef()->countVDepTrans() + patchdef()->countVDepSReacs() + lidx]);

}

////////////////////////////////////////////////////////////////////////////////

double stex::Tri::getGHKI(uint lidx) const
{
    if (pECharge_last_dt == 0)
        return 0;

    uint nghkcurrs = pPatchdef->countGHKcurrs();
    AssertLog(lidx < nghkcurrs);

    int efcharge = pECharge_last[lidx];
    auto efcharged = static_cast<double>(efcharge);

    return ((efcharged*steps::math::E_CHARGE)/pECharge_last_dt);
}

////////////////////////////////////////////////////////////////////////////////

double stex::Tri::getGHKI() const
{
    if (pECharge_last_dt == 0)
        return 0;

    uint nghkcurrs = pPatchdef->countGHKcurrs();
    int efcharge=0;
    for (uint i =0; i < nghkcurrs; ++i)
    {
        efcharge += pECharge_last[i];
    }

    auto efcharged = static_cast<double>(efcharge);

    return ((efcharged*steps::math::E_CHARGE)/pECharge_last_dt);
}

////////////////////////////////////////////////////////////////////////////////

double stex::Tri::computeI(double v, double dt, double simtime, double efdt)
{
    double current = 0.0;
    uint nocs = patchdef()->countOhmicCurrs();
    for (uint i = 0; i < nocs; ++i)
    {
        ssolver::OhmicCurrdef * ocdef = patchdef()->ohmiccurrdef(i);
        // First calculate the last little bit up to the simtime
        double integral = pPoolCount[patchdef()->ohmiccurr_chanstate(i)]*(simtime - pOCtime_upd[i]);
        AssertLog(integral >= 0.0);
        pOCchan_timeintg[i] += integral;
        pOCtime_upd[i] = simtime;

        // Find the mean number of channels open over the dt
        double n = pOCchan_timeintg[i]/dt;
        current += (n*ocdef->getG())*(v-ocdef->getERev());
    }
    uint nghkcurrs = pPatchdef->countGHKcurrs();
    int efcharge=0;
    for (uint i =0; i < nghkcurrs; ++i)
    {
        efcharge += pECharge[i];
    }

    // The contribution from GHK charge movement.
    auto efcharged = static_cast<double>(efcharge);

    // Convert charge to coulombs and find mean current
    current += ((efcharged*steps::math::E_CHARGE)/dt);
    resetECharge(dt, efdt, simtime);
    resetOCintegrals();

    return current;
}

////////////////////////////////////////////////////////////////////////////////

double stex::Tri::getOhmicI(double v,double /*dt*/) const
{
    double current = 0.0;
    uint nocs = patchdef()->countOhmicCurrs();
    for (uint i = 0; i < nocs; ++i)
    {
        ssolver::OhmicCurrdef * ocdef = patchdef()->ohmiccurrdef(i);
        // The next is ok because Patchdef returns local index
        uint n = pPoolCount[patchdef()->ohmiccurr_chanstate(i)];
        current += (n*ocdef->getG())*(v-ocdef->getERev());
    }

    return current;
}

////////////////////////////////////////////////////////////////////////////////

double stex::Tri::getOhmicI(uint lidx, double v,double /*dt*/) const
{
    AssertLog(lidx < patchdef()->countOhmicCurrs());
    ssolver::OhmicCurrdef * ocdef = patchdef()->ohmiccurrdef(lidx);
    uint n = pPoolCount[patchdef()->ohmiccurr_chanstate(lidx)];

    return (n*ocdef->getG())*(v-ocdef->getERev());
}

////////////////////////////////////////////////////////////////////////////////

//END
