#include "framework.h"
#include "vhidloader.h"

#pragma comment(lib, "swdevice.lib")

VOID WINAPI
CreationCallback(
    _In_     HSWDEVICE  hSwDevice,
    _In_     HRESULT    hrCreateResult,
    _In_opt_ PVOID      pContext,
    _In_opt_ PCWSTR     pszDeviceInstanceId
)
{
    HANDLE hEvent = (HANDLE)pContext;
    if (hEvent) SetEvent(hEvent);

    UNREFERENCED_PARAMETER(hSwDevice);
    UNREFERENCED_PARAMETER(hrCreateResult);
    UNREFERENCED_PARAMETER(pszDeviceInstanceId);
}

VHIDLoader::VHIDLoader()
    : m_hSwDevice(NULL)
{

}

VHIDLoader::~VHIDLoader()
{

}

int VHIDLoader::Load()
{
    HANDLE hEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (hEvent == NULL) return -1;

    SW_DEVICE_CREATE_INFO createInfo = { 0 };
    PCWSTR description = L"vhiddev";

    // These match the Pnp id's in the inf file so OS will load the driver when the device is created    
    PCWSTR instanceId = L"vhiddev";
    PCWSTR hardwareIds = L"vhiddev\0\0";
    PCWSTR compatibleIds = L"vhiddev\0\0";

    createInfo.cbSize = sizeof(createInfo);
    createInfo.pszzCompatibleIds = compatibleIds;
    createInfo.pszInstanceId = instanceId;
    createInfo.pszzHardwareIds = hardwareIds;
    createInfo.pszDeviceDescription = description;

    createInfo.CapabilityFlags = SWDeviceCapabilitiesRemovable |
        SWDeviceCapabilitiesSilentInstall |
        SWDeviceCapabilitiesDriverRequired;

    // Create the device
    HRESULT hr = SwDeviceCreate(L"VHIDDevice",
        L"HTREE\\ROOT\\0",
        &createInfo,
        0,
        nullptr,
        CreationCallback,
        (PVOID)hEvent,
        &m_hSwDevice);
    if (FAILED(hr)) return -2;

    // Wait for callback to signal that the device has been created
    DWORD waitResult = WaitForSingleObject(hEvent, 10 * 1000);
    if (waitResult != WAIT_OBJECT_0) return -3;

    return 0;
}

int VHIDLoader::Unload()
{
    SwDeviceClose(m_hSwDevice);
    m_hSwDevice = NULL;
    return 0;
}
