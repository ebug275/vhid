#pragma once

EXTERN_C_START

NTSTATUS
MouseCreatePdo(
    _In_  WDFDEVICE      Device,
    _In_  HARDWARE_TYPE  HardwareType
);

BOOLEAN
MouseGetCrispinessLevel(
    _In_  WDFDEVICE  ChildDevice,
    _Out_ PUCHAR     Level
);

BOOLEAN
MouseSetCrispinessLevel(
    _In_ WDFDEVICE  ChildDevice,
    _In_ UCHAR      Level
);

BOOLEAN
MouseIsSafetyLockEnabled(
    _In_ WDFDEVICE  Context
);

NTSTATUS
MouseQueueInitialize(
    _In_ WDFDEVICE  Device
);

NTSTATUS
MouseCompleteReport(
    _In_ WDFDEVICE   Device,
    _In_ WDFREQUEST  Request
);

EVT_WDF_IO_QUEUE_IO_INTERNAL_DEVICE_CONTROL MouseEvtIoInternalDeviceControl;
EVT_WDF_IO_QUEUE_IO_STOP MouseEvtIoStop;

NTSTATUS
MouseUrbProcess(
    _In_ PPDO_DEVICE_DATA  PdoData,
    _In_ WDFREQUEST        Request,
    _In_ PURB              Urb
);

NTSTATUS
MouseSelectConfiguration(
    _In_ PPDO_DEVICE_DATA  PdoData,
    _In_ WDFREQUEST        Request,
    _In_ PURB              Urb
);

NTSTATUS
MouseGetUsbDescriptor(
    _In_ PPDO_DEVICE_DATA  PdoData,
    _In_ WDFREQUEST        Request,
    _In_ PURB              Urb
);

NTSTATUS
MouseProcessClassInterface(
    _In_ PPDO_DEVICE_DATA  PdoData,
    _In_ WDFREQUEST        Request,
    _In_ PURB              Urb
);

NTSTATUS
MouseProcessControlTransfer(
    _In_ PPDO_DEVICE_DATA  PdoData,
    _In_ WDFREQUEST        Request,
    _In_ PURB              Urb
);

EXTERN_C_END
