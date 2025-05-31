// SPDX-FileCopyrightText: 2023-2025 OFDL Robotics Lab
// SPDX-FileCopyrightText: 2023-2025 HsiangYi Tsai <devilhyt@gmail.com>
// SPDX-License-Identifier: MIT

/**
 * Echo Mode Example
 *
 * This is the source code for the "Receive Data from the Host" guide of the LUMP Device Builder Library.
 * https://github.com/devilhyt/lump-device-builder-library-private/wiki/Advanced-Topics#receive-data-from-the-host
 */

// Select a serial interface for device communication.
#define DEVICE_SERIAL Serial0
#define RX_PIN        20
#define TX_PIN        21

#define NUM_DATA 2

#include <LumpDeviceBuilder.h>

// Define the supported modes for the device.
LumpMode modes[]{
    {"Echo", DATA16, NUM_DATA, 4, 0, "", {-1023, 1023}, {0, 100}, {-1023, 1023}, LUMP_INFO_MAPPING_NONE, LUMP_INFO_MAPPING_ABS}
};

uint8_t numModes = sizeof(modes) / sizeof(LumpMode);

// Instantiate the device.
LumpDevice<HardwareSerial> device(&DEVICE_SERIAL, RX_PIN, TX_PIN, 68, 115200, modes, numModes);

// Define actions for each mode.
void runDeviceModes() {
  // Get the device state and mode.
  auto state = device.state();
  auto mode  = device.mode();

  // Check the device state and perform actions accordingly.
  switch (state) {
    case LumpDeviceState::InitMode:
      break;

    case LumpDeviceState::Communicating: {
      static int16_t positive    = 0;
      static int16_t negative    = 0;
      static uint32_t period     = 5; // 200Hz
      static uint32_t prevMillis = 0;
      uint32_t currentMillis     = millis();

      // Read data from the host.
      switch (mode) {
        case 0: {
          if (device.hasDataMsg(mode)) {
            int16_t *data = device.readDataMsg<int16_t>(mode);
            positive = data[0];
            negative = data[1];
          }
          break;
        }

        default:
          break;
      }

      // Send data to the host.
      if (device.hasNack() || (currentMillis - prevMillis > period)) {
        switch (mode) {
          case 0: {
            int16_t data[] = {positive, negative};
            device.send(data, NUM_DATA);
            break;
          }

          default:
            break;
        }

        prevMillis = currentMillis;
      }
      break;
    }

    default:
      break;
  }
}

// Initialize the device and run it.
void setup() {
  device.begin();
}

void loop() {
  device.run();
  runDeviceModes();
}