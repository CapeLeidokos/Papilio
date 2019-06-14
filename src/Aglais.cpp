/* -*- mode: c++ -*-
 * Kaleidoscope-Simulator -- A C++ testing API for the Kaleidoscope keyboard 
 *                         firmware.
 * Copyright (C) 2019  noseglasses (shinynoseglasses@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Aglais.h"
#include "aglais/Parser.h"
#include "aglais/Consumer_.h"
#include "assertions/generic_report/ReportEquals.h"

namespace kaleidoscope {
namespace simulator {
   
class SimulatorConsumerAdaptor : public aglais::Consumer_
{
   public:
      
      SimulatorConsumerAdaptor(Simulator &simulator)
         :  simulator_(simulator)
      {}
      
      virtual void onFirmwareId(const char *firmware_id) override {
         // TODO: Use this method to verify that the firmware that was used
         //       to generate the Aglais-script that is currently 
         //       parsed matches the firmware running in the simulator.
      }
      
      virtual void onStartCycle(uint16_t cycle_id, uint16_t cycle_start_time) override {
         simulator_.setTime(cycle_start_time);
      }
      virtual void onEndCycle(uint16_t cycle_id, uint16_t cycle_end_time) override {
         simulator_.cycle();
         
         if(!simulator_.reportAssertionsQueue().empty()) {
            simulator_.error() << "Report assertions are left in queue";
         }
         
         simulator_.setTime(cycle_end_time);
      }
      virtual void onKeyPressed(uint8_t row, uint8_t col) override {
         simulator_.pressKey(row, col);
      }
      virtual void onKeyReleased(uint8_t row, uint8_t col) override {
         simulator_.releaseKey(row, col);
      }
      virtual void onKeyboardReport(uint8_t id, int length, const void *data) override {
         
         switch(id) {
            // TODO: React appropriately on the following
            //
            case HID_REPORTID_GAMEPAD:
            case HID_REPORTID_CONSUMERCONTROL:
            case HID_REPORTID_SYSTEMCONTROL:
               simulator_.log() << "***Ignoring hid report with id = " << id;
               break;
            case HID_REPORTID_MOUSE_ABSOLUTE:
               {
                  simulator_.reportAssertionsQueue().queue(
                     assertions::ReportEquals<AbsoluteMouseReport>{data}
                  );
               }
               break;
            case HID_REPORTID_MOUSE:
               {
                  simulator_.reportAssertionsQueue().queue(
                     assertions::ReportEquals<MouseReport>{data}
                  );
               }
               break;
            case HID_REPORTID_NKRO_KEYBOARD:
               {
                  simulator_.reportAssertionsQueue().queue(
                     assertions::ReportEquals<KeyboardReport>{data}
                  );
               }
               break;
            default:
               simulator_.error() << "Aglais encountered unknown HID report with id = " << id;
         }
      }
      virtual void onSetTime(uint16_t time) override {
         simulator_.setTime(time);
      }
      virtual void onCycle(uint16_t cycle_id, uint16_t cycle_start_time, uint16_t cycle_end_time) override {
         simulator_.setTime(cycle_start_time);
         simulator_.cycle();
         simulator_.setTime(cycle_end_time);
      }
      
   private:
      
      Simulator &simulator_;
      
};

void parseAglaisScript(const char *code, Simulator &simulator)
{
   aglais::Parser parser;
   SimulatorConsumerAdaptor sca(simulator);
   
   parser.parse(code, sca);
}

} // namespace simulator
} // namespace kaleidoscope
