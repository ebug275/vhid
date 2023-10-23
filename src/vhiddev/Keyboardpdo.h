#pragma once

EXTERN_C_START

NTSTATUS
KeyboardCreatePdo(
    _In_  WDFDEVICE      Device,
    _In_  HARDWARE_TYPE  HardwareType
);

BOOLEAN
KeyboardGetCrispinessLevel(
    _In_  WDFDEVICE  ChildDevice,
    _Out_ PUCHAR     Level
);

BOOLEAN
KeyboardSetCrispinessLevel(
    _In_ WDFDEVICE  ChildDevice,
    _In_ UCHAR      Level
);

BOOLEAN
KeyboardIsSafetyLockEnabled(
    _In_ WDFDEVICE  Context
);

NTSTATUS
KeyboardQueueInitialize(
    _In_ WDFDEVICE  Device
);

NTSTATUS
KeyboardCompleteReport(
    _In_ WDFDEVICE   Device,
    _In_ WDFREQUEST  Request
);

EVT_WDF_IO_QUEUE_IO_INTERNAL_DEVICE_CONTROL KeyboardEvtIoInternalDeviceControl;
EVT_WDF_IO_QUEUE_IO_STOP KeyboardEvtIoStop;

NTSTATUS
KeyboardUrbProcess(
    _In_ PPDO_DEVICE_DATA  PdoData,
    _In_ WDFREQUEST        Request,
    _In_ PURB              Urb
);

NTSTATUS
KeyboardSelectConfiguration(
    _In_ PPDO_DEVICE_DATA  PdoData,
    _In_ WDFREQUEST        Request,
    _In_ PURB              Urb
);

NTSTATUS
KeyboardGetUsbDescriptor(
    _In_ PPDO_DEVICE_DATA  PdoData,
    _In_ WDFREQUEST        Request,
    _In_ PURB              Urb
);

NTSTATUS
KeyboardProcessClassInterface(
    _In_ PPDO_DEVICE_DATA  PdoData,
    _In_ WDFREQUEST        Request,
    _In_ PURB              Urb
);

NTSTATUS
KeyboardProcessControlTransfer(
    _In_ PPDO_DEVICE_DATA  PdoData,
    _In_ WDFREQUEST        Request,
    _In_ PURB              Urb
);

EXTERN_C_END
