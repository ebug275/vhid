#pragma once

EXTERN_C_START

//
// The device extension for the PDOs.
// That's of the toaster device which this bus driver enumerates.
//
typedef struct _PDO_DEVICE_DATA
{
    UCHAR               HardwareType;
    WDFDEVICE           FdoDevice;
    WDFIOTARGET         IoTargetToParentBusFdo;
    WDFQUEUE            PendingQueue;

    PUCHAR              DeviceDescriptor;
    PUCHAR              ConfigurationDescriptor;
    PUCHAR              ReportDescriptor;
} PDO_DEVICE_DATA, * PPDO_DEVICE_DATA;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(PDO_DEVICE_DATA, PdoGetData)

//
// The device extension of the bus itself.  From whence the PDO's are born.
//
typedef struct _FDO_DEVICE_DATA
{
    WDFWAITLOCK         ChildLock;
    WDFIOTARGET	        IoTargetToSelf;
    PDEVICE_OBJECT      DeviceObjectToSelf;
    WDFDEVICE           PdoChilds[HARDWARE_TYPE_MAX];
} FDO_DEVICE_DATA, * PFDO_DEVICE_DATA;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(FDO_DEVICE_DATA, FdoGetData)

//
// Function to initialize the device and its callbacks
//
EVT_WDF_DEVICE_PREPARE_HARDWARE EvtDevicePrepareHardware;
EVT_WDF_DEVICE_RELEASE_HARDWARE EvtDeviceReleaseHardware;
EVT_WDF_DEVICE_D0_ENTRY EvtDeviceD0Entry;
EVT_WDF_DEVICE_D0_EXIT EvtDeviceD0Exit;

//
// Function to initialize the device and its callbacks
//
NTSTATUS
CreateDevice(
    _Inout_ PWDFDEVICE_INIT DeviceInit
    );

NTSTATUS
DoStaticEnumeration(
    _In_ WDFDEVICE Device
);

EXTERN_C_END
