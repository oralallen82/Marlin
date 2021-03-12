/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include "DGUSDisplay.h"
#include "../dgus/DGUSVPVariable.h"

#include "../../../../inc/MarlinConfig.h"

enum DGUSLCD_Screens : uint8_t;

struct creality_dwin_settings_t {
  size_t settings_size;
  bool led_state;
  bool display_standby;
  bool display_sound;
  int16_t standby_screen_brightness;
};

class DGUSScreenHandler {
public:
  DGUSScreenHandler() = default;

  static bool loop();

  static void DefaultSettings();
  static void LoadSettings(const char* buff);
  static void StoreSettings(char* buff);
  static void SetTouchScreenConfiguration();
  static void KillScreenCalled();

  static void OnPowerlossResume();

  /// Send all 4 strings that are displayed on the infoscreen, confirmation screen and kill screen
  /// The bools specifing whether the strings are in RAM or FLASH.
  static void sendinfoscreen(const char* line1, const char* line2, const char* line3, const char* line4, bool l1inflash, bool l2inflash, bool l3inflash, bool liinflash);

  static void HandleUserConfirmationPopUp(uint16_t ConfirmVP, const char* line1, const char* line2, const char* line3, const char* line4, bool l1inflash, bool l2inflash, bool l3inflash, bool liinflash);

  /// "M117" Message -- msg is a RAM ptr.
  static void setstatusmessage(const char* msg);
  /// The same for messages from Flash
  static void setstatusmessagePGM(PGM_P const msg);
  // Callback for VP "Display wants to change screen on idle printer"
  static void ScreenChangeHookIfIdle(DGUS_VP_Variable &var, void *val_ptr);
  // Callback for VP "Screen has been changed"
  static void ScreenChangeHook(DGUS_VP_Variable &var, void *val_ptr);
  // Callback for VP "All Heaters Off"
  static void HandleAllHeatersOff(DGUS_VP_Variable &var, void *val_ptr);
  // Hook for "Change this temperature"
  static void HandleTemperatureChanged(DGUS_VP_Variable &var, void *val_ptr);
  static void HandleFanSpeedChanged(DGUS_VP_Variable &var, void *val_ptr);
  // Hook for "Change Flowrate"
  static void HandleFlowRateChanged(DGUS_VP_Variable &var, void *val_ptr);
  // Hook for manual extrude.
  static void HandleManualExtrude(DGUS_VP_Variable &var, void *val_ptr);
  // Hook for motor lock and unlook
  static void HandleMotorLockUnlock(DGUS_VP_Variable &var, void *val_ptr);
  #if ENABLED(POWER_LOSS_RECOVERY)
    // Hook for power loss recovery.
    static void HandlePowerLossRecovery(DGUS_VP_Variable &var, void *val_ptr);
  #endif
  // Hook for settings
  static void HandleSettings(DGUS_VP_Variable &var, void *val_ptr);
  static void HandleStepPerMMChanged(DGUS_VP_Variable &var, void *val_ptr);
  static void HandleStepPerMMExtruderChanged(DGUS_VP_Variable &var, void *val_ptr);
  static void HandleFeedAmountChanged(DGUS_VP_Variable &var, void *val_ptr);

  // Hook for move to position
  static void HandlePositionChange(DGUS_VP_Variable &var, void *val_ptr);

  static void HandleToggleTouchScreenMute(DGUS_VP_Variable &var, void *val_ptr);
  static void HandleToggleTouchScreenStandbySetting(DGUS_VP_Variable &var, void *val_ptr);
  static void HandleTouchScreenStandbyBrightnessSetting(DGUS_VP_Variable &var, void *val_ptr);

  #if HAS_PID_HEATING
    // Hook for "Change this temperature PID para"
    static void HandleTemperaturePIDChanged(DGUS_VP_Variable &var, void *val_ptr);
    // Hook for PID autotune
    static void HandlePIDAutotune(DGUS_VP_Variable &var, void *val_ptr);
  #endif
  #if HAS_BED_PROBE
    // Hook for "Change probe offset z"
    static void HandleZoffsetChange(DGUS_VP_Variable &var, void *val_ptr);

    static void OnMeshLevelingStart();

    static void OnMeshLevelingUpdate(const int8_t xpos, const int8_t ypos);
  #endif

  // Hook for live z adjust action
  static void HandleLiveAdjustZ(DGUS_VP_Variable &var, void *val_ptr);

