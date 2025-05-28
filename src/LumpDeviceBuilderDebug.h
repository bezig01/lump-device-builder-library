// SPDX-FileCopyrightText: 2023-2025 OFDL Robotics Lab
// SPDX-FileCopyrightText: 2023-2025 HsiangYi Tsai <devilhyt@gmail.com>
// SPDX-License-Identifier: MIT

/**
 * Debugging settings for LUMP Device Builder Library
 *
 * This header file contains debugging settings for the LUMP Device Builder Library.
 */

#ifndef LUMP_DEVICE_BUILDER_DEBUG_H
#define LUMP_DEVICE_BUILDER_DEBUG_H

#ifndef LUMP_DEBUG_SPEED
  #define LUMP_DEBUG_SPEED 115200
#endif

#ifdef LUMP_DEBUG_SERIAL
  #define LUMP_DEBUG_BEGIN(...)   LUMP_DEBUG_SERIAL.begin(__VA_ARGS__)
  #define LUMP_DEBUG_END(...)     LUMP_DEBUG_SERIAL.end(__VA_ARGS__)
  #define LUMP_DEBUG_WRITE(...)   LUMP_DEBUG_SERIAL.write(__VA_ARGS__)
  #define LUMP_DEBUG_PRINT(...)   LUMP_DEBUG_SERIAL.print(__VA_ARGS__)
  #define LUMP_DEBUG_PRINTLN(...) LUMP_DEBUG_SERIAL.println(__VA_ARGS__)
  #define LUMP_DEBUG_PRINT_RX_BUFFER(buffer, size) \
    LUMP_DEBUG_PRINT("[RX] ");                     \
    for (uint8_t i = 0; i < size; ++i) {           \
      LUMP_DEBUG_PRINT(buffer[i], HEX);            \
      LUMP_DEBUG_PRINT(" ");                       \
    }
#else
  #define LUMP_DEBUG_BEGIN(...)           ((void)0)
  #define LUMP_DEBUG_END(...)             ((void)0)
  #define LUMP_DEBUG_WRITE(...)           ((void)0)
  #define LUMP_DEBUG_PRINT(...)           ((void)0)
  #define LUMP_DEBUG_PRINTLN(...)         ((void)0)
  #define LUMP_DEBUG_PRINT_RX_BUFFER(...) ((void)0)
#endif

#endif // LUMP_DEVICE_BUILDER_DEBUG_H