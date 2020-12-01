/*
  HIDPowerDevice.cpp

  Copyright (c) 2020, Aleksandr Bratchik
  Original code (pre-library): Copyright (c) 2020, Aleksandr Bratchik

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "HIDPowerDevice.h"

#if defined(_USING_HID)

//================================================================================
//================================================================================
//	UPS

static const uint8_t _hidReportDescriptor[] PROGMEM = {

    // APC descriptor
    0x05, 0x84, // USAGE_PAGE (Power Device)
    0x09, 0x04, // USAGE (UPS)
    0xA1, 0x01, // COLLECTION (Application)
    0x09, 0x24, //   USAGE (Sink)
    0xA1, 0x02, //   COLLECTION (Logical)
    0x75, 0x08, //     REPORT_SIZE (8)
    0x95, 0x01, //     REPORT_COUNT (1)
    0x15, 0x00, //     LOGICAL_MINIMUM (0)
    0x26, 0xFF, 0x00, //     LOGICAL_MAXIMUM (255)
    0x85, HID_PD_IPRODUCT, //     REPORT_ID (1)
    0x09, 0xFE, //     USAGE (iProduct)
    0x79, 0x02, //     STRING INDEX (2)
    0xB1, 0x23, //     FEATURE (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Nonvolatile, Bitfield)
    0x85, HID_PD_SERIAL, //     REPORT_ID (2)
    0x09, 0xFF, //     USAGE (iSerialNumber)
    0x79, 0x03, //     STRING INDEX (3)
    0xB1, 0x23, //     FEATURE (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Nonvolatile, Bitfield)
    0x85, HID_PD_MANUFACTURER, //     REPORT_ID (3)
    0x09, 0xFD, //     USAGE (iManufacturer)
    0x79, 0x01, //     STRING INDEX (1)
    0xB1, 0x23, //     FEATURE (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Nonvolatile, Bitfield)
    0x05, 0x85, //     USAGE_PAGE (Battery System)
    0x85, HID_PD_RECHARGEABLE, //     REPORT_ID (6)
    0x09, 0x8B, //     USAGE (Rechargable)                  
    0xB1, 0x23, //     FEATURE (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Nonvolatile, Bitfield)
    0x85, HID_PD_CAPACITYMODE, //     REPORT_ID (22)
    0x09, 0x2C, //     USAGE (CapacityMode)
    0xB1, 0x23, //     FEATURE (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Nonvolatile, Bitfield)
    0x85, HID_PD_RELSTATEOFCHARGE, //     REPORT_ID (25)
    0x09, 0x64, //     USAGE (RelativeStateOfCharge)
    0x81, 0x23, //     INPUT (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Bitfield)
    0x09, 0x64, //     USAGE (RelativeStateOfCharge)
    0xB1, 0xA3, //     FEATURE (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Volatile, Bitfield)
    0x85, HID_PD_ABSSTATEOFCHARGE, //     REPORT_ID (25)
    0x09, 0x65, //     USAGE (RelativeStateOfCharge)
    0x81, 0x23, //     INPUT (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Bitfield)
    0x09, 0x65, //     USAGE (RelativeStateOfCharge)
    0xB1, 0xA3, //     FEATURE (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Volatile, Bitfield)
    0x85, HID_PD_CPCTYGRANULARITY1, //     REPORT_ID (16)
    0x09, 0x8D, //     USAGE (CapacityGranularity1)
    0xB1, 0x22, //     FEATURE (Data, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Nonvolatile, Bitfield)
    0x85, HID_PD_CPCTYGRANULARITY2, //     REPORT_ID (24)
    0x09, 0x8E, //     USAGE (CapacityGranularity2)
    0xB1, 0x23, //     FEATURE (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Nonvolatile, Bitfield)
    0x85, HID_PD_FULLCHRGECAPACITY, //     REPORT_ID (14)
    0x75, 0x18, //     REPORT_SIZE (24)
    0x27, 0xFE, 0xFF, 0xFF, 0x00, //     LOGICAL_MAXIMUM (16777214)
    0x67, 0x01, 0x10, 0x10, 0x00, //     UNIT (AmpSec)
    0x55, 0x00, //     UNIT_EXPONENT (0)            
    0x09, 0x67, //     USAGE (FullChargeCapacity)
    0xB1, 0x83, //     FEATURE (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Volatile, Bitfield)
    0x85, HID_PD_DESIGNCAPACITY, //     REPORT_ID (23)
    0x09, 0x83, //     USAGE (DesignCapacity)
    0xB1, 0x83, //     FEATURE (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Volatile, Bitfield)
    0x85, HID_PD_REMAININGCAPACITY, //     REPORT_ID (12)
    0x09, 0x66, //     USAGE (RemainingCapacity)
    0x81, 0x23, //     INPUT (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Bitfield)
    0x09, 0x66, //     USAGE (RemainingCapacity)
    0xB1, 0xA3, //     FEATURE (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Volatile, Bitfield)
    0x85, HID_PD_WARNCAPACITYLIMIT, //     REPORT_ID (15)
    0x09, 0x8C, //     USAGE (WarningCapacityLimit)
    0xB1, 0xA2, //     FEATURE (Data, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Volatile, Bitfield)
    0x85, HID_PD_REMNCAPACITYLIMIT, //     REPORT_ID (17)
    0x09, 0x29, //     USAGE (RemainingCapacityLimit)
    0xB1, 0xA2, //     FEATURE (Data, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Volatile, Bitfield)
    0x85, HID_PD_AVERAGECURRENT, //     REPORT_ID (27)
    0x09, 0x62, //     USAGE (AverageCurrent)
    0xB1, 0xA2, //     FEATURE (Data, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Volatile, Bitfield)
    0x85, HID_PD_MANUFACTUREDATE, //     REPORT_ID (9)
    0x09, 0x85, //     USAGE (ManufacturerDate)
    0x75, 0x10, //     REPORT_SIZE (16)
    0x27, 0xFF, 0xFF, 0x00, 0x00, //     LOGICAL_MAXIMUM (65534)
    0xB1, 0xA3, //     FEATURE (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Volatile, Bitfield)
    0x85, HID_PD_RUNTIMETOEMPTY, //     REPORT_ID (13)
    0x09, 0x68, //     USAGE (RunTimeToEmpty)
    0x27, 0xFF, 0xFF, 0x00, 0x00, //     LOGICAL_MAXIMUM (65534)
    0x66, 0x01, 0x10, //     UNIT (Seconds)
    0x55, 0x00, //     UNIT_EXPONENT (0)
    0x81, 0xA3, //     INPUT (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Bitfield)
    0x09, 0x68, //     USAGE (RunTimeToEmpty)
    0xB1, 0xA3, //     FEATURE (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Volatile, Bitfield)
    0x85, HID_PD_AVERAGETIME2FULL, //     REPORT_ID (26)
    0x09, 0x6A, //     USAGE (AverageTimeToFull)  
    0x81, 0xA3, //     INPUT (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Bitfield)
    0x09, 0x6A, //     USAGE (AverageTimeToFull)
    0xB1, 0xA3, //     FEATURE (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Volatile, Bitfield) 
    0x05, 0x84, //     USAGE_PAGE (Power Device)
    0x85, HID_PD_AVERAGETIME2EMPTY, //     REPORT_ID (28)
    0x09, 0x69, //     USAGE (AverageTimeToEmpty)  
    0x81, 0xA3, //     INPUT (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Bitfield)
    0x09, 0x69, //     USAGE (AverageTimeToEmpty)
    0xB1, 0xA3, //     FEATURE (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Volatile, Bitfield) 
    0x05, 0x84, //     USAGE_PAGE (Power Device)
    0x85, HID_PD_DELAYBE4SHUTDOWN, //     REPORT_ID (18)
    0x09, 0x57, //     USAGE (DelayBeforeShutdown)
    0x16, 0xFF, 0xFF, //     LOGICAL_MINIMUM (65534)
    0x26, 0xFF, 0x7F, //     LOGICAL_MAXIMUM (32767)
    0xB1, 0xA2, //     FEATURE (Data, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Volatile, Bitfield)
    0x85, HID_PD_DELAYBE4REBOOT, //     REPORT_ID (19)
    0x09, 0x55, //     USAGE (DelayBeforeReboot)
    0x75, 0x18, //     REPORT_SIZE (24)
    0x17, 0xFF, 0xFF, 0xFF, 0xFF, //     LOGICAL_MINIMUM (4294967295)
    0x27, 0x18, 0xC5, 0x13, 0x00, //     LOGICAL_MAXIMUM (1295640)
    0xB1, 0xA2, //     FEATURE (Data, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Volatile, Bitfield)
    0x05, 0x85, //     USAGE_PAGE (Battery System)
    0x85, HID_PD_REMAINTIMELIMIT, //     REPORT_ID (8)
    0x09, 0x2A, //     USAGE (RemainingTimeLimit)
    0x75, 0x10, //     REPORT_SIZE (16)
    0x27, 0x64, 0x05, 0x00, 0x00, //     LOGICAL_MAXIMUM (1380)
    0x16, 0x78, 0x00, //     LOGICAL_MINIMUM (120)
    0x81, 0x22, //     INPUT (Data, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Bitfield)
    0x09, 0x2A, //     USAGE (RemainingTimeLimit)
    0xB1, 0xA2, //     FEATURE (Data, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Volatile, Bitfield)
    0x05, 0x84, //     USAGE_PAGE (Power Device)
    0x85, HID_PD_CONFIGVOLTAGE, //     REPORT_ID (10)
    0x09, 0x40, //     USAGE (ConfigVoltage)
    0x15, 0x00, //     LOGICAL_MINIMUM (0)
    0x27, 0xFF, 0xFF, 0x00, 0x00, //     LOGICAL_MAXIMUM (65535)
    0x67, 0x21, 0xD1, 0xF0, 0x00, //     UNIT (Millivolts)
    0x55, 0x00, //     UNIT_EXPONENT (0)
    0xB1, 0x23, //     FEATURE (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Nonvolatile, Bitfield)
    0x85, HID_PD_VOLTAGE, //     REPORT_ID (11)
    0x09, 0x30, //     USAGE (Voltage)
    0xB1, 0xA3, //     FEATURE (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Volatile, Bitfield)
    0x85, HID_PD_CURRENT, //     REPORT_ID (21)
    0x09, 0x31, //     USAGE (Current)
    0x16, 0xFF, 0xFF, //     LOGICAL_MINIMUM (-32768)
    0x26, 0xFF, 0x7F, //     LOGICAL_MAXIMUM (32767)
    0x67, 0x21, 0xD1, 0xF0, 0x00, //     UNIT (??)
    0x55, 0x05, //     UNIT_EXPONENT (5)
    0xB1, 0xA3, //     FEATURE (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Volatile, Bitfield)
    0x85, HID_PD_AUDIBLEALARMCTRL, //     REPORT_ID (20)
    0x09, 0x5A, //     USAGE (AudibleAlarmControl)
    0x75, 0x08, //     REPORT_SIZE (8)
    0x15, 0x01, //     LOGICAL_MINIMUM (1)
    0x25, 0x03, //     LOGICAL_MAXIMUM (3)
    0x65, 0x00, //     UNIT (0)
    0x55, 0x00, //     UNIT_EXPONENT (0)
    0x81, 0x22, //     INPUT (Data, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Bitfield)
    0x09, 0x5A, //     USAGE (AudibleAlarmControl)
    0xB1, 0xA2, //     FEATURE (Data, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Volatile, Bitfield)
    0x09, 0x02, //     USAGE (PresentStatus)
    0xA1, 0x02, //     COLLECTION (Logical)
    0x85, HID_PD_PRESENTSTATUS, //       REPORT_ID (7)
    0x05, 0x85, //       USAGE_PAGE (Battery System)
    0x09, 0x44, //       USAGE (Charging)
    0x75, 0x01, //       REPORT_SIZE (1)
    0x15, 0x00, //       LOGICAL_MINIMUM (0)
    0x25, 0x01, //       LOGICAL_MAXIMUM (1)
    0x81, 0xA3, //       INPUT (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Bitfield)
    0x09, 0x44, //       USAGE (Charging)
    0xB1, 0xA3, //       FEATURE (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Volatile, Bitfield)
    0x09, 0x45, //       USAGE (Discharging)
    0x81, 0xA3, //       INPUT (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Bitfield)
    0x09, 0x45, //       USAGE (Discharging)
    0xB1, 0xA3, //       FEATURE (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Volatile, Bitfield)
    0x09, 0xD0, //       USAGE (ACPresent)
    0x81, 0xA3, //       INPUT (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Bitfield)
    0x09, 0xD0, //       USAGE (ACPresent)
    0xB1, 0xA3, //       FEATURE (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Volatile, Bitfield)
    0x09, 0xD1, //       USAGE (BatteryPresent)
    0x81, 0xA3, //       INPUT (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Bitfield)
    0x09, 0xD1, //       USAGE (BatteryPresent)
    0xB1, 0xA3, //       FEATURE (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Volatile, Bitfield)
    0x09, 0x42, //       USAGE (BelowRemainingCapacityLimit)
    0x81, 0xA3, //       INPUT (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Bitfield)
    0x09, 0x42, //       USAGE (BelowRemainingCapacityLimit)
    0xB1, 0xA3, //       FEATURE (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Volatile, Bitfield)
    0x05, 0x84, //       USAGE_PAGE (Power Device)
    0x09, 0x68, //       USAGE (ShutdownRequested)
    0x81, 0xA2, //       INPUT (Data, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Bitfield)
    0x09, 0x68, //       USAGE (ShutdownRequested)
    0xB1, 0xA2, //       FEATURE (Data, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Volatile, Bitfield)
    0x09, 0x69, //       USAGE (ShutdownImminent)
    0x81, 0xA3, //       INPUT (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Bitfield)
    0x09, 0x69, //       USAGE (ShutdownImminent)
    0xB1, 0xA3, //       FEATURE (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Volatile, Bitfield)
    0x05, 0x85, //       USAGE_PAGE (Battery System)
    0x09, 0x43, //       USAGE (RemainingTimeLimitExpired)
    0x81, 0xA2, //       INPUT (Data, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Bitfield)
    0x09, 0x43, //       USAGE (RemainingTimeLimitExpired)
    0xB1, 0xA2, //       FEATURE (Data, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Volatile, Bitfield)
    0x05, 0x84, //       USAGE_PAGE (Power Device)
    0x09, 0x73, //       USAGE (CommunicationLost)
    0x81, 0xA3, //       INPUT (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Bitfield)
    0x09, 0x73, //       USAGE (CommunicationLost)
    0xB1, 0xA3, //       FEATURE (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Volatile, Bitfield)
    0x05, 0x85, //       USAGE_PAGE (Battery System)
    0x09, 0x4B, //       USAGE (NeedReplacement)
    0x81, 0xA3, //       INPUT (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Bitfield)
    0x09, 0x4B, //       USAGE (NeedReplacement)
    0xB1, 0xA3, //       FEATURE (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Volatile, Bitfield)
    0x05, 0x84, //       USAGE_PAGE (Power Device)
    0x09, 0x65, //       USAGE (Overload)
    0x81, 0xA3, //       INPUT (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Bitfield)
    0x09, 0x65, //       USAGE (Overload)
    0xB1, 0xA3, //       FEATURE (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Volatile, Bitfield)
    0x05, 0x85, //       USAGE_PAGE (Battery System)
    0x09, 0xDB, //       USAGE (VoltageNotRegulated)
    0x81, 0xA3, //       INPUT (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Bitfield)
    0x09, 0xDB, //       USAGE (VoltageNotRegulated)
    0xB1, 0xA3, //       FEATURE (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Volatile, Bitfield)
    0x09, 0x46, //       USAGE (FullyCharged)
    0x81, 0xA3, //       INPUT (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Bitfield)
    0x09, 0x46, //       USAGE (FullyCharged)
    0xB1, 0xA3, //       FEATURE (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Volatile, Bitfield)
    0x09, 0x47, //       USAGE (FullyDischarged)
    0x81, 0xA3, //       INPUT (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Bitfield)
    0x09, 0x47, //       USAGE (FullyDischarged)
    0xB1, 0xA3, //       FEATURE (Constant, Variable, Absolute, No Wrap, Linear, No Preferred, No Null Position, Volatile, Bitfield)
    0x95, 0x02, //       REPORT_COUNT (2)
    0x81, 0x01, //       INPUT (Constant, Array, Absolute)
    0xB1, 0x01, //       FEATURE (Constant, Array, Absolute, No Wrap, Linear, Preferred State, No Null Position, Nonvolatile, Bitfield) 
    0xC0, //     END_COLLECTION  
    0xC0, //   END_COLLECTION  
    0xC0 // END_COLLECTION                         

};

HIDPowerDevice_::HIDPowerDevice_(void) {
    static HIDSubDescriptor node(_hidReportDescriptor, sizeof (_hidReportDescriptor));

    HID().AppendDescriptor(&node);
}

void HIDPowerDevice_::begin(void) {
    HID().begin();
}

void HIDPowerDevice_::setSerial(Serial_& serial) {
    HID().setSerial(serial);
}

void HIDPowerDevice_::end(void) {
}

int HIDPowerDevice_::sendDate(uint8_t id, uint16_t year, uint8_t month, uint8_t day) {
    uint16_t bval = (year - 1980)*512 + month * 32 + day;
    return HID().SendReport(id, &bval, sizeof (bval));
}

int HIDPowerDevice_::sendReport(uint8_t id, const void* bval, int len) {
    return HID().SendReport(id, bval, len);
}

int HIDPowerDevice_::setFeature(uint8_t id, const void *data, int len) {
    return HID().SetFeature(id, data, len);
}

HIDPowerDevice_ PowerDevice;

#endif

