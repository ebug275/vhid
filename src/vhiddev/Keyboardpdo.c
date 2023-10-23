#include "driver.h"
#include "keyboardpdo.tmh"

#ifdef ALLOC_PRAGMA
#pragma alloc_text(PAGE, KeyboardCreatePdo)
#endif

UCHAR device_descriptor_keyboard[] = {
    // 18 bytes
    0x12,        // bLength
    0x01,        // bDescriptorType (Device)
    0x10, 0x01,  // bcdUSB 1.10
    0x00,        // bDeviceClass (Use class information in the Interface Descriptors)
    0x00,        // bDeviceSubClass 
    0x00,        // bDeviceProtocol 
    0x40,        // bMaxPacketSize0 64
    LOBYTE(HARDWARE_VENDOR_ID),
    HIBYTE(HARDWARE_VENDOR_ID),   // idVendor 0xEF99 (unused vendor id)
    LOBYTE(KEYBOARD_PRODUCT_ID),
    HIBYTE(KEYBOARD_PRODUCT_ID),  // idProduct 0x0821
    0x00, 0x01,  // bcdDevice 1.00
    0x01,        // iManufacturer (String Index)
    0x02,        // iProduct (String Index)
    0x00,        // iSerialNumber (String Index)
    0x01,        // bNumConfigurations 1
};

UCHAR configuration_descriptor_keyboard[] = {
    // 34 bytes
    0x09,        // bLength
    0x02,        // bDescriptorType (Configuration)
    0x22, 0x00,  // wTotalLength 34
    0x01,        // bNumInterfaces 1
    0x01,        // bConfigurationValue
    0x00,        // iConfiguration (String Index)
    0xA0,        // bmAttributes
    0x32,        // bMaxPower 100mA

    0x09,        // bLength
    0x04,        // bDescriptorType (Interface)
    0x00,        // bInterfaceNumber 0
    0x00,        // bAlternateSetting
    0x01,        // bNumEndpoints 1
    0x03,        // bInterfaceClass
    0x00,        // bInterfaceSubClass
    0x00,        // bInterfaceProtocol
    0x00,        // iInterface (String Index)

    0x09,        // bLength
    0x21,        // bDescriptorType (HID)
    0x10, 0x01,  // bcdHID 1.10
    0x00,        // bCountryCode
    0x01,        // bNumDescriptors
    0x22,        // bDescriptorType[0] (HID)
    0x3F, 0x00,  // wDescriptorLength[0] 63

    0x07,        // bLength
    0x05,        // bDescriptorType (Endpoint)
    0x81,        // bEndpointAddress (IN/D2H)
    0x03,        // bmAttributes (Interrupt)
    0x40, 0x00,  // wMaxPacketSize 64
    0x01,        // bInterval 1 (unit depends on device speed)
};

UCHAR report_desciptor_keyboard[] = {
    // keyboard - 63 bytes
    0x05, 0x01,                 // Usage Page (Generic Desktop Ctrls)
    0x09, 0x06,                 // Usage (Keyboard)
    0xA1, 0x01,                 // Collection (Application)
    0x05, 0x07,                 //   Usage Page (Kbrd/Keypad)
    0x19, 0xE0,                 //   Usage Minimum (0xE0)
    0x29, 0xE7,                 //   Usage Maximum (0xE7)
    0x15, 0x00,                 //   Logical Minimum (0)
    0x25, 0x01,                 //   Logical Maximum (1)
    0x75, 0x01,                 //   Report Size (1)
    0x95, 0x08,                 //   Report Count (8)
    0x81, 0x02,                 //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x95, 0x01,                 //   Report Count (1)
    0x75, 0x08,                 //   Report Size (8)
    0x81, 0x01,                 //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x95, 0x05,                 //   Report Count (5)
    0x75, 0x01,                 //   Report Size (1)
    0x05, 0x08,                 //   Usage Page (LEDs)
    0x19, 0x01,                 //   Usage Minimum (Num Lock)
    0x29, 0x05,                 //   Usage Maximum (Kana)
    0x91, 0x02,                 //   Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x95, 0x01,                 //   Report Count (1)
    0x75, 0x03,                 //   Report Size (3)
    0x91, 0x01,                 //   Output (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x95, 0x06,                 //   Report Count (6)
    0x75, 0x08,                 //   Report Size (8)
    0x15, 0x00,                 //   Logical Minimum (0)
    0x25, 0x65,                 //   Logical Maximum (101)
    0x05, 0x07,                 //   Usage Page (Kbrd/Keypad)
    0x19, 0x00,                 //   Usage Minimum (0)
    0x29, 0x65,                 //   Usage Maximum (101)
    0x81, 0x00,                 //   Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,                       // End Collection
};

