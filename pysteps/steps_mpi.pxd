# -*- coding: utf-8 -*-
# =====================================================================================================================
# These bindings were automatically generated by cyWrap. Please do dot modify.
# Additional functionality shall be implemented in sub-classes.
#
__copyright__ = "Copyright 2016 EPFL BBP-project"
# =====================================================================================================================
from libcpp cimport bool
cimport std
cimport steps_solver
cimport steps_model
cimport steps_tetmesh
cimport steps_wm
cimport steps_rng


# ======================================================================================================================
cdef extern from "steps/mpi/mpi_common.hpp" namespace "steps::mpi":
# ----------------------------------------------------------------------------------------------------------------------
    enum MsgTag:
        OPSPLIT_MOLECULE_CHANGE
        OPSPLIT_MOLECULE_CHANGE_COMPLETE
        OPSPLIT_COUNT_SYNC_INFO
        OPSPLIT_COUNT_SYNC_DATA
        OPSPLIT_SYNC_COMPLETE
        OPSPLIT_KPROC_UPD
        OPSPLIT_UPD_COMPLETE

# ======================================================================================================================
cdef extern from "steps/mpi/mpi_init.hpp" namespace "steps::mpi":
# ----------------------------------------------------------------------------------------------------------------------
    void mpiInit()
    int getRank()
    int getNHosts()

# ======================================================================================================================
cdef extern from "steps/mpi/mpi_finish.hpp" namespace "steps::mpi":
# ----------------------------------------------------------------------------------------------------------------------
    void mpiFinish()


