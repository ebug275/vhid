#include "pch.h"
#include "vhidlib.h"

#pragma comment(lib, "setupapi.lib") 

GAMEPAD_REPORT g_gamepad_report;
KEYBOARD_REPORT g_keyboard_report;
MOUSE_REPORT g_mouse_report;

#pragma region device_export_functions
HANDLE vhidlib_open_device()
{
	HANDLE device = INVALID_HANDLE_VALUE;

	HDEVINFO dev_info;
	SP_DEVINFO_DATA dev_info_data;
	SP_DEVICE_INTERFACE_DATA dev_interface_data;
	PSP_DEVICE_INTERFACE_DETAIL_DATA dev_interface_detail = NULL;
	int dev_index = 0;
	ULONG length;
	ULONG requiredLength = 0;

	ZeroMemory(&g_gamepad_report, sizeof(GAMEPAD_REPORT));
	ZeroMemory(&g_keyboard_report, sizeof(KEYBOARD_REPORT));
	ZeroMemory(&g_mouse_report, sizeof(MOUSE_REPORT));

	dev_info = SetupDiGetClassDevs(&GUID_DEVINTERFACE_VHIDDEV, NULL, NULL, DIGCF_DEVICEINTERFACE);

	dev_interface_data.cbSize = sizeof(SP_INTERFACE_DEVICE_DATA);
	while (SetupDiEnumDeviceInterfaces(dev_info, NULL, &GUID_DEVINTERFACE_VHIDDEV, dev_index, &dev_interface_data))
	{
		if (!SetupDiGetDeviceInterfaceDetail(dev_info, &dev_interface_data, NULL, 0, &requiredLength, NULL))
		{
			if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
			{
				OutputDebugString(_T("SetupDiGetDeviceInterfaceDetail error (not resource error)\n"));
				break;
			}
		}

		dev_interface_detail = (PSP_DEVICE_INTERFACE_DETAIL_DATA)LocalAlloc(LMEM_FIXED, requiredLength);
		if (NULL == dev_interface_detail)
		{
			OutputDebugString(_T("cannot allocate interface detail data\n"));
			break;
		}

		dev_interface_detail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
		length = requiredLength;
		dev_info_data.cbSize = sizeof(SP_DEVINFO_DATA);
		if (SetupDiGetDeviceInterfaceDetail(dev_info, &dev_interface_data, dev_interface_detail, length, &requiredLength, &dev_info_data))
		{
			device = CreateFile(dev_interface_detail->DevicePath,
				GENERIC_READ | GENERIC_WRITE,
				FILE_SHARE_READ | FILE_SHARE_WRITE,
				NULL,
				OPEN_EXISTING,
				NULL,
				NULL);
		}

		LocalFree(dev_interface_detail);

		if (device != INVALID_HANDLE_VALUE)
		{
			break;
		}

		dev_index++;
	}

	return device;
}

void vhidlib_close_device(HANDLE device)
{
	if (device != INVALID_HANDLE_VALUE)
	{
		CloseHandle(device);
	}
}
#pragma endregion device_export_functions

#pragma region gamepad_export_functions
bool vhidlib_pad_down(HANDLE device, UCHAR btn)
{
	bool ret = false;
	DWORD dwRetBytes = 0;

	g_gamepad_report.report_id = 1;
	g_gamepad_report.button |= btn;
	if (DeviceIoControl(device,
		static_cast<DWORD>(IOCTL_VHIDDEV_GAMEPAD_INPUT),
		&g_gamepad_report,
		sizeof(GAMEPAD_REPORT),
		0,
		0,
		&dwRetBytes,
		NULL))
	{
		ret = true;
	}

	return ret;
}

bool vhidlib_pad_up(HANDLE device, UCHAR btn)
{
	bool ret = false;
	DWORD dwRetBytes = 0;

	g_gamepad_report.report_id = 1;
	g_gamepad_report.button &= ~btn;
	if (DeviceIoControl(device,
		static_cast<DWORD>(IOCTL_VHIDDEV_GAMEPAD_INPUT),
		&g_gamepad_report,
		sizeof(GAMEPAD_REPORT),
		0,
		0,
		&dwRetBytes,
		NULL))
	{
		ret = true;
	}

	return ret;
}

bool vhidlib_pad_click(HANDLE device, UCHAR btn)
{
	vhidlib_pad_down(device, btn);
	vhidlib_pad_up(device, btn);
	return true;
}

bool vhidlib_pad_analog(HANDLE device, CHAR x, CHAR y)
{
	bool ret = false;
	DWORD dwRetBytes = 0;

	g_gamepad_report.report_id = 1;
	g_gamepad_report.x = x;
	g_gamepad_report.y = y;
	if (DeviceIoControl(device,
		static_cast<DWORD>(IOCTL_VHIDDEV_GAMEPAD_INPUT),
		&g_gamepad_report,
		sizeof(GAMEPAD_REPORT),
		0,
		0,
		&dwRetBytes,
		NULL))
	{
		ret = true;
	}

	return ret;
}
#pragma endregion gamepad_export_functions

