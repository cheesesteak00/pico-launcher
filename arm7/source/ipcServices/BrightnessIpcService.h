#pragma once
#include "ipc/ThreadIpcService.h"
#include "ipcChannels.h"

class BrightnessIpcService : public ThreadIpcService
{
    u32 _threadStack[64];

public:
    BrightnessIpcService()
        : ThreadIpcService(IPC_CHANNEL_BRIGHTNESS, 10, _threadStack, sizeof(_threadStack)) { }

    void HandleMessage(u32 data) override;
};