UCHAR keyboard_strings[][64] = {
    { 0x04, 0x03, 0x09, 0x00 },
    { 0x16, 0x03, 0x4D, 0x00, 0x46, 0x00, 0x52, 0x00, 0x5F, 0x00, 0x53, 0x00, 0x54, 0x00, 0x52, 0x00,
      0x49, 0x00, 0x4E, 0x00, 0x47, 0x00 }, // MFR_STRING
    { 0x1C, 0x03, 0x56, 0x00, 0x48, 0x00, 0x49, 0x00, 0x44, 0x00, 0x5F, 0x00, 0x4B, 0x00, 0x45, 0x00,
      0x59, 0x00, 0x42, 0x00, 0x4F, 0x00, 0x41, 0x00, 0x52, 0x00, 0x44, 0x00 }, // VHID_KEYBOARD
    { 0x1A, 0x03, 0x53, 0x00, 0x4E, 0x00, 0x31, 0x00, 0x32, 0x00, 0x33, 0x00, 0x34, 0x00, 0x35, 0x00,
      0x36, 0x00, 0x37, 0x00, 0x38, 0x00, 0x39, 0x00, 0x30, 0x00 } // SN1234567890
};

NTSTATUS
KeyboardCreatePdo(
    _In_  WDFDEVICE      Device,
    _In_  HARDWARE_TYPE  HardwareType
)
{
    NTSTATUS status = STATUS_SUCCESS;
    PWDFDEVICE_INIT pDeviceInit = NULL;
    PFDO_DEVICE_DATA fdoData = NULL;
    PPDO_DEVICE_DATA pdoData = NULL;
    WDFDEVICE hChild = NULL;
    WDF_QUERY_INTERFACE_CONFIG qiConfig;
    WDF_OBJECT_ATTRIBUTES pdoAttributes;
    WDF_DEVICE_PNP_CAPABILITIES pnpCaps;
    WDF_DEVICE_POWER_CAPABILITIES powerCaps;
    PDO_INTERFACE_STANDARD ChildInterface;

    UNREFERENCED_PARAMETER(HardwareType);

    DECLARE_CONST_UNICODE_STRING(hardwareId, KEYBOARD_HARDWARE_ID);
    DECLARE_CONST_UNICODE_STRING(compatId, KEYBOARD_COMPATIBLE_ID);
    DECLARE_CONST_UNICODE_STRING(deviceLocation, L"mocon 1");
    DECLARE_UNICODE_STRING_SIZE(buffer, MAX_ID_LEN);

    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_PDO, "%!FUNC! serial no = %d Entry", HardwareType);

    PAGED_CODE();

    fdoData = FdoGetData(Device);

    // WDFDEVICE_INIT 구조체 생성
    pDeviceInit = WdfPdoInitAllocate(Device);
    if (pDeviceInit == NULL) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_PDO, "%!FUNC! WdfPdoInitAllocate failed");
        status = STATUS_INSUFFICIENT_RESOURCES;
        goto Cleanup;
    }

    // DeviceType 지정
    WdfDeviceInitSetDeviceType(pDeviceInit, FILE_DEVICE_BUS_EXTENDER);

    // Device ID 지정
    status = WdfPdoInitAssignDeviceID(pDeviceInit, &hardwareId);
    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_PDO, "%!FUNC! WdfPdoInitAssignDeviceID failed %!STATUS!", status);
        goto Cleanup;
    }

    // Hardware ID 지정
    status = WdfPdoInitAddHardwareID(pDeviceInit, &hardwareId);
    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_PDO, "%!FUNC! WdfPdoInitAddHardwareID failed %!STATUS!", status);
        goto Cleanup;
    }

    // Compatible ID 지정
    status = WdfPdoInitAddCompatibleID(pDeviceInit, &compatId);
    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_PDO, "%!FUNC! WdfPdoInitAddCompatibleID failed %!STATUS!", status);
        goto Cleanup;
    }

    // Instance ID 지정
    status = RtlUnicodeStringPrintf(&buffer, L"%02d", HARDWARE_TYPE_KEYBOARD);
    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_PDO, "%!FUNC! RtlUnicodeStringPrintf for instance id failed %!STATUS!", status);
        goto Cleanup;
    }

    status = WdfPdoInitAssignInstanceID(pDeviceInit, &buffer);
    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_PDO, "%!FUNC! WdfPdoInitAssignInstanceID failed %!STATUS!", status);
        goto Cleanup;
    }

    // Device Text 지정
    // 드라이버가 설치되지 않았을 때, 장치관리자에 기본으로 표시될 텍스트
    // locale을 다르게 여러번 호출 가능
    status = RtlUnicodeStringPrintf(&buffer, L"VHIDDEV_Device_%02d", HARDWARE_TYPE_KEYBOARD);
    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_PDO, "%!FUNC! RtlUnicodeStringPrintf for device text failed %!STATUS!", status);
        goto Cleanup;
    }

    status = WdfPdoInitAddDeviceText(pDeviceInit, &buffer, &deviceLocation, 0x409);
    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_PDO, "%!FUNC! WdfPdoInitAddDeviceText failed %!STATUS!", status);
        goto Cleanup;
    }

    // WdfRequestForwardToParentDeviceIoQueue를 사용하여 pdo에서 parent fdo로 request를 전달하기 위해서 호출
    // WdfDeviceCreate를 사용하여 pdo를 생성하기 전에 호출해 주어야 함
    WdfPdoInitAllowForwardingRequestToParent(pDeviceInit);

    WdfPdoInitSetDefaultLocale(pDeviceInit, 0x409);

    // pdo의 device extension을 설정
    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&pdoAttributes, PDO_DEVICE_DATA);

    // child pdo 생성
    status = WdfDeviceCreate(&pDeviceInit, &pdoAttributes, &hChild);
    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_PDO, "%!FUNC! WdfDeviceCreate failed %!STATUS!", status);
        goto Cleanup;
    }

    // pdo의 device extension을 가져와서 초기화 작업
    pdoData = PdoGetData(hChild);
    RtlZeroMemory(pdoData, sizeof(PPDO_DEVICE_DATA));

    pdoData->HardwareType = HARDWARE_TYPE_KEYBOARD;
    pdoData->FdoDevice = Device;
    pdoData->DeviceDescriptor = device_descriptor_keyboard;
    pdoData->ConfigurationDescriptor = configuration_descriptor_keyboard;
    pdoData->ReportDescriptor = report_desciptor_keyboard;

    // pdo의 pnp 속성 지정
    WDF_DEVICE_PNP_CAPABILITIES_INIT(&pnpCaps);
    //pnpCaps.Removable = WdfTrue;
    //pnpCaps.EjectSupported = WdfTrue;
    pnpCaps.SurpriseRemovalOK = WdfTrue;

    pnpCaps.Address = HARDWARE_TYPE_KEYBOARD;
    pnpCaps.UINumber = HARDWARE_TYPE_KEYBOARD;

    WdfDeviceSetPnpCapabilities(hChild, &pnpCaps);

    // pdo의 power 속성 지정
    WDF_DEVICE_POWER_CAPABILITIES_INIT(&powerCaps);

    powerCaps.DeviceD1 = WdfTrue;
    powerCaps.WakeFromD1 = WdfTrue;
    powerCaps.DeviceWake = PowerDeviceD1;

    powerCaps.DeviceState[PowerSystemWorking] = PowerDeviceD0;
    powerCaps.DeviceState[PowerSystemSleeping1] = PowerDeviceD1;
    powerCaps.DeviceState[PowerSystemSleeping2] = PowerDeviceD3;
    powerCaps.DeviceState[PowerSystemSleeping3] = PowerDeviceD3;
    powerCaps.DeviceState[PowerSystemHibernate] = PowerDeviceD3;
    powerCaps.DeviceState[PowerSystemShutdown] = PowerDeviceD3;

    WdfDeviceSetPowerCapabilities(hChild, &powerCaps);

    //
    // Create a custom interface so that other drivers can
    // query (IRP_MN_QUERY_INTERFACE) and use our callbacks directly.
    //
    RtlZeroMemory(&ChildInterface, sizeof(ChildInterface));

    ChildInterface.InterfaceHeader.Size = sizeof(ChildInterface);
    ChildInterface.InterfaceHeader.Version = 1;
    ChildInterface.InterfaceHeader.Context = (PVOID)hChild;

    //
    // Let the framework handle reference counting.
    //
    ChildInterface.InterfaceHeader.InterfaceReference = WdfDeviceInterfaceReferenceNoOp;
    ChildInterface.InterfaceHeader.InterfaceDereference = WdfDeviceInterfaceDereferenceNoOp;

    ChildInterface.GetCrispinessLevel = KeyboardGetCrispinessLevel;
    ChildInterface.SetCrispinessLevel = KeyboardSetCrispinessLevel;
    ChildInterface.IsSafetyLockEnabled = KeyboardIsSafetyLockEnabled;

    WDF_QUERY_INTERFACE_CONFIG_INIT(&qiConfig, (PINTERFACE)&ChildInterface, &GUID_VHIDDEV_INTERFACE_KEYBOARD, NULL);

    // pdo에 query interface 정보를 지정
    status = WdfDeviceAddQueryInterface(hChild, &qiConfig);
    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_PDO, "%!FUNC! WdfDeviceAddQueryInterface failed %!STATUS!", status);
        goto Cleanup;
    }

    // pdo에 전달되는 request를 처리할 queue를 생성
    status = KeyboardQueueInitialize(hChild);
    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_PDO, "%!FUNC! KeyboardQueueInitialize failed %!STATUS!", status);
        goto Cleanup;
    }

    //// 생성한 pdo를 parent fdo의 child 목록에 추가
    //// fdo에 추가된 이후에는 WdfObjectDelete 말고 WdfPdoMarkMissing를 이용해서 device를 삭제해야함
    status = WdfFdoAddStaticChild(Device, hChild);
    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_PDO, "%!FUNC! WdfFdoAddStaticChild failed %!STATUS!", status);
        goto Cleanup;
    }

    fdoData->PdoChilds[HARDWARE_TYPE_KEYBOARD] = hChild;

    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_PDO, "%!FUNC! PdoChild = %p Exit", hChild);

    return status;

