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


// STL headers.
#include <cassert>
#include <iostream>
#include <sstream>

// STEPS headers.
#include "util/common.h"
#include "util/error.hpp"
#include "vertexconnection.hpp"
#include "vertexelement.hpp"

// logging
#include <easylogging++.h>
////////////////////////////////////////////////////////////////////////////////

namespace sefield = steps::solver::efield;
using namespace std;

////////////////////////////////////////////////////////////////////////////////

sefield::VertexConnection::VertexConnection(sefield::VertexElement * v1, sefield::VertexElement * v2)
: pVert1(v1)
, pVert2(v2)
, pGeomCC(0.0)
{
    AssertLog(v1 != nullptr);
    AssertLog(v2 != nullptr);
    pVert1->addConnection(this);
    pVert2->addConnection(this);
}

////////////////////////////////////////////////////////////////////////////////

sefield::VertexConnection::~VertexConnection()
= default;

////////////////////////////////////////////////////////////////////////////////

void sefield::VertexConnection::checkpoint(std::fstream & cp_file)
{
    cp_file.write(reinterpret_cast<char*>(&pGeomCC), sizeof(double));
}

////////////////////////////////////////////////////////////////////////////////

void sefield::VertexConnection::restore(std::fstream & cp_file)
{
    cp_file.read(reinterpret_cast<char*>(&pGeomCC), sizeof(double));
}

////////////////////////////////////////////////////////////////////////////////

sefield::VertexElement * sefield::VertexConnection::getOther(sefield::VertexElement * element)
{
    VertexElement * ret;
    if (pVert1 == element)
    {
        ret = pVert2;
    }
    else if (pVert2 == element)
    {
        ret = pVert1;
    }
    else
    {
        AssertLog(false);
    }
    return ret;
}

////////////////////////////////////////////////////////////////////////////////

//bool VertexConnection::hasInternalEnd()
//{
//    return (vea->isInternal() || veb->isInternal());
//}

////////////////////////////////////////////////////////////////////////////////

//bool VertexConnection::isEdge()
//{
//    return (vea->isEdge() && veb->isEdge());
//}

////////////////////////////////////////////////////////////////////////////////

// END