# # ======================================================================================================================
# cdef extern from "steps/mpi/tetopsplit/tet.hpp" namespace "steps::mpi::tetopsplit":
# # ----------------------------------------------------------------------------------------------------------------------
#     # ctypedef Tet* TetP
#     # ctypedef std.vector[Tet*] TetPVec
#     # ctypedef std.vector[Tet*].iterator TetPVecI
#     # ctypedef std.vector[Tet*].const_iterator TetPVecCI
#
#     ###### Cybinding for Tet ######
#     cdef cppclass Tet:
#         Tet(unsigned int, steps_solver.Compdef*, double, double, double, double, double, double, double, double, double, int, int, int, int, int, int)
#         void checkpoint(std.fstream)
#         void restore(std.fstream)
#         void setNextTet(unsigned int, Tet*)
#         void setNextTri(unsigned int, Tri*)
#         void setNextTri(Tri*)
#         void setupKProcs(TetOpSplitP*)
#         Tri* nextTri(unsigned int)
#         Tet* nextTet(unsigned int)
#         double area(unsigned int)
#         double dist(unsigned int)
#         int getTetDirection(unsigned int)
#         void setDiffBndDirection(unsigned int)
#         bool getDiffBndDirection(unsigned int)
#         Diff* diff(unsigned int)
#         int tet(unsigned int)
#         void setupDeps()
#         bool KProcDepSpecTet(unsigned int, WmVol*, unsigned int)
#         bool KProcDepSpecTri(unsigned int, Tri*, unsigned int)
#         void setCount(unsigned int, unsigned int, double)
#         void incCount(unsigned int, int, double)
#         double getPoolOccupancy(unsigned int)
#         double getLastUpdate(unsigned int)
#         void resetPoolOccupancy()
#
# # ======================================================================================================================
# cdef extern from "steps/mpi/tetopsplit/patch.hpp" namespace "steps::mpi::tetopsplit":
# # ----------------------------------------------------------------------------------------------------------------------
#     # ctypedef Patch* PatchP
#     # ctypedef std.vector[Patch*] PatchPVec
#     # ctypedef std.vector[Patch*].iterator PatchPVecI
#     # ctypedef std.vector[Patch*].const_iterator PatchPVecCI
#
#     ###### Cybinding for Patch ######
#     cdef cppclass Patch(steps_wm.Patch):
#         Patch(steps_solver.Patchdef*)
#         void checkpoint(std.fstream)
#         void restore(std.fstream)
#         void addTri(Tri*)
#         void reset()
#         steps_solver.Patchdef* def_()
#         double area()
#         double* pools()
#         void modCount(unsigned int, double)
#         unsigned int countTris()
#         Tri* pickTriByArea(double)
#         std.vector[Tri*].const_iterator bgnTri()
#         std.vector[Tri*].const_iterator endTri()
#
# # ======================================================================================================================
# cdef extern from "steps/mpi/tetopsplit/crstruct.hpp" namespace "steps::mpi::tetopsplit":
# # ----------------------------------------------------------------------------------------------------------------------
#
#     ###### Cybinding for CRGroup ######
#     cdef cppclass CRGroup:
#         CRGroup(int, unsigned int)
#         void free_indices()
#
#     ###### Cybinding for CRKProcData ######
#     cdef cppclass CRKProcData:
#         CRKProcData()
#
# # ======================================================================================================================
# cdef extern from "steps/mpi/tetopsplit/tri.hpp" namespace "steps::mpi::tetopsplit":
# # ----------------------------------------------------------------------------------------------------------------------
#     # ctypedef Tri* TriP
#     # ctypedef std.vector[Tri*] TriPVec
#     # ctypedef std.vector[Tri*].iterator TriPVecI
#     # ctypedef std.vector[Tri*].const_iterator TriPVecCI
#
#     ###### Cybinding for Tri ######
#     cdef cppclass Tri:
#         Tri(unsigned int, steps_solver.Patchdef*, double, double, double, double, double, double, double, int, int, int, int, int, int, int)
#         void checkpoint(std.fstream)
#         void restore(std.fstream)
#         void setInnerTet(WmVol*)
#         void setOuterTet(WmVol*)
#         void setNextTri(unsigned int, Tri*)
#         void setupKProcs(TetOpSplitP*, bool)
#         void reset()
#         steps_solver.Patchdef* patchdef()
#         unsigned int idx()
#         double area()
#         WmVol* iTet()
#         WmVol* oTet()
#         Tri* nextTri(unsigned int)
#         int tri(unsigned int)
#         double length(unsigned int)
#         double dist(unsigned int)
#         int tet(unsigned int)
#         int getTriDirection(unsigned int)
#         void setDiffBndDirection(unsigned int)
#         bool getDiffBndDirection(unsigned int)
#         unsigned int getStartKProcIdx()
#         void setupDeps()
#         bool KProcDepSpecTet(unsigned int, WmVol*, unsigned int)
#         bool KProcDepSpecTri(unsigned int, Tri*, unsigned int)
#         void incECharge(unsigned int, int)
#         void resetECharge()
#         void resetOCintegrals()
#         double computeI(double, double, double)
#         double getOhmicI(double, double)
#         double getOhmicI(unsigned int, double, double)
#         double getGHKI(double)
#         double getGHKI(unsigned int, double)
#         unsigned int* pools()
#         void setCount(unsigned int, unsigned int, double)
#         void incCount(unsigned int, int, double)
#         bool clamped(unsigned int)
#         void setClamped(unsigned int, bool)
#         void setOCchange(unsigned int, unsigned int, double, double)
#         std.vector[KProc*] kprocBegin()
#         std.vector[KProc*] kprocEnd()
#         std.vector[KProc*] kprocs()
#         unsigned int countKProcs()
#         KProc* getKProc(unsigned int)
#         SReac* sreac(unsigned int)
#         SDiff* sdiff(unsigned int)
#         VDepTrans* vdeptrans(unsigned int)
#         VDepSReac* vdepsreac(unsigned int)
#         GHKcurr* ghkcurr(unsigned int)
#         bool getInHost()
#         void setHost(int, int)
#         int getHost()
#         void setSolver(TetOpSplitP*)
#         TetOpSplitP* solver()
#         double getPoolOccupancy(unsigned int)
#         double getLastUpdate(unsigned int)
#         void resetPoolOccupancy()
#
# # ======================================================================================================================
# cdef extern from "steps/mpi/tetopsplit/vdepsreac.hpp" namespace "steps::mpi::tetopsplit":
# # ----------------------------------------------------------------------------------------------------------------------
#
#     ###### Cybinding for VDepSReac ######
#     cdef cppclass VDepSReac:
#         VDepSReac(steps_solver.VDepSReacdef*, Tri*)
#         void checkpoint(std.fstream)
#         void restore(std.fstream)
#         void setupDeps()
#         bool depSpecTet(unsigned int, WmVol*)
#         bool depSpecTri(unsigned int, Tri*)
#         void reset()
#         double rate(TetOpSplitP*)
#         double getScaledDcst(TetOpSplitP*)
#         void apply(steps_rng.RNG*, double, double, double)
#         std.vector[KProc*] getLocalUpdVec(int)
#         std.vector[unsigned int] getRemoteUpdVec(int)
#         void resetOccupancies()
#         bool getInHost()
#         int getHost()
#
# # ======================================================================================================================
# cdef extern from "steps/mpi/tetopsplit/vdeptrans.hpp" namespace "steps::mpi::tetopsplit":
# # ----------------------------------------------------------------------------------------------------------------------
#
#     ###### Cybinding for VDepTrans ######
#     cdef cppclass VDepTrans:
#         VDepTrans(steps_solver.VDepTransdef*, Tri*)
#         void checkpoint(std.fstream)
#         void restore(std.fstream)
#         void setupDeps()
#         bool depSpecTet(unsigned int, WmVol*)
#         bool depSpecTri(unsigned int, Tri*)
#         void reset()
#         double rate(TetOpSplitP*)
#         double getScaledDcst(TetOpSplitP*)
#         void apply(steps_rng.RNG*, double, double, double)
#         std.vector[KProc*] getLocalUpdVec(int)
#         std.vector[unsigned int] getRemoteUpdVec(int)
#         void resetOccupancies()
#         bool getInHost()
#         int getHost()
#
# # ======================================================================================================================
# cdef extern from "steps/mpi/tetopsplit/ghkcurr.hpp" namespace "steps::mpi::tetopsplit":
# # ----------------------------------------------------------------------------------------------------------------------
#
#     ###### Cybinding for GHKcurr ######
#     cdef cppclass GHKcurr:
#         GHKcurr(steps_solver.GHKcurrdef*, Tri*)
#         void checkpoint(std.fstream)
#         void restore(std.fstream)
#         void setupDeps()
#         bool depSpecTet(unsigned int, WmVol*)
#         bool depSpecTri(unsigned int, Tri*)
#         void reset()
#         double rate(TetOpSplitP*)
#         double getScaledDcst(TetOpSplitP*)
#         void apply(steps_rng.RNG*, double, double, double)
#         std.vector[KProc*] getLocalUpdVec(int)
#         std.vector[unsigned int] getRemoteUpdVec(int)
#         void resetOccupancies()
#         bool efflux()
#         void setEffFlux(bool)
#         bool getInHost()
#         int getHost()
#
# # ======================================================================================================================
# cdef extern from "steps/mpi/tetopsplit/kproc.hpp" namespace "steps::mpi::tetopsplit":
# # ----------------------------------------------------------------------------------------------------------------------
#     # ctypedef KProc* KProcP
#     # ctypedef std.vector[KProc*] KProcPVec
#     # ctypedef std.vector[KProc*].iterator KProcPVecI
#     # ctypedef std.vector[KProc*].const_iterator KProcPVecCI
#     # ctypedef std.set[KProc*] KProcPSet
#     # ctypedef std.set[KProc*].iterator KProcPSetI
#     # ctypedef std.set[KProc*].const_iterator KProcPSetCI
#
#     ###### Cybinding for KProc ######
#     cdef cppclass KProc:
#         KProc()
#         void checkpoint(std.fstream)
#         void restore(std.fstream)
#         bool active()
#         bool inactive()
#         void setActive(bool)
#         unsigned int flags()
#         unsigned int schedIDX()
#         void setSchedIDX(unsigned int)
#         void setupDeps()
#         bool depSpecTet(unsigned int, WmVol*)
#         bool depSpecTri(unsigned int, Tri*)
#         void reset()
#         void resetCcst()
#         double rate(TetOpSplitP*)
#         double getScaledDcst(TetOpSplitP*)
#         double c()
#         double h()
#         int apply(steps_rng.RNG*)
#         int apply(steps_rng.RNG*, unsigned int)
#         std.vector[KProc*] getLocalUpdVec(int)
#         std.vector[unsigned int] getRemoteUpdVec(int)
#         void apply(steps_rng.RNG*, double, double, double)
#         void resetOccupancies()
#         bool getInHost()
#         int getHost()
#         unsigned int getExtent()
#         void resetExtent()
#
# # ======================================================================================================================
# cdef extern from "steps/mpi/tetopsplit/comp.hpp" namespace "steps::mpi::tetopsplit":
# # ----------------------------------------------------------------------------------------------------------------------
#     # ctypedef Comp* CompP
#     # ctypedef std.vector[Comp*] CompPVec
#     # ctypedef std.vector[Comp*].iterator CompPVecI
#     # ctypedef std.vector[Comp*].const_iterator CompPVecCI
#
#     ###### Cybinding for Comp ######
#     cdef cppclass Comp:
#         Comp(steps_solver.Compdef*)
#         void checkpoint(std.fstream)
#         void restore(std.fstream)
#         void addTet(WmVol*)
#         void reset()
#         steps_solver.Compdef* def_()
#         double vol()
#         double* pools()
#         void modCount(unsigned int, double)
#         unsigned int countTets()
#         WmVol* pickTetByVol(double)
#         std.vector[WmVol*].const_iterator bgnTet()
#         std.vector[WmVol*].const_iterator endTet()
#
# ======================================================================================================================
cdef extern from "steps/mpi/tetopsplit/tetopsplit.hpp" namespace "steps::mpi::tetopsplit":
# ----------------------------------------------------------------------------------------------------------------------
#     # ctypedef unsigned int SchedIDX
#     # ctypedef std.set[unsigned int] SchedIDXSet
#     # ctypedef std.set[unsigned int].iterator SchedIDXSetI
#     # ctypedef std.set[unsigned int].const_iterator SchedIDXSetCI
#     # ctypedef std.vector[unsigned int] SchedIDXVec
#     # ctypedef std.vector[unsigned int].iterator SchedIDXVecI
#     # ctypedef std.vector[unsigned int].const_iterator SchedIDXVecCI

    enum SubVolType:
        SUB_WM
        SUB_TET
        SUB_TRI

    ###### Cybinding for TetOpSplitP ######
    cdef cppclass TetOpSplitP:
        TetOpSplitP(steps_model.Model*, steps_wm.Geom*, steps_rng.RNG*, int, std.vector[unsigned int], std.map[unsigned int,unsigned int], std.vector[unsigned int]) except +
        std.string getSolverName() except +
        std.string getSolverDesc() except +
        std.string getSolverAuthors() except +
        std.string getSolverEmail() except +
        void checkpoint(std.string) except +
        void restore(std.string) except +
        void reset() except +
        void run(double) except +
        void advance(double) except +
        void step() except +
        void setEfieldDT(double) except +
        void setNSteps(unsigned int) except +
        void setTime(double) except +
        void setTemp(double) except +
        double getTime() except +
        double getEfieldDT() except +
        double getTemp() except +
        double getA0() except +
        unsigned int getNSteps() except +
        double getCompVol(std.string) except +
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
        unsigned int getCompReacExtent(std.string, std.string) except +
        void resetCompReacExtent(std.string, std.string) except +
        double getTetVol(unsigned int) except +
        void setTetVol(unsigned int, double) except +
        bool getTetSpecDefined(unsigned int, std.string) except +
        double getTetCount(unsigned int, std.string) except +
        void setTetCount(unsigned int, std.string, double) except +
        double getTetAmount(unsigned int, std.string) except +
        void setTetAmount(unsigned int, std.string, double) except +
        double getTetConc(unsigned int, std.string) except +
        void setTetConc(unsigned int, std.string, double) except +
        bool getTetClamped(unsigned int, std.string) except +
        void setTetClamped(unsigned int, std.string, bool) except +
        double getTetReacK(unsigned int, std.string) except +
        void setTetReacK(unsigned int, std.string, double) except +
        bool getTetReacActive(unsigned int, std.string) except +
        void setTetReacActive(unsigned int, std.string, bool) except +
        double getTetDiffD(unsigned int, std.string, unsigned int) except +
        void setTetDiffD(unsigned int, std.string, double, unsigned int) except +
        bool getTetDiffActive(unsigned int, std.string) except +
        void setTetDiffActive(unsigned int, std.string, bool) except +
        double getTetReacC(unsigned int, std.string) except +
        double getTetReacH(unsigned int, std.string) except +
        double getTetReacA(unsigned int, std.string) except +
        double getTetDiffA(unsigned int, std.string) except +
        double getTetV(unsigned int) except +
        void setTetV(unsigned int, double) except +
        bool getTetVClamped(unsigned int) except +
        void setTetVClamped(unsigned int, bool) except +
        double getPatchArea(std.string) except +
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
        unsigned int getPatchSReacExtent(std.string, std.string) except +
        void resetPatchSReacExtent(std.string, std.string) except +
        bool getPatchVDepSReacActive(std.string, std.string) except +
        void setPatchVDepSReacActive(std.string, std.string, bool) except +
        void setDiffBoundaryDiffusionActive(std.string, std.string, bool) except +
        bool getDiffBoundaryDiffusionActive(std.string, std.string) except +
        void setDiffBoundaryDcst(std.string, std.string, double, std.string) except +
        void setSDiffBoundaryDiffusionActive(std.string, std.string, bool) except +
        bool getSDiffBoundaryDiffusionActive(std.string, std.string) except +
        void setSDiffBoundaryDcst(std.string, std.string, double, std.string) except +
        double getTriArea(unsigned int) except +
        void setTriArea(unsigned int, double) except +
        bool getTriSpecDefined(unsigned int, std.string) except +
        double getTriCount(unsigned int, std.string) except +
        void setTriCount(unsigned int, std.string, double) except +
        double getTriAmount(unsigned int, std.string) except +
        void setTriAmount(unsigned int, std.string, double) except +
        bool getTriClamped(unsigned int, std.string) except +
        void setTriClamped(unsigned int, std.string, bool) except +
        double getTriSReacK(unsigned int, std.string) except +
        void setTriSReacK(unsigned int, std.string, double) except +
        bool getTriSReacActive(unsigned int, std.string) except +
        void setTriSReacActive(unsigned int, std.string, bool) except +
        double getTriSReacC(unsigned int, std.string) except +
        double getTriSReacH(unsigned int, std.string) except +
        double getTriSReacA(unsigned int, std.string) except +
        double getTriSDiffD(unsigned int, std.string, unsigned int) except +
        void setTriSDiffD(unsigned int, std.string, double, unsigned int) except +
        double getTriV(unsigned int) except +
        void setTriV(unsigned int, double) except +
        bool getTriVClamped(unsigned int) except +
        void setTriVClamped(unsigned int, bool) except +
        double getTriOhmicI(unsigned int) except +
        double getTriOhmicI(unsigned int, std.string) except +
        double getTriGHKI(unsigned int) except +
        double getTriGHKI(unsigned int, std.string) except +
        double getTriI(unsigned int) except +
        void setTriIClamp(unsigned int, double) except +
        bool getTriVDepSReacActive(unsigned int, std.string) except +
        void setTriVDepSReacActive(unsigned int, std.string, bool) except +
        void setTriCapac(unsigned int, double) except +
        double getVertV(unsigned int) except +
        void setVertV(unsigned int, double) except +
        bool getVertVClamped(unsigned int) except +
        void setVertVClamped(unsigned int, bool) except +
        void setVertIClamp(unsigned int, double) except +
        void setMembPotential(std.string, double) except +
        void setMembCapac(std.string, double) except +
        void setMembVolRes(std.string, double) except +
        void setMembRes(std.string, double, double) except +
        std.vector[double] getBatchTetCounts(std.vector[unsigned int], std.string) except +
        std.vector[double] getBatchTriCounts(std.vector[unsigned int], std.string) except +
        void getBatchTetCountsNP(unsigned int*, int, std.string, double*, int) except +
        void getBatchTriCountsNP(unsigned int*, int, std.string, double*, int) except +
        std.vector[double] getROITetCounts(std.string, std.string) except +
        std.vector[double] getROITriCounts(std.string, std.string) except +
        void getROITetCountsNP(std.string, std.string, double*, int) except +
        void getROITriCountsNP(std.string, std.string, double*, int) except +
        double getROIVol(std.string) except +
        double getROIArea(std.string) except +
        double getROICount(std.string, std.string) except +
        void setROICount(std.string, std.string, double) except +
        double getROIAmount(std.string, std.string) except +
        double getROIConc(std.string, std.string) except +
        void setROIConc(std.string, std.string, double) except +
        void setROIClamped(std.string, std.string, bool) except +
        void setROIReacK(std.string, std.string, double) except +
        void setROISReacK(std.string, std.string, double) except +
        void setROIDiffD(std.string, std.string, double) except +
        void setROIReacActive(std.string, std.string, bool) except +
        void setROISReacActive(std.string, std.string, bool) except +
        void setROIDiffActive(std.string, std.string, bool) except +
        void setROIVDepSReacActive(std.string, std.string, bool) except +
        unsigned int getROIReacExtent(std.string, std.string) except +
        void resetROIReacExtent(std.string, std.string) except +
        unsigned int getROISReacExtent(std.string, std.string) except +
        void resetROISReacExtent(std.string, std.string) except +
        unsigned int getROIDiffExtent(std.string, std.string) except +
        void resetROIDiffExtent(std.string, std.string) except +
        void saveMembOpt(std.string) except +
        double sumBatchTetCountsNP(unsigned int*, int, std.string) except +
        double sumBatchTriCountsNP(unsigned int*, int, std.string) except +
        double sumBatchTriGHKIsNP(unsigned int*, int, std.string) except +
        double sumBatchTriOhmicIsNP(unsigned int*, int, std.string) except +
        void setDiffApplyThreshold(int) except +
        double getReacExtent(bool) except +
        double getDiffExtent(bool) except +
        double getNIteration() except +
        double getCompTime() except +
        double getSyncTime() except +
        double getIdleTime() except +
        double getUpdPeriod() except +
        double getEFieldTime() except +
        double getRDTime() except +
        double getDataExchangeTime() except +
        void repartitionAndReset(std.vector[unsigned int],std.map[unsigned int, unsigned int], std.vector[unsigned int]) except +
 