Cleanup:
    TraceEvents(TRACE_LEVEL_ERROR, TRACE_PDO, "%!FUNC! failed %!STATUS!", status);

    // child pdo 생성에 실패하면, 생성해 놓은 리소스를 제거
    if (pDeviceInit != NULL) {
        WdfDeviceInitFree(pDeviceInit);
    }

    if (hChild) {
        WdfObjectDelete(hChild);
    }

    return status;
}

BOOLEAN
KeyboardGetCrispinessLevel(
    _In_  WDFDEVICE  ChildDevice,
    _Out_ PUCHAR     Level
)
{
    UNREFERENCED_PARAMETER(ChildDevice);

    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_PDO, "%!FUNC! Entry");

    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_PDO, "%!FUNC! Exit");

    *Level = 10;
    return TRUE;
}

BOOLEAN
KeyboardSetCrispinessLevel(
    _In_ WDFDEVICE  ChildDevice,
    _In_ UCHAR      Level
)
{
    UNREFERENCED_PARAMETER(ChildDevice);
    UNREFERENCED_PARAMETER(Level);

    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_PDO, "%!FUNC! Entry");

    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_PDO, "%!FUNC! Exit");

    return TRUE;
}

BOOLEAN
KeyboardIsSafetyLockEnabled(
    _In_ WDFDEVICE  ChildDevice
)
{
    UNREFERENCED_PARAMETER(ChildDevice);

    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_PDO, "%!FUNC! Entry");

    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_PDO, "%!FUNC! Exit");

    return TRUE;
}

