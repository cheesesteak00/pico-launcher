#include "common.h"
#include <string.h>
#include "StringUtil.h"
#include "PathUtil.h"

u32 PathUtil::JoinPath(const char* base, const char* name, char* out, u32 outSize)
{
    if (!out || outSize == 0)
        return 0;

    u32 written = StringUtil::Copy(out, base, outSize);
    if (written + 1 >= outSize)
        return outSize;

    if (written > 0 && out[written - 1] != '/')
    {
        out[written++] = '/';
        out[written] = 0;
        if (written + 1 >= outSize)
            return outSize;
    }

    written += StringUtil::Copy(out + written, name, outSize - written);
    return written;
}
