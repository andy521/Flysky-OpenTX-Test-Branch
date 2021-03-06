/*
 * Copyright (C) OpenTX
 *
 * Based on code named
 *   th9x - http://code.google.com/p/th9x
 *   er9x - http://code.google.com/p/er9x
 *   gruvin9x - http://code.google.com/p/gruvin9x
 *
 * License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _MODULES_H_
#define _MODULES_H_

#include "myeeprom.h"

#define CROSSFIRE_CHANNELS_COUNT        16

#if defined(MULTIMODULE)
inline bool isModuleMultimodule(uint8_t idx)
{
  return idx == EXTERNAL_MODULE && g_model.moduleData[EXTERNAL_MODULE].type == MODULE_TYPE_MULTIMODULE;
}

inline bool isModuleMultimoduleDSM2(uint8_t idx)
{
  return isModuleMultimodule(idx) && g_model.moduleData[idx].getMultiProtocol(true) == MM_RF_PROTO_DSM2;
}
#else
inline bool isModuleMultimodule(uint8_t)
{
  return false;
}

inline bool isModuleMultimoduleDSM2(uint8_t)
{
  return false;
}
#endif

#if defined(PCBFLYSKY)
inline bool isModuleFlysky(uint8_t idx)
{
  return g_model.moduleData[idx].type == MODULE_TYPE_FLYSKY;
}
#else
inline bool isModuleFlysky(uint8_t idx)
{
  return idx == EXTERNAL_MODULE && g_model.moduleData[EXTERNAL_MODULE].type == MODULE_TYPE_FLYSKY;
}
#endif

#if defined(PCBFRSKY)
inline bool isModuleXJT(uint8_t idx)
{
  return g_model.moduleData[idx].type == MODULE_TYPE_XJT;
}
#else
inline bool isModuleXJT(uint8_t idx)
{
  return idx == EXTERNAL_MODULE && g_model.moduleData[EXTERNAL_MODULE].type == MODULE_TYPE_XJT;
}
#endif

#if defined(CROSSFIRE)
inline bool isModuleCrossfire(uint8_t idx)
{
  return idx == EXTERNAL_MODULE && g_model.moduleData[EXTERNAL_MODULE].type == MODULE_TYPE_CROSSFIRE;
}
#else
inline bool isModuleCrossfire(uint8_t idx)
{
  return false;
}
#endif

#if defined(EXTRA_MODULE)
inline bool isExtraModule(uint8_t idx)
{
  return idx == EXTRA_MODULE;
}
#else
inline bool isExtraModule(uint8_t)
{
  return false;
}
#endif

#if defined(TARANIS_INTERNAL_PPM)
inline bool isModulePPM(uint8_t idx)
{
  return idx == TRAINER_MODULE ||
         (idx == INTERNAL_MODULE && g_model.moduleData[INTERNAL_MODULE].type == MODULE_TYPE_PPM) ||
         (idx == EXTERNAL_MODULE && g_model.moduleData[EXTERNAL_MODULE].type == MODULE_TYPE_PPM));
}
#else
inline bool isModulePPM(uint8_t idx)
{
  return idx == TRAINER_MODULE ||
         isExtraModule(idx) ||
         (idx == EXTERNAL_MODULE && g_model.moduleData[EXTERNAL_MODULE].type == MODULE_TYPE_PPM);
}
#endif

inline bool isModuleR9M(uint8_t idx)
{
  return g_model.moduleData[idx].type == MODULE_TYPE_R9M;
}

inline bool isModuleR9M_FCC(uint8_t idx)
{
  return isModuleR9M(idx) && g_model.moduleData[idx].subType == MODULE_SUBTYPE_R9M_FCC;
}

inline bool isModuleR9M_LBT(uint8_t idx)
{
  return isModuleR9M(idx) && g_model.moduleData[idx].subType == MODULE_SUBTYPE_R9M_LBT;
}

inline bool isModulePXX(uint8_t idx)
{
  return isModuleXJT(idx) || isModuleR9M(idx);
}

#if defined(DSM2)
inline bool isModuleDSM2(uint8_t idx)
{
  return idx == EXTERNAL_MODULE && g_model.moduleData[EXTERNAL_MODULE].type == MODULE_TYPE_DSM2;
}

inline bool isModuleSBUS(uint8_t idx)
{
  return idx == EXTERNAL_MODULE && g_model.moduleData[EXTERNAL_MODULE].type == MODULE_TYPE_SBUS;
}
#else
inline bool isModuleDSM2(uint8_t idx)
{
  return false;
}
#endif

// order is the same as in enum Protocols in myeeprom.h (none, ppm, pxx, flysky, dsm, crossfire, multi, r9m, sbus)
static const int8_t maxChannelsModules_M8[] = {0, 8, 8, 6, -2, 8, 4, 8, 8}; // relative to 8!
static const int8_t maxChannelsXJT[] = {0, 8, 0, 4}; // relative to 8!

constexpr int8_t MAX_TRAINER_CHANNELS_M8 = MAX_TRAINER_CHANNELS - 8;
constexpr int8_t MAX_EXTRA_MODULE_CHANNELS_M8 = 8; // only 16ch PPM

inline int8_t maxModuleChannels_M8(uint8_t idx)
{
  if (isExtraModule(idx))
    return MAX_EXTRA_MODULE_CHANNELS_M8;
  else if (idx == TRAINER_MODULE)
    return MAX_TRAINER_CHANNELS_M8;
  else if (isModuleXJT(idx))
    return maxChannelsXJT[1 + g_model.moduleData[idx].rfProtocol];
  else
    return maxChannelsModules_M8[g_model.moduleData[idx].type];
}

inline int8_t maxModuleChannels(uint8_t idx)
{
  return 8 + maxModuleChannels_M8(idx);
}

inline int8_t minModuleChannels(uint8_t idx)
{
  if (isModuleCrossfire(idx))
    return 16;
  else
    return 1;
}

inline int8_t defaultModuleChannels_M8(uint8_t idx)
{
  if (isModulePPM(idx))
    return 0; // 8 channels
  else if (isModuleDSM2(idx))
    return 0; // 8 channels
  else if (isModuleMultimoduleDSM2(idx))
    return -1; // 7 channels
  else if (isModuleFlysky(idx))
    return 6; // 14 channels
  else
    return 8; // 16 channels
}

inline int8_t sentModuleChannels(uint8_t idx)
{
  if (isModuleCrossfire(idx))
    return CROSSFIRE_CHANNELS_COUNT;
  else if (isModuleMultimodule(idx) && !isModuleMultimoduleDSM2(idx))
    return 16;
  else
    return 8 + g_model.moduleData[idx].channelsCount;
}

inline bool isModuleTypeAllowed(uint8_t idx, uint8_t type)
{
#if defined(PCBFLYSKY)
  if (idx == INTERNAL_MODULE) {
    return (type == MODULE_TYPE_NONE || type == MODULE_TYPE_FLYSKY);
  }
  else if (idx == EXTERNAL_MODULE) {
    return (type == MODULE_TYPE_NONE || type == MODULE_TYPE_PPM
         || type == MODULE_TYPE_XJT || type == MODULE_TYPE_CROSSFIRE
         || type == MODULE_TYPE_R9M);
  }
#endif

  return true;
}

inline bool isModuleNeedingReceiverNumber(uint8_t idx)
{
  return isModulePXX(idx) || isModuleDSM2(idx) || isModuleMultimodule(idx);
}

inline bool isModuleNeedingBindRangeButtons(uint8_t idx)
{
  return isModulePXX(idx) || isModuleDSM2(idx) || isModuleMultimodule(idx) || isModuleFlysky(idx);
}

inline bool isModuleNeedingFailsafeButton(uint8_t idx)
{
  return isModulePXX(idx) || isModuleR9M(idx) || isModuleFlysky(idx);
}

#endif // _MODULES_H_