NTSTATUS
KeyboardQueueInitialize(
    _In_ WDFDEVICE  Device
)
{
    PPDO_DEVICE_DATA pdoData;
    WDFQUEUE queue;
    NTSTATUS status;
    WDF_IO_QUEUE_CONFIG queueConfig;
    WDF_OBJECT_ATTRIBUTES queueAttributes;
    PPENDING_QUEUE_CONTEXT queueContext;

    PAGED_CODE();

    pdoData = PdoGetData(Device);

    // internal request를 전달받을 기본 queue를 생성
    WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(&queueConfig, WdfIoQueueDispatchParallel);
    queueConfig.EvtIoInternalDeviceControl = KeyboardEvtIoInternalDeviceControl;
    queueConfig.EvtIoStop = KeyboardEvtIoStop;

    status = WdfIoQueueCreate(Device, &queueConfig, WDF_NO_OBJECT_ATTRIBUTES, &queue);
    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_PDO, "%!FUNC! WdfIoQueueCreate (default queue) failed %!STATUS!", status);
        return status;
    }

    // interrupt 요청에 대해서 저장할 queue 생성
    WDF_IO_QUEUE_CONFIG_INIT(&queueConfig, WdfIoQueueDispatchManual);
    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&queueAttributes, PENDING_QUEUE_CONTEXT);

    status = WdfIoQueueCreate(Device, &queueConfig, &queueAttributes, &queue);
    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_PDO, "%!FUNC! WdfIoQueueCreate (pending queue) failed %!STATUS!", status);
        return status;
    }

    queueContext = GetPendingQueueContext(queue);
    queueContext->Queue = queue;
    queueContext->PdoData = pdoData;

    pdoData->PendingQueue = queue;

    return status;
}

