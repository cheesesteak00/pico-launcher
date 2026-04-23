#pragma once
#include "IAppSettingsService.h"
#include "JsonAppSettingsSerializer.h"

class JsonAppSettingsService : public IAppSettingsService
{
    JsonAppSettingsSerializer _serializer;
    AppSettings _appSettings;
    const char* _filePath;
    static constexpr const char* FAVORITES_FILE_PATH = "/_pico/favorites.txt";
    static constexpr u32 FAVORITES_FILE_BUFFER_SIZE = 256 * 100;

public:
    explicit JsonAppSettingsService(const char* filePath);

    AppSettings& GetAppSettings() override { return _appSettings; }
    const AppSettings& GetAppSettings() const override { return _appSettings; }

    void Save() const override;
    void SaveFavorites() const override;

private:
    void LoadFavorites();
};