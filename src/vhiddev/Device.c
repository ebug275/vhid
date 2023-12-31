#include "driver.h"
#include "device.tmh"

#ifdef ALLOC_PRAGMA
#pragma alloc_text (PAGE, CreateDevice)
#endif

NTSTATUS
CreateDevice(
    _Inout_ PWDFDEVICE_INIT DeviceInit
    )
{
    WDF_PNPPOWER_EVENT_CALLBACKS pnpPowerCallbacks;
    WDF_OBJECT_ATTRIBUTES deviceAttributes;
    PNP_BUS_INFORMATION busInfo;
    PFDO_DEVICE_DATA deviceContext;
    WDFDEVICE device;
    NTSTATUS status;

    PDEVICE_OBJECT deviceObject;
    WDFIOTARGET ioTarget;
    WDF_IO_TARGET_OPEN_PARAMS openParams;

    PAGED_CODE();

    WDF_PNPPOWER_EVENT_CALLBACKS_INIT(&pnpPowerCallbacks);
    pnpPowerCallbacks.EvtDevicePrepareHardware = EvtDevicePrepareHardware;
    pnpPowerCallbacks.EvtDeviceReleaseHardware = EvtDeviceReleaseHardware;
    pnpPowerCallbacks.EvtDeviceD0Entry = EvtDeviceD0Entry;
    pnpPowerCallbacks.EvtDeviceD0Exit = EvtDeviceD0Exit;
    WdfDeviceInitSetPnpPowerEventCallbacks(DeviceInit, &pnpPowerCallbacks);

    WdfDeviceInitSetDeviceType(DeviceInit, FILE_DEVICE_BUS_EXTENDER);
    WdfDeviceInitSetExclusive(DeviceInit, TRUE);

    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&deviceAttributes, FDO_DEVICE_DATA);

    status = WdfDeviceCreate(&DeviceInit, &deviceAttributes, &device);
    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_DEVICE, "WdfDeviceCreate failed %!STATUS!", status);
        return status;
    }

    //
    // Get a pointer to the device context structure that we just associated
    // with the device object. We define this structure in the device.h
    // header file. DeviceGetContext is an inline function generated by
    // using the WDF_DECLARE_CONTEXT_TYPE_WITH_NAME macro in device.h.
    // This function will do the type checking and return the device context.
    // If you pass a wrong object handle it will return NULL and assert if
    // run under framework verifier mode.
    //
    deviceContext = FdoGetData(device);
    RtlZeroMemory(deviceContext, sizeof(PFDO_DEVICE_DATA));

    WDF_OBJECT_ATTRIBUTES_INIT(&deviceAttributes);
    deviceAttributes.ParentObject = device;

    status = WdfWaitLockCreate(&deviceAttributes, &deviceContext->ChildLock);
    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_DEVICE, "WdfWaitLockCreate failed %!STATUS!", status);
        return status;
    }

    //
    // Create a device interface so that applications can find and talk
    // to us.
    //
    status = WdfDeviceCreateDeviceInterface(device, &GUID_DEVINTERFACE_VHIDDEV, NULL); // ReferenceString
    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_DEVICE, "WdfDeviceCreateDeviceInterface failed %!STATUS!", status);
        return status;
    }

    //
    // Initialize the I/O Package and any Queues
    //
    status = QueueInitialize(device);
    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_DEVICE, "QueueInitialize failed %!STATUS!", status);
        return status;
    }

    deviceObject = WdfDeviceWdmGetDeviceObject(device);

    status = WdfIoTargetCreate(device, WDF_NO_OBJECT_ATTRIBUTES, &ioTarget);
    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_DEVICE, "WdfIoTargetCreate failed %!STATUS!", status);
        return status;
    }

    WDF_IO_TARGET_OPEN_PARAMS_INIT_EXISTING_DEVICE(&openParams, deviceObject);

    status = WdfIoTargetOpen(ioTarget, &openParams);
    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_DEVICE, "WdfIoTargetOpen failed %!STATUS!", status);
        return status;
    }

    deviceContext->IoTargetToSelf = ioTarget;
    deviceContext->DeviceObjectToSelf = deviceObject;

    busInfo.BusTypeGuid = GUID_BUS_TYPE_USB;
    busInfo.LegacyBusType = PNPBus;
    busInfo.BusNumber = 0;

    WdfDeviceSetBusInformationForChildren(device, &busInfo);

    status = DoStaticEnumeration(device);

    return status;
}