NTSTATUS
KeyboardCompleteReport(
    _In_ WDFDEVICE   Device,
    _In_ WDFREQUEST  Request
)
{
    NTSTATUS status = STATUS_INVALID_PARAMETER;
    WDFREQUEST request;
    PPDO_DEVICE_DATA pdoData = NULL;
    PKEYBOARD_REPORT keyboard_report = NULL;
    PVOID buffer = NULL;
    size_t bufSize = 0;

    WDF_REQUEST_PARAMETERS params;
    PURB urb;

    pdoData = PdoGetData(Device);

    status = WdfRequestRetrieveInputBuffer(Request, sizeof(KEYBOARD_REPORT), &buffer, &bufSize);
    if (!NT_SUCCESS(status) || bufSize < sizeof(KEYBOARD_REPORT)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_PDO, "%!FUNC! not enough input request buffer failed");
        return status;
    }

    keyboard_report = (PKEYBOARD_REPORT)buffer;

    // todo: keyboard report data update

    status = WdfIoQueueRetrieveNextRequest(pdoData->PendingQueue, &request);

    if (NT_SUCCESS(status)) {
        WDF_REQUEST_PARAMETERS_INIT(&params);
        WdfRequestGetParameters(request, &params);
        urb = params.Parameters.Others.Arg1;

        if (urb->UrbHeader.Function == URB_FUNCTION_BULK_OR_INTERRUPT_TRANSFER) {
            if (urb->UrbBulkOrInterruptTransfer.TransferBufferLength >= sizeof(KEYBOARD_REPORT)) {
                RtlCopyMemory(urb->UrbBulkOrInterruptTransfer.TransferBuffer, keyboard_report, sizeof(KEYBOARD_REPORT));
                urb->UrbBulkOrInterruptTransfer.TransferBufferLength = sizeof(KEYBOARD_REPORT);
                status = STATUS_SUCCESS;
            } else {
                TraceEvents(TRACE_LEVEL_ERROR, TRACE_PDO, "%!FUNC! request buffer size (%d bytes) not enough", urb->UrbBulkOrInterruptTransfer.TransferBufferLength);
                status = STATUS_INVALID_PARAMETER;
            }
        } else {
            TraceEvents(TRACE_LEVEL_ERROR, TRACE_PDO, "%!FUNC! request is not URB_FUNCTION_BULK_OR_INTERRUPT_TRANSFER");
            status = STATUS_INVALID_PARAMETER;
        }

        WdfRequestComplete(request, status);
    }

    return status;
}