#pragma region keyboard_export_functions
bool vhidlib_key_down(HANDLE device, UCHAR key)
{
	bool ret = false;
	int i;
	DWORD dwRetBytes = 0;

	for (i = 0; i < MAX_KEY_INPUT; i++)
	{
		if (g_keyboard_report.keys[i] == key)
		{
			// 이미 눌려있는 키 이므로 아무 처리하지 않고 종료
			ret = true;
			break;
		}
		else if (g_keyboard_report.keys[i] == 0)
		{
			g_keyboard_report.keys[i] = key;

			if (DeviceIoControl(device,
				static_cast<DWORD>(IOCTL_VHIDDEV_KEYBOARD_INPUT),
				&g_keyboard_report,
				sizeof(KEYBOARD_REPORT),
				0,
				0,
				&dwRetBytes,
				NULL))
			{
				// 키 입력정보 전송에 성공
				ret = true;
			}

			break;
		}
	}

	return ret;
}

bool vhidlib_key_up(HANDLE device, UCHAR key)
{
	bool ret = false;
	int i, j;
	DWORD dwRetBytes = 0;

	for (i = 0; i < MAX_KEY_INPUT; i++)
	{
		if (g_keyboard_report.keys[i] == key)
		{
			for (j = i; j < MAX_KEY_INPUT - 1; j++)
			{
				g_keyboard_report.keys[j] = g_keyboard_report.keys[j + 1];
			}
			g_keyboard_report.keys[MAX_KEY_INPUT - 1] = 0;


			if (DeviceIoControl(device,
				static_cast<DWORD>(IOCTL_VHIDDEV_KEYBOARD_INPUT),
				&g_keyboard_report,
				sizeof(KEYBOARD_REPORT),
				0,
				0,
				&dwRetBytes,
				NULL))
			{
				// 키 입력정보 전송에 성공
				ret = true;
			}
			break;
		}
	}

	return ret;
}

bool vhidlib_key_click(HANDLE device, UCHAR key)
{
	vhidlib_key_down(device, key);
	vhidlib_key_up(device, key);
	return true;
}
#pragma endregion keyboard_export_functions

#pragma region mouse_export_functions
bool vhidlib_mouse_down(HANDLE device, UCHAR btn)
{
	bool ret = false;
	DWORD dwRetBytes = 0;

	g_mouse_report.report_id = 1;
	g_mouse_report.button |= btn;
	g_mouse_report.x = g_mouse_report.y = g_mouse_report.wheel = 0;
	if (DeviceIoControl(device,
		static_cast<DWORD>(IOCTL_VHIDDEV_MOUSE_INPUT),
		&g_mouse_report,
		sizeof(MOUSE_REPORT),
		0,
		0,
		&dwRetBytes,
		NULL))
	{
		ret = true;
	}

	return ret;
}

bool vhidlib_mouse_up(HANDLE device, UCHAR btn)
{
	bool ret = false;
	DWORD dwRetBytes = 0;

	g_mouse_report.report_id = 1;
	g_mouse_report.button &= ~btn;
	g_mouse_report.x = g_mouse_report.y = g_mouse_report.wheel = 0;
	if (DeviceIoControl(device,
		static_cast<DWORD>(IOCTL_VHIDDEV_MOUSE_INPUT),
		&g_mouse_report,
		sizeof(MOUSE_REPORT),
		0,
		0,
		&dwRetBytes,
		NULL))
	{
		ret = true;
	}

	return ret;
}

bool vhidlib_mouse_move(HANDLE device, CHAR x, CHAR y)
{
	bool ret = false;
	DWORD dwRetBytes = 0;

	g_mouse_report.report_id = 1;
	g_mouse_report.x = x;
	g_mouse_report.y = y;
	g_mouse_report.wheel = 0;
	if (DeviceIoControl(device,
		static_cast<DWORD>(IOCTL_VHIDDEV_MOUSE_INPUT),
		&g_mouse_report,
		sizeof(MOUSE_REPORT),
		0,
		0,
		&dwRetBytes,
		NULL))
	{
		ret = true;
	}

	return ret;
}

bool vhidlib_mouse_wheel(HANDLE device, CHAR wheel)
{
	bool ret = false;
	DWORD dwRetBytes = 0;

	g_mouse_report.report_id = 1;
	g_mouse_report.x = g_mouse_report.y = 0;
	g_mouse_report.wheel = wheel;
	if (DeviceIoControl(device,
		static_cast<DWORD>(IOCTL_VHIDDEV_MOUSE_INPUT),
		&g_mouse_report,
		sizeof(MOUSE_REPORT),
		0,
		0,
		&dwRetBytes,
		NULL))
	{
		ret = true;
	}

	return ret;
}
#pragma endregion mouse_export_functions
