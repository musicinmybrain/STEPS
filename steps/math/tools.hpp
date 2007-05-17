////////////////////////////////////////////////////////////////////////////////
// STEPS - STochastic Engine for Pathway Simulation
// Copyright (C) 2005-2007 Stefan Wils. All rights reserved.
//
// $Id$
////////////////////////////////////////////////////////////////////////////////

#ifndef STEPS_MATH_TOOLS_HPP
#define STEPS_MATH_TOOLS_HPP 1

// Autotools definitions.
#ifdef HAVE_CONFIG_H
#include <steps/config.h>
#endif

// Standard library & STL headers.
#include <cmath>
#include <cstdlib>
#include <vector>

// Boost headers.
//#include <steps/boost/scoped_ptr.hpp>
//#include <steps/boost/shared_ptr.hpp>
//#include <steps/boost/weak_ptr.hpp>

// STEPS headers.
#include <steps/common.h>
#include <steps/math/constants.hpp>

START_NAMESPACE(steps)
START_NAMESPACE(math)

////////////////////////////////////////////////////////////////////////////////

inline bool isLargerEps(float r)
{
    if (fabsf(r) > static_cast<float>(IEEE_EPSILON32)) return true;
    return false;
}

inline bool isLargerEps(double r)
{
    if (fabs(r) > static_cast<double>(IEEE_EPSILON64)) return true;
    return false;
}

////////////////////////////////////////////////////////////////////////////////

inline bool isSmallerEps(float r)
{
    if (fabsf(r) <= static_cast<float>(IEEE_EPSILON32)) return true;
    return false;
}

inline bool isSmallerEps(double r)
{
    if (fabs(r) <= static_cast<double>(IEEE_EPSILON64)) return true;
    return false;
}

////////////////////////////////////////////////////////////////////////////////

inline float getSysRand(float min, float max)
{
    return min + ((max - min) *
        static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
}

inline double getSysRand(double min, double max)
{
    return min + ((max - min) *
        static_cast<double>(rand()) / static_cast<double>(RAND_MAX));
}

inline int getSysRand(int min, int max)
{
    return static_cast<int>(floorf(0.5 + getSysRand(static_cast<double>(min),
                                              static_cast<double>(max))));
}

////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline T min(T const & v1, T const & v2)
{
    return (v1 < v2 ? v1 : v2);
}

template<typename T>
inline T min(T const & v1, T const & v2, T const & v3)
{
    return min(v1, min(v2, v3));
}

template<typename T>
inline T min(T const & v1, T const & v2, T const & v3, T const & v4)
{
    return min(min(v1, v2), min(v3, v4));
}

////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline T max(T const & v1, T const & v2)
{
    return (v1 > v2 ? v1 : v2);
}

template<typename T>
inline T max(T const & v1, T const & v2, T const & v3)
{
    return max(v1, max(v2, v3));
}

template<typename T>
inline T max(T const & v1, T const & v2, T const & v3, T const & v4)
{
    return max(max(v1, v2), max(v3, v4));
}

////////////////////////////////////////////////////////////////////////////////

// Transfers sign of argument sign to argument num.
template<typename T>
inline T sign(T const & num, T const & sign)
{
	if (((sign > 0) && (num < 0)) || ((sign < 0) && (num > 0)))
    	return -num;
	else 
        return num;
}

////////////////////////////////////////////////////////////////////////////////

STEPS_EXTERN 
void setSysRandInitTime(void);

////////////////////////////////////////////////////////////////////////////////

inline float xformDegToRad(float deg)
{
    return (deg * static_cast<float>(PI)) / 180.0f;
}

inline double xformDegToRad(double deg)
{
    return (deg * static_cast<double>(PI)) / 180.0;
}

////////////////////////////////////////////////////////////////////////////////

inline float xformRadToDeg(float rad)
{
    return (rad * 180.0f) / static_cast<float>(PI);
}

inline double xformRadToDeg(double rad)
{
    return (rad * 180.0) / static_cast<double>(PI);
}

////////////////////////////////////////////////////////////////////////////////

END_NAMESPACE(math)
END_NAMESPACE(steps)

#endif
// STEPS_MATH_TOOLS_HPP

// END