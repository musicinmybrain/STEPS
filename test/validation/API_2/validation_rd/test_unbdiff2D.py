########################################################################

# 2D diffusion on an infinite surface from point source. 

# AIMS: to verify STEPS spatial-stochastic solver 'Tetexact' supports 
# local initial conditions and calculates surface diffusion rates correctly.

# STEPS equivalent model: Stochastic 2D diffusion on one outer surface 
# of a disk; central triangle source.

# This model is equivalet to that described at
# http://www.biomedcentral.com/content/supplementary/1752-0509-6-36-s4.pdf
# "3D diffusion in infinite volume"
# with a 2D compartments ("atch") instead of a 3D compartments.

# Verification also takes place of model and mesh construction 
# components, particularly mesh loading and manipulation capabilities 
# with functions such as steps.geom.Tetmesh.getSurfTris and 
# steps.geom.Tetmesh.getTriArea etc. 
# Localised recording by steps.solver.Tetexact.getTriCount is also verified. 

# A 6% tolerance is imposed when comparing the mean output from 100 
# stochastic simulations of the STEPS model to the analytical solution. 
# There is an expected probability of failure of < 1%.

########################################################################

import unittest

import steps.interface

from steps.model import *
from steps.sim import *
from steps.saving import *
from steps.geom import *
from steps.rng import *

import math
import numpy
import time
import datetime

from . import tol_funcs

########################################################################

class TestRDUnbdiff2D(unittest.TestCase):

    def setUp(self):
        global NITER, DT, INT, NINJECT, DCST, tolerance, MESHFILE

        # Number of iterations; plotting dt; sim endtime:
        NITER = 100

        DT = 0.01
        INT = 0.21

        # Number of molecules injected in center; diff constant; number of tets sampled:
        NINJECT = 1000	

        DCST = 0.02e-9

        # With good code <1% fail with a tolerance of 6% 
        tolerance = 6.0/100

        ########################################################################

        MESHFILE = 'coin_10r_1h_13861.inp'

    ########################################################################

    def test_unbdiff2D(self):
        "Surface Diffusion - Unbounded, point source (Tetexact), serial"
        
        mdl = Model()
        with mdl:
            X = Species.Create()
            ssys = SurfaceSystem.Create()
            with ssys:
                dif_X = Diffusion.Create(X, DCST)

        ########################################################################

        mesh = TetMesh.Load('validation_rd/meshes/'+MESHFILE)
        
        with mesh:
            comp = Compartment.Create(mesh.tets)
            
            patch_tris = TriList(tri for tri in mesh.surface if all(v.z > 0 for v in tri.verts))
        
            patch = Patch.Create(patch_tris, comp, None, ssys)

                    
        # Find the central triangle
        ctri = (mesh.tets[0.0, 0.0, 0.5e-6].faces & patch_tris)[0]
        
        # Now find the distance of the center of the tris to the center of the center tri
        trirads = [numpy.linalg.norm(tri.center - ctri.center) * 1e6 for tri in patch_tris]
        triareas = [tri.Area for tri in patch_tris]
        
        ########################################################################

        rng = RNG('r123', 1024, 1000)

        sim = Simulation('Tetexact', mdl, mesh, rng)

        rs = ResultSelector(sim)

        res_count = rs.TRIS(patch_tris).X.Count
        res_conc = 1e-12 * rs.TRIS(patch_tris).X.Count / rs.TRIS(patch_tris).Area

        sim.toSave(res_count, res_conc, dt=DT)

        for j in range(NITER):
            sim.newRun()

            sim.TRI(ctri).X.Count = NINJECT

            sim.run(INT)

        itermeans_count = numpy.mean(res_count.data, axis = 0)
        itermeans_conc = numpy.mean(res_conc.data, axis = 0)

        tpnt_compare = [12, 16, 20]
        passed = True
        max_err = 0.0

        for t in tpnt_compare:
            bin_n = 20
            
            r_max = 0.0 	
            for i in trirads: 		
                if (i > r_max):
                    r_max = i
            
            r_min = 0.0
            
            r_seg = (r_max-r_min)/bin_n
            bin_mins = numpy.zeros(bin_n+1)
            r_tris_binned = numpy.zeros(bin_n)
            bin_areas = numpy.zeros(bin_n)    
            
            r = r_min
            for b in range(bin_n + 1):
                bin_mins[b] = r
                if (b!=bin_n):
                    r_tris_binned[b] = r +r_seg/2.0
                r+=r_seg
            bin_counts = [None]*bin_n
            for i in range(bin_n): bin_counts[i] = []
            for i in range((itermeans_count[t].size)):
                i_r = trirads[i]
                for b in range(bin_n):
                    if(i_r>=bin_mins[b] and i_r<bin_mins[b+1]):
                        bin_counts[b].append(itermeans_count[t][i])
                        bin_areas[b]+=triareas[i]
                        break
            
            bin_concs = numpy.zeros(bin_n)
            for c in range(bin_n): 
                for d in range(bin_counts[c].__len__()):
                    bin_concs[c] += bin_counts[c][d]
                bin_concs[c]/=(bin_areas[c]*1.0e12)
            
            for i in range(bin_n):
                if (r_tris_binned[i] > 2.0 and r_tris_binned[i] < 5.0):
                    rad = r_tris_binned[i]*1.0e-6
                    det_conc = 1.0e-12*(NINJECT/(4*math.pi*DCST*res_count.time[0,t]))*(math.exp((-1.0*(rad*rad))/(4*DCST*res_count.time[0,t])))
                    steps_conc = bin_concs[i]
                    self.assertTrue(tol_funcs.tolerable(det_conc, steps_conc, tolerance))
                    
    ########################################################################

def suite():
    all_tests = []
    all_tests.append(unittest.makeSuite(TestRDUnbdiff2D, "test"))
    return unittest.TestSuite(all_tests)

if __name__ == "__main__":
    unittest.TextTestRunner(verbosity=2).run(suite())
