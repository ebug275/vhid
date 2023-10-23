#pragma once

EXTERN_C_START

NTSTATUS
GamepadCreatePdo(
    _In_  WDFDEVICE      Device,
    _In_  HARDWARE_TYPE  HardwareType
);

BOOLEAN
GamepadGetCrispinessLevel(
    _In_  WDFDEVICE  ChildDevice,
    _Out_ PUCHAR     Level
);

BOOLEAN
GamepadSetCrispinessLevel(
    _In_ WDFDEVICE  ChildDevice,
    _In_ UCHAR      Level
);

BOOLEAN
GamepadIsSafetyLockEnabled(
    _In_ WDFDEVICE  Context
);

NTSTATUS
GamepadQueueInitialize(
    _In_ WDFDEVICE  Device
);

NTSTATUS
GamepadCompleteReport(
    _In_ WDFDEVICE   Device,
    _In_ WDFREQUEST  Request
);

EVT_WDF_IO_QUEUE_IO_INTERNAL_DEVICE_CONTROL GamepadEvtIoInternalDeviceControl;
EVT_WDF_IO_QUEUE_IO_STOP GamepadEvtIoStop;

NTSTATUS
GamepadUrbProcess(
    _In_ PPDO_DEVICE_DATA  PdoData,
    _In_ WDFREQUEST        Request,
    _In_ PURB              Urb
);

NTSTATUS
GamepadSelectConfiguration(
    _In_ PPDO_DEVICE_DATA  PdoData,
    _In_ WDFREQUEST        Request,
    _In_ PURB              Urb
);

NTSTATUS
GamepadGetUsbDescriptor(
    _In_ PPDO_DEVICE_DATA  PdoData,
    _In_ WDFREQUEST        Request,
    _In_ PURB              Urb
);

NTSTATUS
GamepadProcessClassInterface(
    _In_ PPDO_DEVICE_DATA  PdoData,
    _In_ WDFREQUEST        Request,
    _In_ PURB              Urb
);

NTSTATUS
GamepadProcessControlTransfer(
    _In_ PPDO_DEVICE_DATA  PdoData,
    _In_ WDFREQUEST        Request,
    _In_ PURB              Urb
);

EXTERN_C_END
