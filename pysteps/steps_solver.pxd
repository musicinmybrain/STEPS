# -*- coding: utf-8 -*-
# =====================================================================================================================
# These bindings were automatically generated by cyWrap. Please do dot modify.
# Additional functionality shall be implemented in sub-classes.
#
__copyright__ = "Copyright 2016 EPFL BBP-project"
# =====================================================================================================================
from cython.operator cimport dereference as deref
from libcpp cimport bool
cimport std
cimport steps_wm
cimport steps_rng
cimport steps_model
from steps_common cimport *


# ======================================================================================================================
cdef extern from "steps/solver/patchdef.hpp" namespace "steps::solver":
# ----------------------------------------------------------------------------------------------------------------------
    # ctypedef Patchdef* PatchDefP
    # ctypedef std.vector[Patchdef*] PatchDefPVec
    # ctypedef std.vector[Patchdef*].iterator PatchDefPVecI
    # ctypedef std.vector[Patchdef*].const_iterator PatchDefPVecCI

    ###### Cybinding for Patchdef ######
    cdef cppclass Patchdef:
        Patchdef(Statedef*, uint, steps_wm.Patch*)


# ======================================================================================================================
cdef extern from "steps/solver/diffdef.hpp" namespace "steps::solver":
# ----------------------------------------------------------------------------------------------------------------------

    ###### Cybinding for Diffdef ######
    cdef cppclass Diffdef:
        Diffdef(Statedef*, uint, steps_model.Diff*)


# ======================================================================================================================
cdef extern from "steps/solver/ohmiccurrdef.hpp" namespace "steps::solver":
# ----------------------------------------------------------------------------------------------------------------------
    ctypedef OhmicCurrdef* OhmicCurrDefP
    ctypedef std.vector[OhmicCurrdef*] OhmicCurrDefPVec
    ctypedef std.vector[OhmicCurrdef*].iterator OhmicCurrDefPVecI
    ctypedef std.vector[OhmicCurrdef*].const_iterator OhmicCurrDefPVecCI

    ###### Cybinding for OhmicCurrdef ######
    cdef cppclass OhmicCurrdef:
        OhmicCurrdef(Statedef*, uint, steps_model.OhmicCurr*)


# ======================================================================================================================
cdef extern from "steps/solver/sreacdef.hpp" namespace "steps::solver":
# ----------------------------------------------------------------------------------------------------------------------

    ###### Cybinding for SReacdef ######
    cdef cppclass SReacdef:
        SReacdef(Statedef*, uint, steps_model.SReac*)


# ======================================================================================================================
cdef extern from "steps/solver/statedef.hpp" namespace "steps::solver":
# ----------------------------------------------------------------------------------------------------------------------

    ###### Cybinding for Statedef ######
    cdef cppclass Statedef:
        Statedef(steps_model.Model*, steps_wm.Geom*, steps_rng.RNG*)


# ======================================================================================================================
cdef extern from "steps/solver/compdef.hpp" namespace "steps::solver":
# ----------------------------------------------------------------------------------------------------------------------
    ctypedef Compdef* CompDefP
    ctypedef std.vector[Compdef*] CompDefPVec
    ctypedef std.vector[Compdef*].iterator CompDefPVecI
    ctypedef std.vector[Compdef*].const_iterator CompDefPVecCI

    ###### Cybinding for Compdef ######
    cdef cppclass Compdef:
        Compdef(Statedef*, uint, steps_wm.Comp*)


# ======================================================================================================================
cdef extern from "steps/solver/vdepsreacdef.hpp" namespace "steps::solver":
# ----------------------------------------------------------------------------------------------------------------------
    ctypedef VDepSReacdef* VDepSReacDefP
    ctypedef std.vector[VDepSReacdef*] VDepSReacDefPVec
    ctypedef std.vector[VDepSReacdef*].iterator VDepSReacDefPVecI
    ctypedef std.vector[VDepSReacdef*].const_iterator VDepSReacDefPVecCI

    ###### Cybinding for VDepSReacdef ######
    cdef cppclass VDepSReacdef:
        VDepSReacdef(Statedef*, uint, steps_model.VDepSReac*)


