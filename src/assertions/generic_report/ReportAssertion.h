/* -*- mode: c++ -*-
 * Kaleidoscope-Simulator -- A C++ testing API for the Kaleidoscope absolute mouse 
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

#include "assertions/Assertion_.h"
#include "Report_.h"

#include <cassert>

namespace kaleidoscope {
namespace simulator {
   
class ReportAssertion_ : public Assertion_
{
   public:
      
      typedef Report_ ReportType;
      typedef ReportAssertion_ AssertionBaseType;
      
      virtual uint8_t getReportTypeId() const { return GenericReportTypeId; }
      
      virtual void setReport(const Report_ *report) override {
         report_ = report;
      }
      
   protected:
      
      virtual const Report_ &getReport() const { 
         assert(report_);
         return *report_;
      }
      
   protected:
      
      const Report_ *report_ = nullptr;
};
   
/// @brief An abstract report assertion.
/// @details This abstract class serves as base class for any
///        report assertions.
///
///        **Important:** This class is not part of Kaleidoscope-Simulator's 
///                   public API. It is meant for internal use only.
///
template<typename _ReportType>
class ReportAssertion : public ReportAssertion_
{
   public:
      
      typedef _ReportType ReportType;
      
      virtual uint8_t getReportTypeId() const override {
         return _ReportType::hid_report_type_;
      }
      
      static const char *typeString() { 
         return _ReportType::typeString();
      }
      
      virtual const char *getTypeString() const override {
         return _ReportType::typeString();
      }
      
   protected:
      
      virtual const _ReportType &getReport() const override {
         assert(report_);
         return *static_cast<const _ReportType *>(report_);
      }
};

} // namespace simulator
} // namespace kaleidoscope