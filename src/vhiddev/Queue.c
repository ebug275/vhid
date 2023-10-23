#include "driver.h"
#include "queue.tmh"

#ifdef ALLOC_PRAGMA
#pragma alloc_text (PAGE, QueueInitialize)
#pragma alloc_text (PAGE, EvtIoDeviceControl)
#pragma alloc_text (PAGE, EvtIoStop)
#endif

NTSTATUS
QueueInitialize(
    _In_ WDFDEVICE Device
    )
{
    WDFQUEUE queue;
    NTSTATUS status;
    WDF_IO_QUEUE_CONFIG queueConfig;

    PAGED_CODE();

    //
    // Configure a default queue so that requests that are not
    // configure-fowarded using WdfDeviceConfigureRequestDispatching to goto
    // other queues get dispatched here.
    //
    WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(&queueConfig, WdfIoQueueDispatchParallel);

    queueConfig.EvtIoDeviceControl = EvtIoDeviceControl;
    queueConfig.EvtIoStop = EvtIoStop;

    status = WdfIoQueueCreate(Device, &queueConfig, WDF_NO_OBJECT_ATTRIBUTES, &queue);

    if(!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "WdfIoQueueCreate failed %!STATUS!", status);
    }

    return status;
}

VOID
EvtIoDeviceControl(
    _In_ WDFQUEUE Queue,
    _In_ WDFREQUEST Request,
    _In_ size_t OutputBufferLength,
    _In_ size_t InputBufferLength,
    _In_ ULONG IoControlCode
    )
{
    NTSTATUS status = STATUS_INVALID_PARAMETER;
    WDFDEVICE hDevice;
    size_t length = 0;

    PAGED_CODE();

    UNREFERENCED_PARAMETER(OutputBufferLength);
    UNREFERENCED_PARAMETER(InputBufferLength);

    hDevice = WdfIoQueueGetDevice(Queue);

    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_QUEUE,
        "%!FUNC! Queue 0x%p, Request 0x%p OutputBufferLength %d InputBufferLength %d IoControlCode %d",
        Queue, Request, (int)OutputBufferLength, (int)InputBufferLength, IoControlCode);

    switch (IoControlCode) {
    case IOCTL_VHIDDEV_GAMEPAD_INPUT:
        TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_QUEUE, "IOCTL_VHIDDEV_GAMEPAD_INPUT");
        status = CompleteReport(hDevice, Request, HARDWARE_TYPE_GAMEPAD);
        break;

    case IOCTL_VHIDDEV_KEYBOARD_INPUT:
        TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_QUEUE, "IOCTL_VHIDDEV_KEYBOARD_INPUT");
        status = CompleteReport(hDevice, Request, HARDWARE_TYPE_KEYBOARD);
        break;

    case IOCTL_VHIDDEV_MOUSE_INPUT:
        TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_QUEUE, "IOCTL_VHIDDEV_MOUSE_INPUT");
        status = CompleteReport(hDevice, Request, HARDWARE_TYPE_MOUSE);
        break;

    default:
        // unsupported request
        break;
    }

    WdfRequestCompleteWithInformation(Request, status, length);

    return;
}

VOID
EvtIoStop(
    _In_ WDFQUEUE Queue,
    _In_ WDFREQUEST Request,
    _In_ ULONG ActionFlags
)
{
    TraceEvents(TRACE_LEVEL_INFORMATION, 
                TRACE_QUEUE, 
                "%!FUNC! Queue 0x%p, Request 0x%p ActionFlags %d", 
                Queue, Request, ActionFlags);

    //
    // In most cases, the EvtIoStop callback function completes, cancels, or postpones
    // further processing of the I/O request.
    //
    // Typically, the driver uses the following rules:
    //
    // - If the driver owns the I/O request, it calls WdfRequestUnmarkCancelable
    //   (if the request is cancelable) and either calls WdfRequestStopAcknowledge
    //   with a Requeue value of TRUE, or it calls WdfRequestComplete with a
    //   completion status value of STATUS_SUCCESS or STATUS_CANCELLED.
    //
    //   Before it can call these methods safely, the driver must make sure that
    //   its implementation of EvtIoStop has exclusive access to the request.
    //
    //   In order to do that, the driver must synchronize access to the request
    //   to prevent other threads from manipulating the request concurrently.
    //   The synchronization method you choose will depend on your driver's design.
    //
    //   For example, if the request is held in a shared context, the EvtIoStop callback
    //   might acquire an internal driver lock, take the request from the shared context,
    //   and then release the lock. At this point, the EvtIoStop callback owns the request
    //   and can safely complete or requeue the request.
    //
    // - If the driver has forwarded the I/O request to an I/O target, it either calls
    //   WdfRequestCancelSentRequest to attempt to cancel the request, or it postpones
    //   further processing of the request and calls WdfRequestStopAcknowledge with
    //   a Requeue value of FALSE.
    //
    // A driver might choose to take no action in EvtIoStop for requests that are
    // guaranteed to complete in a small amount of time.
    //
    // In this case, the framework waits until the specified request is complete
    // before moving the device (or system) to a lower power state or removing the device.
    // Potentially, this inaction can prevent a system from entering its hibernation state
    // or another low system power state. In extreme cases, it can cause the system
    // to crash with bugcheck code 9F.
    //

    return;
}

NTSTATUS CompleteReport(
    _In_ WDFDEVICE      Device,
    _In_ WDFREQUEST     Request,
    _In_ HARDWARE_TYPE  HardwareType
)
{
    NTSTATUS status = STATUS_SUCCESS;
    PFDO_DEVICE_DATA fdoData = NULL;

    fdoData = FdoGetData(Device);

    switch (HardwareType) {
    case HARDWARE_TYPE_GAMEPAD:
        if (fdoData->PdoChilds[HARDWARE_TYPE_GAMEPAD])
            status = GamepadCompleteReport(fdoData->PdoChilds[HARDWARE_TYPE_GAMEPAD], Request);
        break;

    case HARDWARE_TYPE_KEYBOARD:
        if (fdoData->PdoChilds[HARDWARE_TYPE_KEYBOARD])
            status = KeyboardCompleteReport(fdoData->PdoChilds[HARDWARE_TYPE_KEYBOARD], Request);
        break;

    case HARDWARE_TYPE_MOUSE:
        if (fdoData->PdoChilds[HARDWARE_TYPE_MOUSE])
            status = MouseCompleteReport(fdoData->PdoChilds[HARDWARE_TYPE_MOUSE], Request);
        break;

    default:
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "CompleteReport error - unknown hardware type");
    }

    return status;
}
