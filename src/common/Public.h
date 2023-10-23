#pragma once

//
// Define an Interface Guid so that apps can find the device and talk to it.
//

// {A8956E4C-1D6C-416F-A550-8509C1179E82}
DEFINE_GUID(GUID_DEVINTERFACE_VHIDDEV,
    0xa8956e4c, 0x1d6c, 0x416f, 0xa5, 0x50, 0x85, 0x9, 0xc1, 0x17, 0x9e, 0x82);

// {BD03E308-3174-44E8-B5A7-B7A4DBC2F691}
DEFINE_GUID(GUID_DEVCLASS_VHIDDEV,
    0xbd03e308, 0x3174, 0x44e8, 0xb5, 0xa7, 0xb7, 0xa4, 0xdb, 0xc2, 0xf6, 0x91);

// {6F7B398B-A02B-4D78-9445-895A986186D3}
DEFINE_GUID(GUID_VHIDDEV_INTERFACE_GAMEPAD,
    0x6f7b398b, 0xa02b, 0x4d78, 0x94, 0x45, 0x89, 0x5a, 0x98, 0x61, 0x86, 0xd3);

// {A2045A59-1D92-4893-A353-071248CF9B5D}
DEFINE_GUID(GUID_VHIDDEV_INTERFACE_KEYBOARD,
    0xa2045a59, 0x1d92, 0x4893, 0xa3, 0x53, 0x7, 0x12, 0x48, 0xcf, 0x9b, 0x5d);

// {1174C002-5DF2-498F-8E4E-405E5B7EB563}
DEFINE_GUID(GUID_VHIDDEV_INTERFACE_MOUSE,
    0x1174c002, 0x5df2, 0x498f, 0x8e, 0x4e, 0x40, 0x5e, 0x5b, 0x7e, 0xb5, 0x63);

//
// GUID definition are required to be outside of header inclusion pragma to avoid
// error during precompiled headers.
//

#ifndef __PUBLIC_H
#define __PUBLIC_H

#define FILE_DEVICE_VHIDDEV           0xC800      // vendor specific device type (0x8000 ~ 0xFFFF)

#define VHIDDEV_IOCTL(_index_) \
    CTL_CODE (FILE_DEVICE_VHIDDEV, _index_, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_VHIDDEV_GAMEPAD_INPUT       VHIDDEV_IOCTL (0x10)
#define IOCTL_VHIDDEV_KEYBOARD_INPUT      VHIDDEV_IOCTL (0x11)
#define IOCTL_VHIDDEV_MOUSE_INPUT         VHIDDEV_IOCTL (0x12)

#define MAX_KEY_INPUT 0x6

typedef enum
{
    HARDWARE_TYPE_GAMEPAD = 0,
    HARDWARE_TYPE_KEYBOARD,
    HARDWARE_TYPE_MOUSE,
    HARDWARE_TYPE_MAX
} HARDWARE_TYPE;

#pragma pack(push, 1)

typedef struct _GAMEPAD_REPORT
{
    UCHAR   report_id;
    CHAR    x;
    CHAR    y;
    UCHAR   button;
} GAMEPAD_REPORT, * PGAMEPAD_REPORT;

typedef struct _KEYBOARD_REPORT
{
    UCHAR   modifier;
    UCHAR   reserved;
    UCHAR   keys[MAX_KEY_INPUT];
} KEYBOARD_REPORT, * PKEYBOARD_REPORT;

typedef struct _MOUSE_REPORT
{
    UCHAR   report_id;
    UCHAR   button;
    CHAR    x;
    CHAR    y;
    CHAR    wheel;
} MOUSE_REPORT, * PMOUSE_REPORT;

#pragma pack(pop)

#endif