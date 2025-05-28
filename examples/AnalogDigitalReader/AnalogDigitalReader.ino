// SPDX-FileCopyrightText: 2023-2025 OFDL Robotics Lab
// SPDX-FileCopyrightText: 2023-2025 HsiangYi Tsai <devilhyt@gmail.com>
// SPDX-License-Identifier: MIT

/**
 * Analog / Digital Reader Example
 *
 * This is the source code for the Quickstart guide of the LUMP Device Builder Library.
 * https://github.com/devilhyt/lump-device-builder-library#quickstart
 */

// Select a serial interface for device communication.
#define DEVICE_SERIAL Serial0
#define RX_PIN        20
#define TX_PIN        21

// Pin definitions.
#define ANALOG_PIN  3
#define DIGITAL_PIN 4

#include <LumpDeviceBuilder.h>

// Define the supported modes for the device.
LumpMode modes[]{
    {"Analog", DATA16, 1, 4, 0, "raw", {0, 4095}, {0, 100}, {0, 4095}},
    {"Digital", DATA8, 1, 1, 0, "raw", {0, 1}, {0, 100}, {0, 1}}
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
      // Initialize the mode.
      switch (mode) {
        case 0:
          pinMode(ANALOG_PIN, INPUT);
          break;
        case 1:
          pinMode(DIGITAL_PIN, INPUT);
          break;

        default:
          break;
      }
      break;

    case LumpDeviceState::Communicating: {
      static uint16_t value0     = 0;
      static uint8_t value1      = 0;
      static uint32_t period     = 5; // 200Hz
      static uint32_t prevMillis = 0;
      uint32_t currentMillis     = millis();

      // Send data to the host.
      if (device.hasNack() || (currentMillis - prevMillis > period)) {
        switch (mode) {
          case 0:
            value0 = analogRead(ANALOG_PIN);
            device.send(value0);
            break;

          case 1:
            value1 = digitalRead(DIGITAL_PIN);
            device.send(value1);
            break;

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