# # ======================================================================================================================
# cdef extern from "steps/mpi/tetopsplit/diffboundary.hpp" namespace "steps::mpi::tetopsplit":
# # ----------------------------------------------------------------------------------------------------------------------
#     # ctypedef DiffBoundary* DiffBoundaryP
#     # ctypedef std.vector[DiffBoundary*] DiffBoundaryPVec
#     # ctypedef std.vector[DiffBoundary*].iterator DiffBoundaryPVecI
#     # ctypedef std.vector[DiffBoundary*].const_iterator DiffBoundaryPVecCI
#
#     ###### Cybinding for DiffBoundary ######
#     cdef cppclass DiffBoundary:
#         DiffBoundary(steps_solver.DiffBoundarydef*)
#         void checkpoint(std.fstream)
#         void restore(std.fstream)
#         steps_solver.DiffBoundarydef* def_()
#         Comp* compA()
#         Comp* compB()
#         void setComps(Comp*, Comp*)
#         void setTetDirection(unsigned int, unsigned int)
#         std.vector[unsigned int] getTets()
#         std.vector[unsigned int] getTetDirection()
#
# # ======================================================================================================================
# cdef extern from "steps/mpi/tetopsplit/wmvol.hpp" namespace "steps::mpi::tetopsplit":
# # ----------------------------------------------------------------------------------------------------------------------
#     # ctypedef WmVol* WmVolP
#     # ctypedef std.vector[WmVol*] WmVolPVec
#     # ctypedef std.vector[WmVol*].iterator WmVolPVecI
#     # ctypedef std.vector[WmVol*].const_iterator WmVolPVecCI
#
#     ###### Cybinding for WmVol ######
#     cdef cppclass WmVol:
#         WmVol(unsigned int, steps_solver.Compdef*, double, int, int)
#         void checkpoint(std.fstream)
#         void restore(std.fstream)
#         void setupKProcs(TetOpSplitP*)
#         void setNextTri(Tri*)
#         void reset()
#         steps_solver.Compdef* compdef()
#         unsigned int idx()
#         double vol()
#         unsigned int* pools()
#         void setCount(unsigned int, unsigned int, double)
#         void incCount(unsigned int, int, double)
#         double conc(unsigned int)
#         bool clamped(unsigned int)
#         void setClamped(unsigned int, bool)
#         std.vector[Tri*] nexttriBegin()
#         std.vector[Tri*] nexttriEnd()
#         unsigned int countNextTris()
#         std.vector[KProc*] kprocBegin()
#         std.vector[KProc*] kprocEnd()
#         unsigned int getStartKProcIdx()
#         unsigned int countKProcs()
#         std.vector[KProc*] kprocs()
#         KProc* getKProc(unsigned int)
#         Reac* reac(unsigned int)
#         double getPoolOccupancy(unsigned int)
#         double getLastUpdate(unsigned int)
#         void resetPoolOccupancy()
#         bool getInHost()
#         int getHost()
#         void setHost(int, int)
#         void setSolver(TetOpSplitP*)
#         TetOpSplitP* solver()
#         void setupDeps()
#         bool KProcDepSpecTet(unsigned int, WmVol*, unsigned int)
#         bool KProcDepSpecTri(unsigned int, Tri*, unsigned int)
#
# # ======================================================================================================================
# cdef extern from "steps/mpi/tetopsplit/reac.hpp" namespace "steps::mpi::tetopsplit":
# # ----------------------------------------------------------------------------------------------------------------------
#
#     ###### Cybinding for Reac ######
#     cdef cppclass Reac:
#         Reac(steps_solver.Reacdef*, WmVol*)
#         void checkpoint(std.fstream)
#         void restore(std.fstream)
#         double c()
#         void resetCcst()
#         double kcst()
#         void setKcst(double)
#         double h()
#         void setupDeps()
#         bool depSpecTet(unsigned int, WmVol*)
#         bool depSpecTri(unsigned int, Tri*)
#         void reset()
#         double rate(TetOpSplitP*)
#         double getScaledDcst(TetOpSplitP*)
#         void apply(steps_rng.RNG*, double, double, double)
#         std.vector[KProc*] getLocalUpdVec(int)
#         std.vector[unsigned int] getRemoteUpdVec(int)
#         void resetOccupancies()
#         bool getInHost()
#         int getHost()
#         WmVol* container()
#
# # ======================================================================================================================
# cdef extern from "steps/mpi/tetopsplit/diff.hpp" namespace "steps::mpi::tetopsplit":
# # ----------------------------------------------------------------------------------------------------------------------
#
#     ###### Cybinding for Diff ######
#     cdef cppclass Diff:
#         Diff(steps_solver.Diffdef*, Tet*)
#         void checkpoint(std.fstream)
#         void restore(std.fstream)
#         steps_solver.Diffdef* def_()
#         double dcst(int)
#         void setDcst(double)
#         void setDirectionDcst(int, double)
#         void setupDeps()
#         bool depSpecTet(unsigned int, WmVol*)
#         bool depSpecTri(unsigned int, Tri*)
#         void reset()
#         double rate(TetOpSplitP*)
#         double getScaledDcst(TetOpSplitP*)
#         int apply(steps_rng.RNG*)
#         int apply(steps_rng.RNG*, unsigned int)
#         std.vector[KProc*] getLocalUpdVec(int)
#         std.vector[unsigned int] getRemoteUpdVec(int)
#         void setDiffBndActive(unsigned int, bool)
#         bool getDiffBndActive(unsigned int)
#         unsigned int getLigLidx()
#         Tet* getTet()
#         bool getInHost()
#         int getHost()
#
# # ======================================================================================================================
# cdef extern from "steps/mpi/tetopsplit/sreac.hpp" namespace "steps::mpi::tetopsplit":
# # ----------------------------------------------------------------------------------------------------------------------
#
#     ###### Cybinding for SReac ######
#     cdef cppclass SReac:
#         SReac(steps_solver.SReacdef*, Tri*)
#         void checkpoint(std.fstream)
#         void restore(std.fstream)
#         double c()
#         void resetCcst()
#         double kcst()
#         void setKcst(double)
#         double h()
#         void setupDeps()
#         bool depSpecTet(unsigned int, WmVol*)
#         bool depSpecTri(unsigned int, Tri*)
#         void reset()
#         double rate(TetOpSplitP*)
#         double getScaledDcst(TetOpSplitP*)
#         void apply(steps_rng.RNG*, double, double, double)
#         std.vector[KProc*] getLocalUpdVec(int)
#         std.vector[unsigned int] getRemoteUpdVec(int)
#         void resetOccupancies()
#         bool getInHost()
#         int getHost()
#
# # ======================================================================================================================
# cdef extern from "steps/mpi/tetopsplit/sdiff.hpp" namespace "steps::mpi::tetopsplit":
# # ----------------------------------------------------------------------------------------------------------------------
#
#     ###### Cybinding for SDiff ######
#     cdef cppclass SDiff:
#         SDiff(steps_solver.Diffdef*, Tri*)
#         void checkpoint(std.fstream)
#         void restore(std.fstream)
#         steps_solver.Diffdef* sdef()
#         double dcst(int)
#         void setDcst(double)
#         void setDirectionDcst(int, double)
#         void setupDeps()
#         bool depSpecTet(unsigned int, WmVol*)
#         bool depSpecTri(unsigned int, Tri*)
#         void reset()
#         double rate(TetOpSplitP*)
#         double getScaledDcst(TetOpSplitP*)
#         int apply(steps_rng.RNG*)
#         int apply(steps_rng.RNG*, unsigned int)
#         std.vector[KProc*] getLocalUpdVec(int)
#         std.vector[unsigned int] getRemoteUpdVec(int)
#         bool getInHost()
#         int getHost()
#         unsigned int getLigLidx()
#         Tri* getTri()
#
