#include "application.h"

int WinMain() {
  // A simple usage example.
  Application application(u8"Green Eye 🌱");

  RUN(application, {
    // This is the application main loop.
    // All application manupilation must go here.

    // Set uv index to 6.0 and disable the uv lights.
    application.states.SetUVIndex(6.0f, application.states.Disabled);

    // Retreives the most recent co2 reading.
    float co2 = application.states.GetCO2()[0];
  })
}
