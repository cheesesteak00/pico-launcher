#include "common.h"
#include <memory>
#include "fat/File.h"
#include "JsonAppSettingsService.h"

JsonAppSettingsService::JsonAppSettingsService(const char* filePath)
    : _filePath(filePath)
{
    if (!_serializer.Deserialize(&_appSettings, _filePath))
        Save();
    LoadFavorites();
}

void JsonAppSettingsService::Save() const
{
    _serializer.Serialize(&_appSettings, _filePath);
}

void JsonAppSettingsService::SaveFavorites() const
{
    u32 totalSize = 0;
    for (u32 i = 0; i < _appSettings.numberOfFavorites; i++)
        totalSize += strlen(_appSettings.favoritePaths[i].GetString()) + 1;

    if (totalSize == 0)
    {
        const auto file = std::make_unique<File>();
        file->Open(FAVORITES_FILE_PATH, FA_WRITE | FA_CREATE_ALWAYS);
        return;
    }

    std::unique_ptr<u8[]> fileData(new u8[totalSize]);
    u32 offset = 0;
    for (u32 i = 0; i < _appSettings.numberOfFavorites; i++)
    {
        const char* path = _appSettings.favoritePaths[i].GetString();
        u32 len = strlen(path);
        memcpy(fileData.get() + offset, path, len);
        offset += len;
        fileData[offset++] = '\n';
    }

    const auto file = std::make_unique<File>();
    if (file->Open(FAVORITES_FILE_PATH, FA_WRITE | FA_CREATE_ALWAYS) != FR_OK)
    {
        LOG_ERROR("Couldn't open favorites file for writing\n");
        return;
    }

    u32 bytesWritten;
    if (file->Write(fileData.get(), totalSize, bytesWritten) != FR_OK || bytesWritten != totalSize)
        LOG_ERROR("Error while writing favorites file\n");
}

void JsonAppSettingsService::LoadFavorites()
{
    const auto file = std::make_unique<File>();
    if (file->Open(FAVORITES_FILE_PATH, FA_READ | FA_OPEN_EXISTING) != FR_OK)
        return;

    u32 fileSize = file->GetSize();
    if (fileSize == 0 || fileSize > FAVORITES_FILE_BUFFER_SIZE)
        return;

    std::unique_ptr<u8[]> fileData(new u8[fileSize + 1]);
    u32 bytesRead = 0;
    if (file->Read(fileData.get(), fileSize, bytesRead) != FR_OK)
        return;

    fileData[fileSize] = 0;

    char* lineStart = (char*)fileData.get();
    u32 count = 0;
    for (u32 i = 0; i < fileSize && count < AppSettings::MAX_FAVORITES; i++)
    {
        if (fileData[i] == '\n' || fileData[i] == '\r' || fileData[i] == 0)
        {
            fileData[i] = 0;
            if (lineStart[0] != 0)
                count++;
            lineStart = (char*)fileData.get() + i + 1;
        }
    }
    if (lineStart < (char*)fileData.get() + fileSize && count < AppSettings::MAX_FAVORITES && lineStart[0] != 0)
        count++;

    if (count == 0)
        return;

    _appSettings.favoritePaths = std::make_unique<String<char, 256>[]>(AppSettings::MAX_FAVORITES);

    lineStart = (char*)fileData.get();
    u32 idx = 0;
    for (u32 i = 0; i < fileSize && idx < count; i++)
    {
        if (fileData[i] == 0)
        {
            if (lineStart[0] != 0)
                _appSettings.favoritePaths[idx++] = lineStart;
            lineStart = (char*)fileData.get() + i + 1;
        }
    }
    if (idx < count && lineStart < (char*)fileData.get() + fileSize && lineStart[0] != 0)
        _appSettings.favoritePaths[idx++] = lineStart;

    _appSettings.numberOfFavorites = idx;
}
