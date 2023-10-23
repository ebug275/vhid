#pragma once

#include <ntddk.h>
#include <wdf.h>
#define NTSTRSAFE_LIB
#include <ntstrsafe.h>
#include <initguid.h>
#include <usb.h>
#include <usbioctl.h>
#include <hidport.h>
#include <Wdmguid.h>

#include <Public.h>
#include <Hardware.h>

#include "Device.h"
#include "Queue.h"
#include "Buspdo.h"
#include "Gamepadpdo.h"
#include "Keyboardpdo.h"
#include "Mousepdo.h"
#include "Trace.h"

EXTERN_C_START

//
// WDFDRIVER Events
//

DRIVER_INITIALIZE DriverEntry;
EVT_WDF_DRIVER_DEVICE_ADD EvtDeviceAdd;
EVT_WDF_OBJECT_CONTEXT_CLEANUP EvtDriverContextCleanup;

VOID
DumpMemory(
    _In_ PUCHAR  Buffer,
    _In_ DWORD   BufferLen
);

EXTERN_C_END