  // Hook for heater control
  static void HandleHeaterControl(DGUS_VP_Variable &var, void *val_ptr);
  #if ENABLED(DGUS_PREHEAT_UI)
    // Hook for preheat
    static void HandlePreheat(DGUS_VP_Variable &var, void *val_ptr);
  #endif
  #if ENABLED(DGUS_FILAMENT_LOADUNLOAD)
    // Hook for filament load and unload filament option
    static void HandleFilamentOption(DGUS_VP_Variable &var, void *val_ptr);
    // Hook for filament load and unload
    static void HandleFilamentLoadUnload(DGUS_VP_Variable &var);
  #endif

  #if ENABLED(SDSUPPORT)
    // Callback for VP "Display wants to change screen when there is a SD card"
    static void ScreenChangeHookIfSD(DGUS_VP_Variable &var, void *val_ptr);
    /// Scroll buttons on the file listing screen.
    static void DGUSLCD_SD_ScrollFilelist(DGUS_VP_Variable &var, void *val_ptr);
    /// File touched.
    static void DGUSLCD_SD_FileSelected(DGUS_VP_Variable &var, void *val_ptr);
    /// start print after confirmation received.
    static void DGUSLCD_SD_StartPrint(DGUS_VP_Variable &var, void *val_ptr);
    /// User hit the pause, resume or abort button.
    static void DGUSLCD_SD_ResumePauseAbort(DGUS_VP_Variable &var, void *val_ptr);
    /// User confirmed the abort action
    static void DGUSLCD_SD_ReallyAbort(DGUS_VP_Variable &var, void *val_ptr);
    /// User hit the tune button
    static void DGUSLCD_SD_PrintTune(DGUS_VP_Variable &var, void *val_ptr);
    /// Send a single filename to the display.
    static void DGUSLCD_SD_SendFilename(DGUS_VP_Variable &var);
    /// Marlin informed us that a new SD has been inserted.
    static void SDCardInserted();
    /// Marlin informed us that the SD Card has been removed().
    static void SDCardRemoved();
    /// Marlin informed us about a bad SD Card.
    static void SDCardError();

    static void SetPrintingFromHost();
  #endif

  static void HandleLEDToggle();

  static void HandleFanToggle();

  static void FilamentRunout();

  static void OnFactoryReset();

#if HAS_BUZZER
  static void Buzzer(const uint16_t frequency, const uint16_t duration);
#endif

  static void OnHomingStart();
  static void OnHomingComplete();
  static void OnPrintFinished();

  // OK Button the Confirm screen.
  static void ScreenConfirmedOK(DGUS_VP_Variable &var, void *val_ptr);

  // Update data after went to new screen (by display or by GotoScreen)
  // remember: store the last-displayed screen, so it can get returned to.
  // (e.g for pop up messages)
  static void UpdateNewScreen(DGUSLCD_Screens newscreen, bool save_current_screen=true);

  // Recall the remembered screen.
  static void PopToOldScreen();

  // Make the display show the screen and update all VPs in it.
  static void GotoScreen(DGUSLCD_Screens screen, bool save_current_screen = true);

  static void UpdateScreenVPData();

  // Helpers to convert and transfer data to the display.
  static void DGUSLCD_SendWordValueToDisplay(DGUS_VP_Variable &var);
  static void DGUSLCD_SendStringToDisplay(DGUS_VP_Variable &var);
  static void DGUSLCD_SendStringToDisplayPGM(DGUS_VP_Variable &var);
  static void DGUSLCD_SendTemperaturePID(DGUS_VP_Variable &var);
  static void DGUSLCD_SendPercentageToDisplay(DGUS_VP_Variable &var);
  static void DGUSLCD_SendPrintProgressToDisplay(DGUS_VP_Variable &var);
  static void DGUSLCD_SendPrintTimeToDisplay(DGUS_VP_Variable &var);
  #if ENABLED(PRINTCOUNTER)
    static void DGUSLCD_SendPrintAccTimeToDisplay(DGUS_VP_Variable &var);
    static void DGUSLCD_SendPrintsTotalToDisplay(DGUS_VP_Variable &var);
  #endif
  #if HAS_FAN
    static void DGUSLCD_SendFanStatusToDisplay(DGUS_VP_Variable &var);
    static void DGUSLCD_SendFanSpeedToDisplay(DGUS_VP_Variable &var);
  #endif
  static void DGUSLCD_SendHeaterStatusToDisplay(DGUS_VP_Variable &var);
  #if ENABLED(DGUS_UI_WAITING)
    static void DGUSLCD_SendWaitingStatusToDisplay(DGUS_VP_Variable &var);
  #endif

