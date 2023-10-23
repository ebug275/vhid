#pragma once

EXTERN_C_START

//
// This is the context that can be placed per queue
// and would contain per queue information.
//
typedef struct _PENDING_QUEUE_CONTEXT
{
    WDFQUEUE            Queue;
    PPDO_DEVICE_DATA    PdoData;
    WDFTIMER            Timer;
} PENDING_QUEUE_CONTEXT, * PPENDING_QUEUE_CONTEXT;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(PENDING_QUEUE_CONTEXT, GetPendingQueueContext)

NTSTATUS
QueueInitialize(
    _In_ WDFDEVICE Device
    );

//
// Events from the IoQueue object
//
EVT_WDF_IO_QUEUE_IO_DEVICE_CONTROL EvtIoDeviceControl;
EVT_WDF_IO_QUEUE_IO_STOP EvtIoStop;

NTSTATUS CompleteReport(
    _In_ WDFDEVICE      Device,
    _In_ WDFREQUEST     Request,
    _In_ HARDWARE_TYPE  HardwareType
);

EXTERN_C_END
