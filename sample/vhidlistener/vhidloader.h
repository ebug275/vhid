#pragma once

class VHIDLoader
{
public:
    VHIDLoader();
    ~VHIDLoader();

    int Load();
    int Unload();

private:
    HSWDEVICE m_hSwDevice;
};
