// Copyright (c) 2023-2024 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#include <iostream>
#include <string>

#include "inih/ini_fbw.h"
#include "inih/ini_type_conversion.h"

#include "logging.h"

#include "FuelConfiguration_A339X.h"

void FuelConfiguration_A339X::loadConfigurationFromIni() {
  if (configFilename.empty()) {
    LOG_ERROR(
        "Fadec::FuelConfiguration_A339X: no configuration file specified -> using default fuel"
        " quantities. Use setConfigFilename() to set the configuration file)");
    return;
  }

  LOG_INFO("Fadec::FuelConfiguration_A339X: loading configuration file " + configFilename);

  mINI::INIStructure ini;
  mINI::INIFile      iniFile(configFilename);

  if (!iniFile.read(ini)) {
    LOG_ERROR("Fadec::FuelConfiguration_A339X: failed to read configuration file \"" + configFilename + "\" due to error \"" +
              strerror(errno) + "\" -> using default fuel quantities.");
    return;
  }

  fuelCenter   = mINI::INITypeConversion::getDouble(ini, INI_SECTION_FUEL, INI_SECTION_FUEL_CENTER_QUANTITY, fuelCenterDefault);
  fuelLeft     = mINI::INITypeConversion::getDouble(ini, INI_SECTION_FUEL, INI_SECTION_FUEL_LEFT_QUANTITY, fuelLeftDefault);
  fuelRight    = mINI::INITypeConversion::getDouble(ini, INI_SECTION_FUEL, INI_SECTION_FUEL_RIGHT_QUANTITY, fuelRightDefault);
  fuelLeftAux  = mINI::INITypeConversion::getDouble(ini, INI_SECTION_FUEL, INI_SECTION_FUEL_LEFT_AUX_QUANTITY, fuelLeftAuxDefault);
  fuelRightAux = mINI::INITypeConversion::getDouble(ini, INI_SECTION_FUEL, INI_SECTION_FUEL_RIGHT_AUX_QUANTITY, fuelRightAuxDefault);

  LOG_DEBUG("Fadec::FuelConfiguration_A339X: loaded fuel configuration from " + configFilename + " with the following values:");
  LOG_DEBUG("Fadec::FuelConfiguration_A339X: " + this->toString());
}

void FuelConfiguration_A339X::saveConfigurationToIni() {
  LOG_DEBUG("Fadec::FuelConfiguration_A339X: saving configuration file " + configFilename);

  mINI::INIStructure ini;
  mINI::INIFile      iniFile(configFilename);

  // Do not check a possible error since the file may not exist yet
  iniFile.read(ini);

  ini[INI_SECTION_FUEL][INI_SECTION_FUEL_CENTER_QUANTITY]    = std::to_string(this->fuelCenter);
  ini[INI_SECTION_FUEL][INI_SECTION_FUEL_LEFT_QUANTITY]      = std::to_string(this->fuelLeft);
  ini[INI_SECTION_FUEL][INI_SECTION_FUEL_RIGHT_QUANTITY]     = std::to_string(this->fuelRight);
  ini[INI_SECTION_FUEL][INI_SECTION_FUEL_LEFT_AUX_QUANTITY]  = std::to_string(this->fuelLeftAux);
  ini[INI_SECTION_FUEL][INI_SECTION_FUEL_RIGHT_AUX_QUANTITY] = std::to_string(this->fuelRightAux);

  if (!iniFile.write(ini, true)) {
    LOG_ERROR("Fadec::FuelConfiguration_A339X: failed to write engine conf " + configFilename + " due to error \"" + strerror(errno) +
              "\"");
    return;
  }

  LOG_DEBUG("Fadec::FuelConfiguration_A339X: saved fuel configuration to " + configFilename + " with the following values:");
  LOG_DEBUG("Fadec::FuelConfiguration_A339X: " + this->toString());
}

std::string FuelConfiguration_A339X::toString() const {
  std::ostringstream oss;
  oss << "FuelConfiguration_A339X: { "
      << "fuelCenter: " << fuelCenter        //
      << ", fuelLeft: " << fuelLeft          //
      << ", fuelRight: " << fuelRight        //
      << ", fuelLeftAux: " << fuelLeftAux    //
      << ", fuelRightAux: " << fuelRightAux  //
      << " }";
  return oss.str();
}
