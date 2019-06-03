/* -*- mode: c++ -*-
 * Kaleidoscope-Testing -- A C++ testing API for the Kaleidoscope keyboard 
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

#pragma once

#include "assertions/_Assertion.h"

#include <vector>

namespace kaleidoscope {
namespace testing {
   
/** class Group
 *  brief Groups multiple assertions.
 */
class Group {
   
   KS_TESTING_ASSERTION_WRAPPER(Group)
 
   private:
      
      class Assertion : public _Assertion {
         
         public:

            template<typename..._AssertionPtrs>
            AssertionGroup(_AssertionPtrs...assertion_ptrs)
               :  assertions_(std::forward<_AssertionPtrs>(assertion_ptrs)...)
            {}

            virtual void report(std::ostream &out, const char *add_indent = "") const override;

            virtual void setTestDriver(std::weak_ptr<TestDriver> test_driver) override;

            virtual void describe(std::ostream, const char *add_indent = "") const override {
               return "A group of assertions";
            }

            virtual describeState(std::ostream &out, const char *add_indent = "") const override;

         private:

            virtual bool evalInternal() override {
               valid_ = true;
               
               for(auto &assertion: assertions_) {
                  valid_ &= assertion->eval();
               }
               
               return valid_;
            }
      };
};

} // namespace testing
} // namespace kaleidoscope