VOID
KeyboardEvtIoInternalDeviceControl(
    _In_ WDFQUEUE    Queue,
    _In_ WDFREQUEST  Request,
    _In_ size_t      OutputBufferLength,
    _In_ size_t      InputBufferLength,
    _In_ ULONG       IoControlCode
)
{
    NTSTATUS status = STATUS_INVALID_PARAMETER;
    WDF_REQUEST_PARAMETERS params;
    WDFDEVICE hDevice;
    PPDO_DEVICE_DATA pdoData = NULL;
    PURB urb;

    UNREFERENCED_PARAMETER(OutputBufferLength);
    UNREFERENCED_PARAMETER(InputBufferLength);

    hDevice = WdfIoQueueGetDevice(Queue);
    pdoData = PdoGetData(hDevice);

    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_PDO,
        "%!FUNC! Queue 0x%p, Request 0x%p OutputBufferLength %d InputBufferLength %d IoControlCode %d",
        Queue, Request, (int)OutputBufferLength, (int)InputBufferLength, IoControlCode);

    WDF_REQUEST_PARAMETERS_INIT(&params);
    WdfRequestGetParameters(Request, &params);

    switch (IoControlCode) {
    case IOCTL_INTERNAL_USB_SUBMIT_URB:
        urb = params.Parameters.Others.Arg1;
        status = KeyboardUrbProcess(pdoData, Request, urb);
        break;
    }

    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_PDO, "%!FUNC! Request error %!STATUS!", status);
        WdfRequestComplete(Request, status);
    }

    return;
}

VOID
KeyboardEvtIoStop(
    _In_ WDFQUEUE    Queue,
    _In_ WDFREQUEST  Request,
    _In_ ULONG       ActionFlags
)
{
    UNREFERENCED_PARAMETER(Queue);
    UNREFERENCED_PARAMETER(Request);
    UNREFERENCED_PARAMETER(ActionFlags);

    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_PDO,
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

NTSTATUS
KeyboardUrbProcess(
    _In_ PPDO_DEVICE_DATA  PdoData,
    _In_ WDFREQUEST        Request,
    _In_ PURB              Urb
)
{
    NTSTATUS status = STATUS_SUCCESS;

    switch (Urb->UrbHeader.Function) {
    case URB_FUNCTION_SELECT_CONFIGURATION:
        TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_PDO, "URB_FUNCTION_SELECT_CONFIGURATION");
        status = KeyboardSelectConfiguration(PdoData, Request, Urb);
        break;

    case URB_FUNCTION_GET_DESCRIPTOR_FROM_DEVICE:
        TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_PDO, "URB_FUNCTION_GET_DESCRIPTOR_FROM_DEVICE");
        status = KeyboardGetUsbDescriptor(PdoData, Request, Urb);
        break;

    case URB_FUNCTION_GET_DESCRIPTOR_FROM_INTERFACE:
        TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_PDO, "URB_FUNCTION_GET_DESCRIPTOR_FROM_INTERFACE");
        status = KeyboardGetUsbDescriptor(PdoData, Request, Urb);
        break;

    case URB_FUNCTION_CLASS_INTERFACE:
        TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_PDO, "URB_FUNCTION_CLASS_INTERFACE");
        status = KeyboardProcessClassInterface(PdoData, Request, Urb);
        break;

    case URB_FUNCTION_CONTROL_TRANSFER_EX:
        TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_PDO, "URB_FUNCTION_CONTROL_TRANSFER_EX");
        status = KeyboardProcessControlTransfer(PdoData, Request, Urb);
        break;

    case URB_FUNCTION_BULK_OR_INTERRUPT_TRANSFER:
        TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_PDO, "URB_FUNCTION_BULK_OR_INTERRUPT_TRANSFER");
        status = WdfRequestForwardToIoQueue(Request, PdoData->PendingQueue);;
        break;

    default:
        TraceEvents(TRACE_LEVEL_WARNING, TRACE_PDO, "unknown urb function 0x%x", Urb->UrbHeader.Function);
        status = STATUS_UNSUCCESSFUL;
    }

    return status;
}

