####################################################################################
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
###

import contextlib
import importlib.abc
import importlib.machinery
import sys
import types
import unittest

FAILED_LOAD_STR = 'unittest.loader._FailedTest'

FAKE_MODULE_NAME = 'steps'
FAKE_MODULE_ALL = ['NoOrdering']

class _FakeModule(types.ModuleType):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.__path__ = []
        self.__all__ = FAKE_MODULE_ALL

class _CustomVirtualLoader(importlib.abc.Loader):
    def create_module(self, spec):
        return _FakeModule(spec.name)

    def exec_module(self, module):
        pass

class _CustomMetaPathFinder(importlib.abc.MetaPathFinder):
    def __init__(self):
        self._virtualLoader = _CustomVirtualLoader()

    def find_spec(self, fullname, path, target=None):
        if fullname.startswith(FAKE_MODULE_NAME):
            return importlib.machinery.ModuleSpec(fullname, self._virtualLoader)
        return None

def getAllTests(suite):
    if isinstance(suite, unittest.TestCase):
        yield suite
    elif isinstance(suite, unittest.TestSuite):
        for test in suite:
            yield from getAllTests(test)
    else:
        raise NotImplementedError()

if __name__ == "__main__":
    _, path, prefix = sys.argv

    # Modify the import machinery to get a fake module.
    # We need to do this because unittest actually imports the test modules upon test discovery.
    # Since the test modules also import steps, we need to provide a fake steps package in order
    # for the test discovery to work.
    sys.meta_path.append(_CustomMetaPathFinder())

    loader = unittest.TestLoader()
    # Redirect stdout to avoid STEPS prints on module import
    with contextlib.redirect_stdout(None):
        suite = loader.discover(path, pattern=f'{prefix}*.py')

    failures = []
    successes = []
    for test in getAllTests(suite):
        if test.id().startswith(FAILED_LOAD_STR):
            failures.append(test.id()[len(FAILED_LOAD_STR)+1:])
        else:
            successes.append(test.id())

    print(';'.join(successes))
    if len(failures) > 0:
        print('The following tests could not be loaded properly:', file=sys.stderr)
        print('\n'.join(failures), file=sys.stderr)
        print('Corresponding errors:', file=sys.stderr)
        print('\n'.join(loader.errors), file=sys.stderr)
        sys.exit(1)

