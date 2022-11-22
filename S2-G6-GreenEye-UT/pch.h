// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here

#include <sensors/ph.h>
#include <sensors/co2.h>
#include <sensors/uv_index.h>
#include <sensors/temperature.h>
#include <sensors/air_humidity.h>
#include <sensors/precipitation.h>
#include <sensors/soil_moisture.h>
#include <sensors/soil_fertility.h>

#include <database.h>

#endif //PCH_H
