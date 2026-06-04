#include "common.h"
#include <nds/system.h>
#include <libtwl/spi/spiPmic.h>
#include <libtwl/i2c/i2cMcu.h>
#include "BrightnessIpcService.h"

void BrightnessIpcService::HandleMessage(u32 data)
{
    u8 level = (u8)data;
    if (isDSiMode())
        mcu_writeReg(MCU_REG_BACKLIGHT, level);
    else
        pmic_setBacklightLevel(level);
}
