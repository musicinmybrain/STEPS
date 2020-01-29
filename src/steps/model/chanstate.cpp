/*
 #################################################################################
#
#    STEPS - STochastic Engine for Pathway Simulation
#    Copyright (C) 2007-2020 Okinawa Institute of Science and Technology, Japan.
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

// Autotools definitions.
#ifdef HAVE_CONFIG_H
#include <steps/config.h>
#endif

// STL headers.
#include <cassert>
#include <sstream>
#include <string>

// STEPS headers.
#include "steps/common.h"
#include "steps/error.hpp"
#include "steps/model/chan.hpp"
#include "steps/model/chanstate.hpp"
#include "steps/model/spec.hpp"

// logging
#include "easylogging++.h"

////////////////////////////////////////////////////////////////////////////////

using namespace std;
using namespace steps::model;

////////////////////////////////////////////////////////////////////////////////

ChanState::ChanState(string const & id, Model * model, Chan * chan)
: Spec(id, model)
, pChan(chan)
{
    if (pChan == nullptr)
    {
        ostringstream os;
        os << "No channel provided to ChanState initializer function";
        ArgErrLog(os.str());
    }
    if (model != chan->getModel())
    {
        ostringstream os;
        os << "Channel is unknown in this model.";
        ArgErrLog(os.str());
    }

    pChan->_handleChanStateAdd(this);
}

////////////////////////////////////////////////////////////////////////////////

ChanState::~ChanState()
{
    if (pChan == nullptr) { return;
}
    _handleSelfDelete();
}

////////////////////////////////////////////////////////////////////////////////

void ChanState::_handleSelfDelete()
{
    // Base method
    Spec::_handleSelfDelete();

    pChan->_handleChanStateDel(this);
    pChan = nullptr;
}

////////////////////////////////////////////////////////////////////////////////

void ChanState::setID(string const & id)
{
    AssertLog(pChan != nullptr);
    if (id == getID()) return;
    // The following might raise an exception, e.g. if the new ID is not
    // valid or not unique. If this happens, we don't catch but simply let
    // it pass by into the Python layer.
    pChan->_handleChanStateIDChange(getID(), id);
    // This line will only be executed if the previous call didn't raise
    // an exception.
    Spec::setID(id);
}

////////////////////////////////////////////////////////////////////////////////

// END
