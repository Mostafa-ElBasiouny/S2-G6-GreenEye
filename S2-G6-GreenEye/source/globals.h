#pragma once

#include "sensors/ph.h"
#include "sensors/co2.h"
#include "sensors/uv_index.h"
#include "sensors/temperature.h"
#include "sensors/air_humidity.h"
#include "sensors/air_precipitation.h"
#include "sensors/soil_moisture.h"
#include "sensors/soil_fertility.h"

#include "database.h"
#include "hmi/application.h"

#define INITIALIZE(OBJECT, NAME) OBJECT* NAME = new OBJECT(g_time_interval, g_database)
#define DELETE(NAME) delete NAME

const int g_time_interval = 1; // In seconds.

static Database* g_database = new Database("resources/GreenEye.data");
