#pragma once

#ifdef VHIDLIB_EXPORTS
#define VHIDLIB_API __declspec(dllexport)
#else
#define VHIDLIB_API __declspec(dllimport)
#endif

extern "C" VHIDLIB_API HANDLE vhidlib_open_device();
extern "C" VHIDLIB_API void vhidlib_close_device(HANDLE device);

extern "C" VHIDLIB_API bool vhidlib_pad_down(HANDLE device, UCHAR btn);
extern "C" VHIDLIB_API bool vhidlib_pad_up(HANDLE device, UCHAR btn);
extern "C" VHIDLIB_API bool vhidlib_pad_click(HANDLE device, UCHAR btn);
extern "C" VHIDLIB_API bool vhidlib_pad_analog(HANDLE device, CHAR x, CHAR y);

extern "C" VHIDLIB_API bool vhidlib_key_down(HANDLE device, UCHAR key);
extern "C" VHIDLIB_API bool vhidlib_key_up(HANDLE device, UCHAR key);
extern "C" VHIDLIB_API bool vhidlib_key_click(HANDLE device, UCHAR key);

extern "C" VHIDLIB_API bool vhidlib_mouse_down(HANDLE device, UCHAR btn);
extern "C" VHIDLIB_API bool vhidlib_mouse_up(HANDLE device, UCHAR btn);
extern "C" VHIDLIB_API bool vhidlib_mouse_move(HANDLE device, CHAR x, CHAR y);
extern "C" VHIDLIB_API bool vhidlib_mouse_wheel(HANDLE device, CHAR wheel);
