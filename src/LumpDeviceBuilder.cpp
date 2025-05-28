// SPDX-FileCopyrightText: 2023-2025 OFDL Robotics Lab
// SPDX-FileCopyrightText: 2023-2025 HsiangYi Tsai <devilhyt@gmail.com>
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "LumpDeviceBuilder.h"

namespace LumpDeviceBuilder {

  LumpValueSpan::LumpValueSpan(float min, float max) : min{min}, max{max} {
    isValid = min <= max;
  }

  LumpValueSpan::LumpValueSpan(bool isExist) : isExist{isExist} {}

  LumpMode::LumpMode(
      const char *name,
      uint8_t dataType,
      uint8_t numData,
      uint8_t figures,
      uint8_t decimals,
      const char *symbol,
      LumpValueSpan raw,
      LumpValueSpan pct,
      LumpValueSpan si,
      uint8_t mapIn,
      uint8_t mapOut,
      bool power,
      bool flagsInName
  )
      : dataType{dataType},
        numData{numData},
        figures{figures},
        decimals{decimals},
        raw{raw},
        pct{pct},
        si{si},
        mapIn{mapIn},
        mapOut{mapOut},
        power{power},
        flagsInName{flagsInName} {
    using namespace LumpDeviceBuilder::Internal;

    if (name && flagsInName) {
      memcpy(this->name, name, LUMP_MAX_SHORT_NAME_SIZE + 7);
    } else if (name && strlen(name) > 0 && isalpha(name[0])) {
      size_t nameLen = min(strlen(name), static_cast<size_t>(LUMP_MAX_NAME_SIZE));
      strncpy(this->name, name, nameLen);
    }

    if (symbol && strlen(symbol) > 0) {
      size_t symbolLen = min(strlen(symbol), static_cast<size_t>(LUMP_MAX_UOM_SIZE));
      strncpy(this->symbol, symbol, symbolLen);
    }

    dataTypeSize = sizeOfLumpDataType(dataType);
    dataMsgSize  = numData * dataTypeSize;

    if (mapOut != LUMP_INFO_MAPPING_NONE) {
      dataMsg = calloc(numData, dataTypeSize);
    }
  }

  LumpMode::~LumpMode() {
    if (dataMsg) {
      free(dataMsg);
      dataMsg = nullptr;
    }
  }

  LumpMode::LumpMode(const LumpMode &other)
      : dataType{other.dataType},
        numData{other.numData},
        figures{other.figures},
        decimals{other.decimals},
        raw{other.raw},
        pct{other.pct},
        si{other.si},
        mapIn{other.mapIn},
        mapOut{other.mapOut},
        power{other.power},
        flagsInName{other.flagsInName},
        dataTypeSize{other.dataTypeSize},
        dataMsgSize{other.dataMsgSize},
        hasDataMsg{other.hasDataMsg} {
    memcpy(name, other.name, sizeof(name));
    memcpy(symbol, other.symbol, sizeof(symbol));

    if (mapOut != LUMP_INFO_MAPPING_NONE) {
      dataMsg = malloc(dataMsgSize);
      memcpy(dataMsg, other.dataMsg, dataMsgSize);
    }
  }

  LumpMode &LumpMode::operator=(const LumpMode &other) {
    if (this != &other) {
      if (dataMsg) {
        free(dataMsg);
        dataMsg = nullptr;
      }

      dataType     = other.dataType;
      numData      = other.numData;
      figures      = other.figures;
      decimals     = other.decimals;
      raw          = other.raw;
      pct          = other.pct;
      si           = other.si;
      mapIn        = other.mapIn;
      mapOut       = other.mapOut;
      power        = other.power;
      flagsInName  = other.flagsInName;
      dataTypeSize = other.dataTypeSize;
      dataMsgSize  = other.dataMsgSize;
      hasDataMsg   = other.hasDataMsg;

      memcpy(name, other.name, sizeof(name));
      memcpy(symbol, other.symbol, sizeof(symbol));

      if (mapOut != LUMP_INFO_MAPPING_NONE) {
        dataMsg = malloc(dataMsgSize);
        memcpy(dataMsg, other.dataMsg, dataMsgSize);
      }
    }
    return *this;
  }

  LumpMode::LumpMode(LumpMode &&other) noexcept
      : dataType{other.dataType},
        numData{other.numData},
        figures{other.figures},
        decimals{other.decimals},
        raw{other.raw},
        pct{other.pct},
        si{other.si},
        mapIn{other.mapIn},
        mapOut{other.mapOut},
        power{other.power},
        flagsInName{other.flagsInName},
        dataTypeSize{other.dataTypeSize},
        dataMsgSize{other.dataMsgSize},
        hasDataMsg{other.hasDataMsg} {
    memcpy(name, other.name, sizeof(name));
    memcpy(symbol, other.symbol, sizeof(symbol));

    dataMsg       = other.dataMsg;
    other.dataMsg = nullptr;
  }

  LumpMode &LumpMode::operator=(LumpMode &&other) noexcept {
    if (this != &other) {
      if (dataMsg) {
        free(dataMsg);
        dataMsg = nullptr;
      }

      dataType     = other.dataType;
      numData      = other.numData;
      figures      = other.figures;
      decimals     = other.decimals;
      raw          = other.raw;
      pct          = other.pct;
      si           = other.si;
      mapIn        = other.mapIn;
      mapOut       = other.mapOut;
      power        = other.power;
      flagsInName  = other.flagsInName;
      dataTypeSize = other.dataTypeSize;
      dataMsgSize  = other.dataMsgSize;
      hasDataMsg   = other.hasDataMsg;

      memcpy(name, other.name, sizeof(name));
      memcpy(symbol, other.symbol, sizeof(symbol));

      dataMsg       = other.dataMsg;
      other.dataMsg = nullptr;
    }
    return *this;
  }

} // namespace LumpDeviceBuilder

namespace LumpDeviceBuilder::Internal {

  uint8_t sizeOfLumpDataType(uint8_t dataType) {
    switch (dataType) {
      case LUMP_DATA_TYPE_DATA8:
        return 1;
      case LUMP_DATA_TYPE_DATA16:
        return 2;
      case LUMP_DATA_TYPE_DATA32:
      case LUMP_DATA_TYPE_DATAF:
        return 4;
      default:
        return 0;
    }
  }

  uint32_t versionToBcd(uint32_t version) {
    uint32_t bcd  = 0;
    uint8_t shift = 0;
    while (version) {
      bcd |= (version % 10) << shift;
      version /= 10;
      shift += 4;
    }
    return bcd;
  }

  uint8_t calcChecksum(uint8_t *msg, uint8_t size) {
    uint8_t checksum{0xff};
    while (size--) {
      checksum ^= *msg++;
    }
    return checksum;
  }

  uint8_t queryLog2(uint8_t x) {
    switch (x) {
      case 1:
        return 0;
      case 2:
        return 1;
      case 4:
        return 2;
      case 8:
        return 3;
      case 16:
        return 4;
      case 32:
        return 5;
      default:
        return 255; // error
    }
  }

  uint8_t queryNextPow2(uint8_t x) {
    if (x == 0 || x == 1 || x == 2)
      return x;
    else if (x <= 4)
      return 4;
    else if (x <= 8)
      return 8;
    else if (x <= 16)
      return 16;
    else if (x <= 32)
      return 32;
    else
      return 255; // error
  }

} // namespace LumpDeviceBuilder::Internal