NTSTATUS
KeyboardSelectConfiguration(
    _In_ PPDO_DEVICE_DATA  PdoData,
    _In_ WDFREQUEST        Request,
    _In_ PURB              Urb
)
{
    NTSTATUS status = STATUS_SUCCESS;
    PUSBD_INTERFACE_INFORMATION intf_info = NULL;

    UNREFERENCED_PARAMETER(PdoData);

    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_PDO,
        "%!FUNC! select configuration descriptor: 0x%p, handle: 0x%p, length: %d, pipes: %d, status: 0x%x",
        Urb->UrbSelectConfiguration.ConfigurationDescriptor,
        Urb->UrbSelectConfiguration.ConfigurationHandle,
        Urb->UrbSelectConfiguration.Interface.Length,
        Urb->UrbSelectConfiguration.Interface.NumberOfPipes,
        Urb->UrbHeader.Status);

    if (Urb->UrbSelectConfiguration.ConfigurationDescriptor != NULL) {
        DumpMemory((PUCHAR)Urb->UrbSelectConfiguration.ConfigurationDescriptor, Urb->UrbSelectConfiguration.ConfigurationDescriptor->wTotalLength);

        Urb->UrbSelectConfiguration.ConfigurationHandle = &configuration_descriptor_keyboard[0];

        USB_INTERFACE_DESCRIPTOR* intf = (USB_INTERFACE_DESCRIPTOR*)&configuration_descriptor_keyboard[9];
        intf_info = &Urb->UrbSelectConfiguration.Interface;
        intf_info->Class = intf->bInterfaceClass;
        intf_info->SubClass = intf->bInterfaceSubClass;
        intf_info->Protocol = intf->bInterfaceProtocol;

        USB_ENDPOINT_DESCRIPTOR* endp = (USB_ENDPOINT_DESCRIPTOR*)&configuration_descriptor_keyboard[27];
        intf_info->Pipes[0].MaximumPacketSize = endp->wMaxPacketSize;
        intf_info->Pipes[0].EndpointAddress = endp->bEndpointAddress;
        intf_info->Pipes[0].Interval = endp->bInterval;
        intf_info->Pipes[0].PipeType = UsbdPipeTypeInterrupt;
        intf_info->Pipes[0].PipeHandle = endp;
    }

    if (NT_SUCCESS(status)) {
        WdfRequestComplete(Request, status);
    }

    return status;
}

NTSTATUS
KeyboardGetUsbDescriptor(
    _In_ PPDO_DEVICE_DATA  PdoData,
    _In_ WDFREQUEST        Request,
    _In_ PURB              Urb
)
{
    NTSTATUS status = STATUS_SUCCESS;
    PFDO_DEVICE_DATA pDeviceContext;
    ULONG length = 0;
    PVOID ioBuffer;

    pDeviceContext = FdoGetData(PdoData->FdoDevice);

    switch (Urb->UrbControlDescriptorRequest.DescriptorType) {
    case USB_DEVICE_DESCRIPTOR_TYPE:
        TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_PDO, "USB_DEVICE_DESCRIPTOR_TYPE");
        length = sizeof(device_descriptor_keyboard);

        ioBuffer = Urb->UrbControlDescriptorRequest.TransferBuffer;
        if (length > Urb->UrbControlDescriptorRequest.TransferBufferLength) length = Urb->UrbControlDescriptorRequest.TransferBufferLength;

        DumpMemory((PUCHAR)&device_descriptor_keyboard, length);
        RtlCopyMemory(ioBuffer, &device_descriptor_keyboard, length);

        status = STATUS_SUCCESS;

        break;

    case USB_CONFIGURATION_DESCRIPTOR_TYPE:
        TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_PDO, "USB_CONFIGURATION_DESCRIPTOR_TYPE");
        length = sizeof(configuration_descriptor_keyboard);

        ioBuffer = Urb->UrbControlDescriptorRequest.TransferBuffer;
        if (length > Urb->UrbControlDescriptorRequest.TransferBufferLength) length = Urb->UrbControlDescriptorRequest.TransferBufferLength;

        DumpMemory((PUCHAR)&configuration_descriptor_keyboard, length);
        RtlCopyMemory(ioBuffer, &configuration_descriptor_keyboard, length);

        status = STATUS_SUCCESS;

        break;

    case USB_STRING_DESCRIPTOR_TYPE:
        TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_PDO, "USB_STRING_DESCRIPTOR_TYPE");
        length = (ULONG)keyboard_strings[Urb->UrbControlDescriptorRequest.Index][0];

        ioBuffer = Urb->UrbControlDescriptorRequest.TransferBuffer;
        if (length > Urb->UrbControlDescriptorRequest.TransferBufferLength) length = Urb->UrbControlDescriptorRequest.TransferBufferLength;

        DumpMemory(keyboard_strings[Urb->UrbControlDescriptorRequest.Index], length);
        RtlCopyMemory(ioBuffer, keyboard_strings[Urb->UrbControlDescriptorRequest.Index], length);

        status = STATUS_SUCCESS;

        break;

    case HID_REPORT_DESCRIPTOR_TYPE:
        TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_PDO, "HID_REPORT_DESCRIPTOR_TYPE");
        length = sizeof(report_desciptor_keyboard);

        ioBuffer = Urb->UrbControlDescriptorRequest.TransferBuffer;
        if (length > Urb->UrbControlDescriptorRequest.TransferBufferLength) length = Urb->UrbControlDescriptorRequest.TransferBufferLength;

        DumpMemory(report_desciptor_keyboard, length);
        RtlCopyMemory(ioBuffer, report_desciptor_keyboard, length);

        status = STATUS_SUCCESS;

        break;

    default:
        TraceEvents(TRACE_LEVEL_WARNING, TRACE_PDO, "%!FUNC! unknwon descriptor type 0x%x", Urb->UrbControlDescriptorRequest.DescriptorType);
        status = STATUS_UNSUCCESSFUL;
    }

    if (NT_SUCCESS(status)) {
        WdfRequestComplete(Request, status);
    }

    return status;
}

