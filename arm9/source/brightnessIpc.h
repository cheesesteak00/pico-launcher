#pragma once
#include <libtwl/ipc/ipcFifoSystem.h>
#include "ipcChannels.h"

inline void brightness_setLevel(u8 level)
{
    ipc_sendFifoMessage(IPC_CHANNEL_BRIGHTNESS, level);
}