# ======================================================================================================================
cdef extern from "steps/solver/specdef.hpp" namespace "steps::solver":
# ----------------------------------------------------------------------------------------------------------------------

    ###### Cybinding for Specdef ######
    cdef cppclass Specdef:
        Specdef(Statedef*, uint, steps_model.Spec*)


# ======================================================================================================================
cdef extern from "steps/solver/vdeptransdef.hpp" namespace "steps::solver":
# ----------------------------------------------------------------------------------------------------------------------
    ctypedef VDepTransdef* VDepTransDefP
    ctypedef std.vector[VDepTransdef*] VDepTransDefPVec
    ctypedef std.vector[VDepTransdef*].iterator VDepTransDefPVecI
    ctypedef std.vector[VDepTransdef*].const_iterator VDepTransDefPVecCI

    ###### Cybinding for VDepTransdef ######
    cdef cppclass VDepTransdef:
        VDepTransdef(Statedef*, uint, steps_model.VDepTrans*)


# ======================================================================================================================
cdef extern from "steps/solver/ghkcurrdef.hpp" namespace "steps::solver":
# ----------------------------------------------------------------------------------------------------------------------
    ctypedef GHKcurrdef* GHKcurrDefP
    ctypedef std.vector[GHKcurrdef*] GHKcurrDefPVec
    ctypedef std.vector[GHKcurrdef*].iterator GHKcurrDefPVecI
    ctypedef std.vector[GHKcurrdef*].const_iterator GHKcurrDefPVecCI

    ###### Cybinding for GHKcurrdef ######
    cdef cppclass GHKcurrdef:
        GHKcurrdef(Statedef*, uint, steps_model.GHKcurr*)


# ======================================================================================================================
cdef extern from "steps/solver/reacdef.hpp" namespace "steps::solver":
# ----------------------------------------------------------------------------------------------------------------------

    ###### Cybinding for Reacdef ######
    cdef cppclass Reacdef:
        Reacdef(Statedef*, uint, steps_model.Reac*)


# ======================================================================================================================
cdef extern from "steps/solver/chandef.hpp" namespace "steps::solver":
# ----------------------------------------------------------------------------------------------------------------------

    ###### Cybinding for Chandef ######
    cdef cppclass Chandef:
        Chandef(Statedef*, uint, steps_model.Chan*)


# ======================================================================================================================
cdef extern from "steps/solver/diffboundarydef.hpp" namespace "steps::solver":
# ----------------------------------------------------------------------------------------------------------------------
    ctypedef DiffBoundarydef* DiffBoundaryDefP
    ctypedef std.vector[DiffBoundarydef*] DiffBoundaryDefPVec
    ctypedef std.vector[DiffBoundarydef*].iterator DiffBoundaryDefPVecI
    ctypedef std.vector[DiffBoundarydef*].const_iterator DiffBoundaryDefPVecCI

    ###### Cybinding for DiffBoundarydef ######
    cdef cppclass DiffBoundarydef:
        #The constructor requires another namespace
        pass


# ======================================================================================================================
cdef extern from "steps/solver/sdiffboundarydef.hpp" namespace "steps::solver":
# ----------------------------------------------------------------------------------------------------------------------
    ctypedef SDiffBoundarydef* SDiffBoundaryDefP
    ctypedef std.vector[SDiffBoundarydef*] SDiffBoundaryDefPVec
    ctypedef std.vector[SDiffBoundarydef*].iterator SDiffBoundaryDefPVecI
    ctypedef std.vector[SDiffBoundarydef*].const_iterator SDiffBoundaryDefPVecCI

    ###### Cybinding for DiffBoundarydef ######
    cdef cppclass SDiffBoundarydef:
        #The constructor requires another namespace
        pass


# ======================================================================================================================
cdef extern from "steps/solver/api.hpp" namespace "steps::solver::API":
# ----------------------------------------------------------------------------------------------------------------------
    enum EF_solver:
        EF_NONE
        EF_DEFAULT
        EF_DV_BDSYS
        EF_DV_PETSC