NTSTATUS
KeyboardProcessClassInterface(
    _In_ PPDO_DEVICE_DATA  PdoData,
    _In_ WDFREQUEST        Request,
    _In_ PURB              Urb
)
{
    NTSTATUS status = STATUS_SUCCESS;

    UNREFERENCED_PARAMETER(PdoData);

    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_PDO,
        "%!FUNC! request code: 0x%x, value: 0x%x, index: 0x%x, flag: 0x%x, buffer: 0x%p(MDL: 0x%p), buffer_len: %d, status: 0x%x",
        Urb->UrbControlVendorClassRequest.Request,
        Urb->UrbControlVendorClassRequest.Value,
        Urb->UrbControlVendorClassRequest.Index,
        Urb->UrbControlVendorClassRequest.TransferFlags,
        Urb->UrbControlVendorClassRequest.TransferBuffer,
        Urb->UrbControlVendorClassRequest.TransferBufferMDL,
        Urb->UrbControlVendorClassRequest.TransferBufferLength,
        Urb->UrbHeader.Status);

    if (Urb->UrbControlVendorClassRequest.TransferFlags & USBD_TRANSFER_DIRECTION_IN) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_PDO, "%!FUNC! cannot support direction in vendor or class transfer request");
        status = STATUS_INVALID_PARAMETER;
    }

    if (NT_SUCCESS(status)) {
        DumpMemory(Urb->UrbControlVendorClassRequest.TransferBuffer, Urb->UrbControlVendorClassRequest.TransferBufferLength);
        WdfRequestComplete(Request, status);
    }

    return status;
}

NTSTATUS
KeyboardProcessControlTransfer(
    _In_ PPDO_DEVICE_DATA  PdoData,
    _In_ WDFREQUEST        Request,
    _In_ PURB              Urb
)
{
    NTSTATUS status = STATUS_SUCCESS;

    UNREFERENCED_PARAMETER(PdoData);

    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_PDO,
        "%!FUNC! request pipe: 0x%p, flag: 0x%x, buffer: 0x%p(MDL: 0x%p), buffer_len: %d, timeout: %d, status: 0x%x",
        Urb->UrbControlTransferEx.PipeHandle,
        Urb->UrbControlTransferEx.TransferFlags,
        Urb->UrbControlTransferEx.TransferBuffer,
        Urb->UrbControlTransferEx.TransferBufferMDL,
        Urb->UrbControlTransferEx.TransferBufferLength,
        Urb->UrbControlTransferEx.Timeout,
        Urb->UrbHeader.Status);

    DumpMemory(Urb->UrbControlTransferEx.SetupPacket, sizeof(Urb->UrbControlTransferEx.SetupPacket));

    if (Urb->UrbControlTransferEx.TransferFlags & USBD_TRANSFER_DIRECTION_IN) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_PDO, "%!FUNC! cannot support direction in control transfer request");
        status = STATUS_INVALID_PARAMETER;
    }

    if (NT_SUCCESS(status)) {
        DumpMemory(Urb->UrbControlTransferEx.TransferBuffer, Urb->UrbControlTransferEx.TransferBufferLength);
        WdfRequestComplete(Request, status);
    }

    return status;
}