  static void DGUSLCD_SendAboutFirmwareWebsite(DGUS_VP_Variable &var);
  static void DGUSLCD_SendAboutFirmwareVersion(DGUS_VP_Variable &var);
  static void DGUSLCD_SendAboutPrintSize(DGUS_VP_Variable &var);

  /// Send a value from 0..100 to a variable with a range from 0..255
  static void DGUSLCD_PercentageToUint8(DGUS_VP_Variable &var, void *val_ptr);

  template<typename T>
  static void DGUSLCD_SetValueDirectly(DGUS_VP_Variable &var, void *val_ptr) {
    if (!var.memadr) return;
    union { unsigned char tmp[sizeof(T)]; T t; } x;
    unsigned char *ptr = (unsigned char*)val_ptr;
    LOOP_L_N(i, sizeof(T)) x.tmp[i] = ptr[sizeof(T) - i - 1];
    *(T*)var.memadr = x.t;
  }

  /// Send a float value to the display.
  /// Display will get a 4-byte integer scaled to the number of digits:
  /// Tell the display the number of digits and it cheats by displaying a dot between...
  template<unsigned int decimals>
  static void DGUSLCD_SendFloatAsLongValueToDisplay(DGUS_VP_Variable &var) {
    if (var.memadr) {
      float f = *(float *)var.memadr;
      f *= cpow(10, decimals);

      // Round - truncated values look like skipped numbers
      long roundedValue = static_cast<long>(round(f));
      dgusdisplay.WriteVariable(var.VP, roundedValue);
    }
  }

  // Send an icon to the display, depending on whether it is true or false
  template<unsigned int value_if_true, unsigned int value_if_false>
  static void DGUSLCD_SendIconValue(DGUS_VP_Variable &var) {
    if (var.memadr) {
      bool value = *(bool *)var.memadr;
      uint16_t valueToSend = value ? value_if_true : value_if_false;
      dgusdisplay.WriteVariable(var.VP, valueToSend);
    }
  }

  /// Send a float value to the display.
  /// Display will get a 2-byte integer scaled to the number of digits:
  /// Tell the display the number of digits and it cheats by displaying a dot between...
  template<unsigned int decimals>
  static void DGUSLCD_SendFloatAsIntValueToDisplay(DGUS_VP_Variable &var) {
    if (var.memadr) {
      float f = *(float *)var.memadr;
      f *= cpow(10, decimals);

      // Round - truncated values look like skipped numbers
      int16_t roundedValue = static_cast<int16_t>(round(f));
      dgusdisplay.WriteVariable(var.VP, roundedValue);
    }
  }

  template<AxisEnum Axis>
  static void SendAxisTrustValue(DGUS_VP_Variable &var) {
    bool trust = axis_is_trusted(Axis);

    uint16_t color = trust ? 0xFFFF /*White*/ : 0XF800 /*Red*/;
    dgusdisplay.SetVariableDisplayColor(var.VP, color);

    //PGM_P suffix = trust ? nullptr : "???";
    //dgusdisplay.SetVariableAppendText(var.VP, suffix);
  }


  /// Force an update of all VP on the current screen.
  static inline void ForceCompleteUpdate() { update_ptr = 0; ScreenComplete = false; }
  /// Has all VPs sent to the screen
  static inline bool IsScreenComplete() { return ScreenComplete; }

  static inline DGUSLCD_Screens getCurrentScreen() { return current_screen; }

  static bool HandlePendingUserConfirmation();

  static float feed_amount;
  static bool are_steppers_enabled;

private:
  static DGUSLCD_Screens current_screen;  ///< currently on screen
  static constexpr uint8_t NUM_PAST_SCREENS = 4;
  static DGUSLCD_Screens past_screens[NUM_PAST_SCREENS]; ///< LIFO with past screens for the "back" button.

  static uint8_t update_ptr;    ///< Last sent entry in the VPList for the actual screen.
  static uint16_t skipVP;       ///< When updating the screen data, skip this one, because the user is interacting with it.
  static bool ScreenComplete;   ///< All VPs sent to screen?

  static uint16_t ConfirmVP;    ///< context for confirm screen (VP that will be emulated-sent on "OK").

  static uint8_t MeshLevelIndex;

  #if ENABLED(SDSUPPORT)
    static int16_t top_file;    ///< file on top of file chooser
    static int16_t file_to_print; ///< touched file to be confirmed
  #endif

public: // Needed for VP auto-upload
  static creality_dwin_settings_t Settings;
};

extern DGUSScreenHandler ScreenHandler;