NTSTATUS
EvtDevicePrepareHardware(
    _In_ WDFDEVICE     Device,
    _In_ WDFCMRESLIST  ResourceList,
    _In_ WDFCMRESLIST  ResourceListTranslated
)
{
    UNREFERENCED_PARAMETER(Device);
    UNREFERENCED_PARAMETER(ResourceList);
    UNREFERENCED_PARAMETER(ResourceListTranslated);

    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DEVICE, "%!FUNC! Device 0x%p", Device);

    return STATUS_SUCCESS;
}

NTSTATUS
EvtDeviceReleaseHardware(
    _In_ WDFDEVICE     Device,
    _In_ WDFCMRESLIST  ResourcesTranslated
)
{
    UNREFERENCED_PARAMETER(Device);
    UNREFERENCED_PARAMETER(ResourcesTranslated);

    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DEVICE, "%!FUNC! Device 0x%p", Device);

    return STATUS_SUCCESS;
}

NTSTATUS
EvtDeviceD0Entry(
    _In_ WDFDEVICE               Device,
    _In_ WDF_POWER_DEVICE_STATE  PreviousState
)
{
    NTSTATUS status = STATUS_SUCCESS;

    UNREFERENCED_PARAMETER(PreviousState);

    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DEVICE, "%!FUNC! Device 0x%p, PreviousState %d", Device, PreviousState);

    status = WdfIoTargetStart(WdfDeviceGetIoTarget(Device));

    return status;
}

NTSTATUS
EvtDeviceD0Exit(
    _In_ WDFDEVICE               Device,
    _In_ WDF_POWER_DEVICE_STATE  TargetState
)
{
    UNREFERENCED_PARAMETER(TargetState);

    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DEVICE, "%!FUNC! Device 0x%p, TargetState %d", Device, TargetState);

    WdfIoTargetStop(WdfDeviceGetIoTarget(Device), WdfIoTargetCancelSentIo);

    return STATUS_SUCCESS;
}

NTSTATUS
PlugInDevice(
    _In_ WDFDEVICE      Device,
    _In_ HARDWARE_TYPE  HardwareType
)
{
    NTSTATUS status = STATUS_SUCCESS;
    BOOLEAN unique = TRUE;
    WDFDEVICE hChild;
    PPDO_DEVICE_DATA pdoData;
    PFDO_DEVICE_DATA deviceData;

    PAGED_CODE();

    deviceData = FdoGetData(Device);
    hChild = NULL;

    WdfWaitLockAcquire(deviceData->ChildLock, NULL);
    WdfFdoLockStaticChildListForIteration(Device);

    while ((hChild = WdfFdoRetrieveNextStaticChild(Device, hChild, WdfRetrieveAddedChildren)) != NULL) {
        pdoData = PdoGetData(hChild);

        if (HardwareType == pdoData->HardwareType) {
            unique = FALSE;
            status = STATUS_INVALID_PARAMETER;
            break;
        }
    }

    if (unique) {
        switch (HardwareType) {
        case HARDWARE_TYPE_GAMEPAD:
            GamepadCreatePdo(Device, HardwareType);
            break;

        case HARDWARE_TYPE_KEYBOARD:
            KeyboardCreatePdo(Device, HardwareType);
            break;

        case HARDWARE_TYPE_MOUSE:
            MouseCreatePdo(Device, HardwareType);
            break;

        default:
            TraceEvents(TRACE_LEVEL_WARNING, TRACE_DRIVER, "Do not plugin device - unknown hardware type %d", HardwareType);
        }
    }

    WdfFdoUnlockStaticChildListFromIteration(Device);
    WdfWaitLockRelease(deviceData->ChildLock);

    return status;
}

