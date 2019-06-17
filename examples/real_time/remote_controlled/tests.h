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

#ifdef KALEIDOSCOPE_VIRTUAL_BUILD

#include "Kaleidoscope-Simulator.h"
#include "vendors/keyboardio/model01.h"
#include "aux/terminal_escape_sequences.h"

#include <iostream>
   
const char *executable_name = nullptr;

void parseCommandLine(int argc, char* argv[]) { 
   executable_name = argv[0];
}
   
KALEIDOSCOPE_SIMULATOR_INIT

namespace kaleidoscope {
namespace simulator {
   
void runSimulator(Simulator &simulator) {
   
   using namespace actions;
   using namespace terminal_escape_sequences;

   // Check out https://github.com/CapeLeidokos/Kaleidoscope-Simulator-Control
         
   auto test = simulator.newTest("Real time simulation");
   
   // Activate the rainbow wave LED effect
   //
   simulator.multiTapKey(2 /*num. taps*/, 
                        0 /*row*/, 6/*col*/, 
                        1 /* num. cycles after each tap */
   );

   simulator.permanentReportActions().add(DumpReport{});
   
   std::cout << clear_screen << std::flush;
   std::cout << cursor_to_upper_left << std::flush;
   
   std::cout << 
      "****************************\n"
      "*** Real-time Simulation ***\n"
      "****************************\n"
      "\n"
      "Expecting control input from stdin...\n"
      "\n"
      "Please make sure to pass keyboard input to this executable's stdin.\n"
      "\n"
      "If you are currently building the example with make, stop the\n"
      "build process and execute the following line in your console window.\n"
      "\n"
      "   cat /dev/ttyACM0 | " << executable_name << " -t\n"
      "\n"
      "See the documentation of Kaleidoscope-Simulator and Kaleidoscope-Simulator-Control\n"
      "to see how input for real-time simulations can be generated.\n"
      "\n"
      << std::flush;

   // Wait for input to arrive
   //
   std::string dummy;
   while(dummy.empty()) {
      std::getline(std::cin, dummy);
   }
   
   std::cout << clear_screen << std::flush;
   
   simulator.runRemoteControlled( 
      [&]() {
         std::cout << cursor_to_upper_left << std::flush;
         renderKeyboard(simulator, keyboardio::model01::ascii_keyboard);
      }
   );
}

} // namespace simulator
} // namespace kaleidoscope

#endif
