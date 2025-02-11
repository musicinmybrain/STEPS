/*
#################################################################################
#
#    STEPS - STochastic Engine for Pathway Simulation
#    Copyright (C) 2007-2021 Okinawa Institute of Science and Technology, Japan.
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

#ifndef STEPS_ERROR_HPP
#define STEPS_ERROR_HPP 1

// Standard library & STL headers.
#include <string>
#include <exception>

// STEPS headers.
#include "steps/common.h"

#ifdef ENABLE_ASSERTLOG
#define AssertLog(condition) if (! (condition)) \
CLOG(ERROR, "general_log") << "Assertion failed, please send the log files under .logs/ to developer.", \
throw steps::AssertErr("Assertion failed, please send the log files under .logs/ to developer.")
#else
#define AssertLog(condition) {}
#endif

#define ErrLog(msg) \
CLOG(ERROR, "general_log") << std::string("GeneralErr: ") + (msg), \
throw steps::Err(std::string("GeneralErr: ") + (msg))


#define NotImplErrLog(msg) \
CLOG(ERROR, "general_log") << std::string("NotImplErr: ") + (msg), \
throw steps::NotImplErr(std::string("NotImplErr: ") + (msg))


#define ArgErrLog(msg) \
CLOG(ERROR, "general_log") << std::string("ArgErr: ") + (msg), \
throw steps::ArgErr( std::string("ArgErr: ") + (msg))


#define ProgErrLog(msg) \
CLOG(ERROR, "general_log") << std::string("ProgErr: ") + (msg), \
throw steps::ProgErr(std::string("ProgErr: ") + (msg))


#define SysErrLog(msg) \
CLOG(ERROR, "general_log") << std::string("SysErr: ") + (msg), \
throw steps::SysErr(std::string("SysErr: ") + (msg))


#define IOErrLog(msg) \
CLOG(ERROR, "general_log") << std::string("IOErr: ") + (msg), \
throw steps::IOErr(std::string("IOErr: ") + (msg))


namespace steps {

////////////////////////////////////////////////////////////////////////////////

/// Base STEPS exception class. All 'real' exceptions are derived from this.
///
struct Err: public std::exception
{
    Err(std::string const & msg = ""): pMessage(msg) { }

    const char * getMsg() const noexcept;
    const char * what() const noexcept override { return getMsg(); }

private:
    std::string                 pMessage;
};

////////////////////////////////////////////////////////////////////////////////

struct AssertErr: public Err
{
    AssertErr(std::string const & msg = ""): Err(msg) { }
};

////////////////////////////////////////////////////////////////////////////////

/// This exception gets thrown when some STEPS interface function is not
/// implemented, usually in a solver module. This can be because the
/// function does not make sense for that particular solver, or because
/// the programmer took a shortcut...
///
struct NotImplErr: public Err
{
    NotImplErr(std::string const & msg = ""): Err(msg) { }
};

////////////////////////////////////////////////////////////////////////////////

/// This exception usually gets thrown when some caller-provided argument
/// doesn't make sense immediately. This 'caller' should be restricted
/// to the actual user as much as possible.
///
struct ArgErr: public Err
{
    ArgErr(std::string const & msg = ""): Err(msg) { }
};

////////////////////////////////////////////////////////////////////////////////

/// This exception gets thrown whenever the program knows it has failed,
/// in other words not because of some mistake by the user, but e.g.
/// because of some hard coded limit (running out of index numbers,...)
///
/// In principle, it should be only be thrown in absurdly rare cases.
///
struct ProgErr: public Err
{
    ProgErr(std::string const & msg = ""): Err(msg) { }
};

////////////////////////////////////////////////////////////////////////////////

/// Generic base class for any system-induced error. These exceptions signal
/// truly unexpected situations, such as out-of-memory or I/O problems.
///
struct SysErr: public Err
{
    SysErr(std::string const & msg = ""): Err(msg) { }
};

////////////////////////////////////////////////////////////////////////////////

/// Gets thrown whenever there is a system failure involving I/O, such
/// as dealing with files.
///
struct IOErr
: public SysErr
{
    IOErr(std::string const & msg = ""): SysErr(msg) { }
};

////////////////////////////////////////////////////////////////////////////////

}

#endif
// STEPS_ERROR_HPP

// END