# ======================================================================================================================
cdef extern from "steps/solver/api.hpp" namespace "steps::solver":
# ----------------------------------------------------------------------------------------------------------------------
#
    ###### Cybinding for API ######
    # This is now just placeholding- actually contains ALL solver functions, including
    # Those not in the API
    cdef cppclass API:
        API(steps_model.Model*, steps_wm.Geom*, steps_rng.RNG*)
        # Only functions that are in pyx API
        #std.string getSolverName() except +
        #std.string getSolverDesc() except +
        #std.string getSolverAuthors() except +
        #std.string getSolverEmail() except +
        #void checkpoint(std.string) except +
        #void restore(std.string) except +
        #void reset() except +
        #void run(double) except +
        #void advance(double) except +
        #void step() except +
        #void setRk4DT(double) except +
        #void setDT(double) except +
        #void setEfieldDT(double) except +
        #void setNSteps(uint) except +
        #void setTime(double) except +
        #void setTemp(double) except +
        #double getTime() except +
        #double getRk4DT() except +
        #double getDT() except +
        #double getEfieldDT() except +
        #double getTemp() except +
        #double getA0() except +
        #uint getNSteps() except +
        double getCompVol(std.string) except +
        void setCompVol(std.string, double) except +
        double getCompCount(std.string, std.string) except +
        void setCompCount(std.string, std.string, double) except +
        double getCompAmount(std.string, std.string) except +
        void setCompAmount(std.string, std.string, double) except +
        double getCompConc(std.string, std.string) except +
        void setCompConc(std.string, std.string, double) except +
        bool getCompClamped(std.string, std.string) except +
        void setCompClamped(std.string, std.string, bool) except +
        double getCompReacK(std.string, std.string) except +
        void setCompReacK(std.string, std.string, double) except +
        bool getCompReacActive(std.string, std.string) except +
        void setCompReacActive(std.string, std.string, bool) except +
        double getCompDiffD(std.string, std.string) except +
        void setCompDiffD(std.string, std.string, double) except +
        bool getCompDiffActive(std.string, std.string) except +
        void setCompDiffActive(std.string, std.string, bool) except +
        double getCompReacC(std.string, std.string) except +
        double getCompReacH(std.string, std.string) except +
        double getCompReacA(std.string, std.string) except +
        unsigned long long getCompReacExtent(std.string, std.string) except +
        void resetCompReacExtent(std.string, std.string) except +
        double getTetVol(uint) except +
        void setTetVol(uint, double) except +
        bool getTetSpecDefined(uint, std.string) except +
        double getTetCount(uint, std.string) except +
        void setTetCount(uint, std.string, double) except +
        double getTetAmount(uint, std.string) except +
        void setTetAmount(uint, std.string, double) except +
        double getTetConc(uint, std.string) except +
        void setTetConc(uint, std.string, double) except +
        bool getTetClamped(uint, std.string) except +
        void setTetClamped(uint, std.string, bool) except +
        double getTetReacK(uint, std.string) except +
        void setTetReacK(uint, std.string, double) except +
        bool getTetReacActive(uint, std.string) except +
        void setTetReacActive(uint, std.string, bool) except +
        double getTetDiffD(uint, std.string, uint) except +
        void setTetDiffD(uint, std.string, double, uint) except +
        bool getTetDiffActive(uint, std.string) except +
        void setTetDiffActive(uint, std.string, bool) except +
        double getTetReacC(uint, std.string) except +
        double getTetReacH(uint, std.string) except +
        double getTetReacA(uint, std.string) except +
        double getTetDiffA(uint, std.string) except +
        double getTetV(uint) except +
        void setTetV(uint, double) except +
        bool getTetVClamped(uint) except +
        void setTetVClamped(uint, bool) except +
        double getPatchArea(std.string) except +
        void setPatchArea(std.string, double) except +
        double getPatchCount(std.string, std.string) except +
        void setPatchCount(std.string, std.string, double) except +
        double getPatchAmount(std.string, std.string) except +
        void setPatchAmount(std.string, std.string, double) except +
        bool getPatchClamped(std.string, std.string) except +
        void setPatchClamped(std.string, std.string, bool) except +
        double getPatchSReacK(std.string, std.string) except +
        void setPatchSReacK(std.string, std.string, double) except +
        bool getPatchSReacActive(std.string, std.string) except +
        void setPatchSReacActive(std.string, std.string, bool) except +
        double getPatchSReacC(std.string, std.string) except +
        double getPatchSReacH(std.string, std.string) except +
        double getPatchSReacA(std.string, std.string) except +
        unsigned long long getPatchSReacExtent(std.string, std.string) except +
        void resetPatchSReacExtent(std.string, std.string) except +
        bool getPatchVDepSReacActive(std.string, std.string) except +
        void setPatchVDepSReacActive(std.string, std.string, bool) except +
        void setDiffBoundaryDiffusionActive(std.string, std.string, bool) except +
        bool getDiffBoundaryDiffusionActive(std.string, std.string) except +
        void setDiffBoundaryDcst(std.string, std.string, double, std.string) except +
        void setSDiffBoundaryDiffusionActive(std.string, std.string, bool) except +
        bool getSDiffBoundaryDiffusionActive(std.string, std.string) except +
        void setSDiffBoundaryDcst(std.string, std.string, double, std.string) except +
        double getTriArea(uint) except +
        void setTriArea(uint, double) except +
        bool getTriSpecDefined(uint, std.string) except +
        double getTriCount(uint, std.string) except +
        void setTriCount(uint, std.string, double) except +
        double getTriAmount(uint, std.string) except +
        void setTriAmount(uint, std.string, double) except +
        bool getTriClamped(uint, std.string) except +
        void setTriClamped(uint, std.string, bool) except +
        double getTriSReacK(uint, std.string) except +
        void setTriSReacK(uint, std.string, double) except +
        bool getTriSReacActive(uint, std.string) except +
        void setTriSReacActive(uint, std.string, bool) except +
        double getTriSReacC(uint, std.string) except +
        double getTriSReacH(uint, std.string) except +
        double getTriSReacA(uint, std.string) except +
        double getTriDiffD(uint, std.string, uint) except +
        double getTriSDiffD(uint, std.string, uint) except +
        void setTriDiffD(uint, std.string, double, uint) except +
        void setTriSDiffD(uint, std.string, double, uint) except +
        double getTriV(uint) except +
        void setTriV(uint, double) except +
        bool getTriVClamped(uint) except +
        void setTriVClamped(uint, bool) except +
        double getTriOhmicI(uint) except +
        double getTriOhmicI(uint, std.string) except +
        double getTriGHKI(uint) except +
        double getTriGHKI(uint, std.string) except +
        double getTriI(uint) except +
        void setTriIClamp(uint, double) except +
        bool getTriVDepSReacActive(uint, std.string) except +
        void setTriVDepSReacActive(uint, std.string, bool) except +
        void setTriCapac(uint, double) except +
        double getVertV(uint) except +
        void setVertV(uint, double) except +
        bool getVertVClamped(uint) except +
        void setVertVClamped(uint, bool) except +
        void setVertIClamp(uint, double) except +
        void setMembPotential(std.string, double) except +
        void setMembCapac(std.string, double) except +
        void setMembVolRes(std.string, double) except +
        void setMembRes(std.string, double, double) except +
        uint getNComps() except +
        uint getNPatches() except +
        std.string getCompName(uint) except +
        std.string getPatchName(uint) except +
        uint getNCompSpecs(uint) except +
        uint getNPatchSpecs(uint) except +
        std.string getCompSpecName(uint, uint) except +
        std.string getPatchSpecName(uint, uint) except +
        double sumBatchTetCountsNP(uint*, int, std.string) except +
        double sumBatchTriCountsNP(uint*, int, std.string) except +
        double sumBatchTriGHKIsNP(uint*, int, std.string) except +
        double sumBatchTriOhmicIsNP(uint*, int, std.string) except +
        void setDiffApplyThreshold(int) except +
        unsigned long long getReacExtent(bool) except +
        unsigned long long getDiffExtent(bool) except +
        double getNIteration() except +
        double getCompTime() except +
        double getSyncTime() except +
        double getIdleTime() except +
        double getUpdPeriod() except +
        double getEFieldTime() except +
        double getRDTime() except +
        double getDataExchangeTime() except +
        void repartitionAndReset(std.vector[uint],std.map[uint, uint], std.vector[uint]) except +
