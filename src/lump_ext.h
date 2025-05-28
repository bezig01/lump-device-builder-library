// SPDX-FileCopyrightText: 2023-2025 OFDL Robotics Lab
// SPDX-FileCopyrightText: 2023-2025 HsiangYi Tsai <devilhyt@gmail.com>
// SPDX-License-Identifier: MIT

/**
 * Extended version of LUMP (`lump.h`)
 *
 * This header file extends LUMP (`lump.h`) with additional definitions.
 */

#ifndef LUMP_EXT_H
#define LUMP_EXT_H

#include "lump.h"

/**
 * Mode mapping flags.
 */
typedef enum {
  /** No flags are set. */
  LUMP_INFO_MAPPING_NONE = 0,

  /** N/A 0. */
  LUMP_INFO_MAPPING_NA0 = 1,

  /** N/A 1. */
  LUMP_INFO_MAPPING_NA1 = 1 << 1,

  /** DIS (Discrete [0, 1, 2, 3]). */
  LUMP_INFO_MAPPING_DIS = 1 << 2,

  /** REL (Relative [-1..1]). */
  LUMP_INFO_MAPPING_REL = 1 << 3,

  /** ABS (Absolute [min..max]). */
  LUMP_INFO_MAPPING_ABS = 1 << 4,

  /** N/A 5. */
  LUMP_INFO_MAPPING_NA5 = 1 << 5,

  /** Supports Functional Mapping 2.0+. */
  LUMP_INFO_MAPPING_SUPPORT_FUNCTIONAL_MAPPING_2 = 1 << 6,

  /** Supports NULL value. */
  LUMP_INFO_MAPPING_SUPPORT_NULL = 1 << 7,
} lump_info_mapping_t;

/**
 * Short alias for LUMP data types.
 */
typedef enum {
  /** 8-bit signed integer. */
  DATA8 = LUMP_DATA_TYPE_DATA8,

  /** little-endian 16-bit signed integer. */
  DATA16 = LUMP_DATA_TYPE_DATA16,

  /** little-endian 32-bit signed integer. */
  DATA32 = LUMP_DATA_TYPE_DATA32,

  /** little-endian 32-bit floating point. */
  DATAF = LUMP_DATA_TYPE_DATAF,
} lump_data_type_short_t;

/* Timeout thresholds (milliseconds) */
#ifndef LUMP_AUTO_ID_DELAY
  #define LUMP_AUTO_ID_DELAY 500
#endif
#ifndef LUMP_ACK_TIMEOUT
  #define LUMP_ACK_TIMEOUT 80
#endif
#ifndef LUMP_NACK_TIMEOUT
  #define LUMP_NACK_TIMEOUT 1500 // LUMP NACK timeout thresholds (milliseconds).
#endif
#define LUMP_INTER_MODE_PAUSE 10
#define LUMP_UART_INIT_DELAY  5

/* UART settings */
#define LUMP_UART_BUFFER_SIZE LUMP_MAX_MSG_SIZE + 3
#define LUMP_UART_SPEED_MIN   2400
#define LUMP_UART_SPEED_MID   57600
#define LUMP_UART_SPEED_LPF2  115200
#define LUMP_UART_SPEED_MAX   460800

/* Message */
#define LUMP_MSG_SIZE_SHIFT 3 // Bit shift for LUMP message size.

/* View */
#define LUMP_VIEW_ALL 255 // Shows all modes in view and data log.

/* LUMP_CMD_EXT_MODE payload */
#define LUMP_EXT_MODE_0 0x0 // mode is < 8.
#define LUMP_EXT_MODE_8 0x8 // mode is >= 8.

/**
 *  Macro to convert a mode number to an INFO_MODE value.
 *
 *  @param m Mode number.
 *  @return INFO_MODE value.
 */
#define LUMP_INFO_MODE(m) (m > LUMP_MAX_MODE ? LUMP_INFO_MODE_PLUS_8 : 0x0)

#endif // LUMP_EXT_H