NTSTATUS
UnPlugDevice(
    _In_ WDFDEVICE      Device,
    _In_ HARDWARE_TYPE  HardwareType
)
{
    PPDO_DEVICE_DATA pdoData;
    BOOLEAN found = FALSE;
    BOOLEAN plugOutAll;
    WDFDEVICE hChild;
    NTSTATUS status = STATUS_INVALID_PARAMETER;

    PAGED_CODE();

    plugOutAll = (HARDWARE_TYPE_MAX == HardwareType) ? TRUE : FALSE;

    hChild = NULL;

    WdfFdoLockStaticChildListForIteration(Device);

    while ((hChild = WdfFdoRetrieveNextStaticChild(Device, hChild, WdfRetrieveAddedChildren)) != NULL) {
        if (plugOutAll) {
            status = WdfPdoMarkMissing(hChild);
            if (!NT_SUCCESS(status)) {
                TraceEvents(TRACE_LEVEL_ERROR, TRACE_DEVICE, "WdfPdoMarkMissing failed %!STATUS!", status);
                break;
            }

            found = TRUE;
        } else {
            pdoData = PdoGetData(hChild);

            if (HardwareType == pdoData->HardwareType) {
                status = WdfPdoMarkMissing(hChild);
                if (!NT_SUCCESS(status)) {
                    TraceEvents(TRACE_LEVEL_ERROR, TRACE_DEVICE, "WdfPdoMarkMissing failed %!STATUS!", status);
                    break;
                }

                found = TRUE;
                break;
            }
        }
    }

    WdfFdoUnlockStaticChildListFromIteration(Device);

    if (found) {
        status = STATUS_SUCCESS;
    }

    return status;
}

NTSTATUS
EjectDevice(
    _In_ WDFDEVICE      Device,
    _In_ HARDWARE_TYPE  HardwareType
)
{
    PPDO_DEVICE_DATA pdoData;
    BOOLEAN ejectAll;
    WDFDEVICE hChild;
    NTSTATUS status = STATUS_INVALID_PARAMETER;

    PAGED_CODE();

    ejectAll = (HARDWARE_TYPE_MAX == HardwareType) ? TRUE : FALSE;
    hChild = NULL;

    WdfFdoLockStaticChildListForIteration(Device);

    while ((hChild = WdfFdoRetrieveNextStaticChild(Device, hChild, WdfRetrieveAddedChildren)) != NULL) {
        pdoData = PdoGetData(hChild);

        if (ejectAll || HardwareType == pdoData->HardwareType) {
            status = STATUS_SUCCESS;
            WdfPdoRequestEject(hChild);
            if (!ejectAll) {
                break;
            }
        }
    }

    WdfFdoUnlockStaticChildListFromIteration(Device);

    return status;
}

NTSTATUS
DoStaticEnumeration(
    _In_ WDFDEVICE  Device
)
{
    NTSTATUS status = STATUS_SUCCESS;
    
    // plugin gamepad device
    status = PlugInDevice(Device, HARDWARE_TYPE_GAMEPAD);
    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DEVICE, "%!FUNC! PlugInDevice for GAMEPAD result %!STATUS!", status);

    // plugin keyboard device
    status = PlugInDevice(Device, HARDWARE_TYPE_KEYBOARD);
    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DEVICE, "%!FUNC! PlugInDevice for KEYBOARD result %!STATUS!", status);

    // plugin mouse device
    status = PlugInDevice(Device, HARDWARE_TYPE_MOUSE);
    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DEVICE, "%!FUNC! PlugInDevice for MOUSE result %!STATUS!", status);

    return status;
}
