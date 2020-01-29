Release Notes

Version 3.5.0 (2019-10)
==========================

Python API
----------
1. values for unknown tetrahedron and triangle identifiers changed from
   -1 to `steps.geom.UNKNOWN_TET` and `steps.geom.UNKNOWN_TRI`
   respectively.
   You may use these 2 constants instead of -1.

    A Python 3 Flake8 extension is available to help you migrate your code.

    ```
    $ pip3 install flake8 flake8-oist-steps
    $ cat test.py
    import steps.utilities.meshio as meshio

    mesh = meshio.loadMesh('axon_cube_L1000um_D866m_1135tets')[0]
    idx = mesh.findTetByPoint([0, 0, 0])
    if idx == -1:
        print('boundary')
    $ flake8 test.py
    ./test.py:5:4: E421 consider using steps.geom.UNKNOWN_TET constant instead of -1.
    ```

    You may use the following snippet in your scripts header to have backward
    compatible code:

   ```python
   try:
       from steps.geom import UNKNOWN_TET, UNKNOWN_TRI
   except ImportError:
       UNKNOWN_TET = -1
       UNKNOWN_TRI = -1
   ```

2. A new constant `steps.geom.INDEX_DTYPE` provides the proper NumPy datatype according
to whether STEPS has been built with 32 bits or 64 bits identifiers.

Build and Packaging
-------------------
1. New `USE_64_BITS_INDICES` CMake option to use 64bits unsigned integers
   for identifiers instead of 32bits.
2. New CMake variables to use either system libraries or bundle code.
    * `USE_BUNDLE_EASYLOGGINGP`
    * `USE_BUNDLE_RANDOM123`
    * `USE_BUNDLE_SUNDIALS`
3. Improve spack support
4. Improve support of GCC 7 and higher
5. Improve support of AppleClang compiler

Internal code base
------------------
1. Disable OpenMP instructions in TetOpSplit solver
2. Modernize code base by using most of C++11
3. Increase code safety by using strong types to distinguish
   identifiers from tetrahedrons, triangles, and vertices

Version 3.4.0 (2018-11)
==========================
1. Optimization of non-spatial deterministic solver Wmrk4. Vast improvement in performance for large models.
2. Various bug fixes and unit tests.

Version 3.3.0 (2018-04)
==========================
1. Implemented Rejection-based SSA (Thanh V, Zunino R, Priami C (n.d.) On the rejection-based algorithm for simulation and analysis of large-scale reaction networks. The Journal of Chemical Physics 142:244106) as a faster alternative to Wmdirect
2. Assertion and Exception logging overhaul
3. Remove SWIG interface
4. Matlab Simbiology support utility

Version 3.2.0 (2017-10)
==========================
1. Extensive rework on Cython binding and documentation with Jupyter notebook.
2. Extend support for Python 3.3~.
3. several bug fixes.
Note: SWIG Binding will be removed in 3.3 release.

Version 3.1.0 (2017-6)
==========================
1. Optimization on solver constructors,which significantly speedup the initialization time for simulations with large meshes.
2. Cython binding for user interface
3. Parallel EField solver using PETSc library
4. Surface Diffusion Boundary for Tetexact and TetOpSplit
5. Further improve CMake compilation system
6. Restructure validation and test packages

Version 3.0.2 (2017-2)
==========================
1. Bug fix: abort CMake if SWIG is REQUIRED but not found

Version 3.0.1 (2017-2)
==========================
1. Bug fix for crash problem when providing fractional value to setCompCount()
and setPatchCount() in tetexact

Version 3.0.0 (2017-2)
==========================
1. Implement parallel TetOpSplit solver.
2. Change compilation system to CMake.

Version 2.2.1 (2014-10)
==========================
1. Bug fixes for CUBIT-STEPS geometry preparation toolkit.
2. Implement directional diffusion constant.

Version 2.2.0 (2014-04-16)
==========================
1. Add CUBIT-STEPS geometry preparation toolkit and visualization toolkit.
2. Add "Region of Interest" dataset in Tetmesh Geometry and related functions in Tetexact solver
3. Add direct NumPy access functions in Tetmesh geometry and Tetexact solver

Version 2.1.0 (2013-09-05)
==========================
1. Add steps.geom.castToTmComp and steps.geom.castToTmPatch methods for casting Comp and Patch
objects to their TetMesh counterpart (if possible).
2. Added direct connection between TetODE and E-Field. Previously connection was possible
only through the python interface.
3. Other small additions such as optional percentage of starting nodes tested for mesh breadth
first search in E-Field setup.

Version 2.0.0 (2013-04-22)
==========================
1. First version to include E-Field and related objects. Allows
simulation of the potential across a membrane specified as a collection of
triangles comprising a surface in the tetrahedral mesh. See documentation for
more information.
2. Addition of solver TetODE for spatial deterministic simulations. Uses the CVODE
library for solutions.
3. Addition of surface diffusion in mesh-based solvers (Tetexact and TetODE), which models a
diffusive flux between triangles that form part of a patch surface, analogous to
volume diffusion between tetrahedral elements in a compartment.
4. Several other smaller additions and fixes.

Version 1.3.0 (2011-12)
==========================
1. SSA engine changes from Direct SSA to Composition & Rejection SSA for Tetexact solver.

Version 1.2.1 (2011-06-10)
==========================
1. Important bugfix in Tetexact to fix a problem that was causing
crashes during construction.

Version 1.2.0 (2011-04-13)
==========================
1. SBML support in steps/utilities/sbml.py
2. Addition of Diffusion Boundary object, which allows for optional
chemical diffusion between connected compartments.
3. Initial conditions improvement for Tetexact solver, resulting
in a  uniform initial spatial concentration across the
compartment (or patch) regardless of tetrahedron volumes (or triangle
areas), important where tetrahedrons vary considerably in volume.
Thanks to Haroon Anwar for bringing this issue to our attention.
4. Bugfix on Wmrk4 deterministic solver on the reset() function,
which previously did not reset counts in Patches.
5. Changed zero-order reaction constants to the conventional M/s.
Previously was /s.
6. Bugfix on surface-reactions. Now reaction constants for reactions
that involve reactants only on a patch (nothing in a volume) are
correctly scaled in 2D, as opposed to previous 3D scaling. Thanks
to Gabriela Antunes for pointing out this one.
7. Additions to examples folder to include scripts that link to the
user documentation, found in examples/tutorial.

Version 1.1.2 (2010-05-8)
==========================
1. Solve crash problem of Tetexact solver in some Linux system.
Binary packages of version 1.1.1 are not affected,
so only source package is released.

Version 1.1.1 (2010-04-7)
==========================
1. New setup.py to fix undefined symbol error in Linux
2. function name fix in utilities/visual.py

Version 1.1.0 (2010-03-20)
==========================
1. Replace GNU distribution system with Python Distutils.
2. Provide a visualization toolkit for mesh based simulation
(steps.utilitis.visual)
3. Provide checkpointing support for Wmdirect and Tetexact solvers.
(Please check user manual for more details)
4. New user manual and API reference

Version 1.0.1 (2010-01-28)
==========================
1. Bug fix of utilities.meshio.importTetgen() method crashing.
2. Changes to 'def' classes so that these classes no longer store
a pointer to model level objects or access data from those objects,
instead coping all data during construction and setup.
Also modified rng.py to be directly created by swig for
compatibility with python3.

Version 1.0.0 (2010-01-05)
==========================
First release.