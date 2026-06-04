#pragma once
#include <memory>
#include "core/String.h"
#include "RomBrowserDisplaySettings.h"
#include "FileAssociation.h"

class AppSettings
{
public:
    String<char, 16> language = "english";
    String<char, 64> theme = "material";
    String<char, 256> lastUsedFilePath = "";
    RomBrowserDisplaySettings romBrowserDisplaySettings;
    u8 brightnessLevel = 3;

    std::unique_ptr<FileAssociation[]> fileAssociations;
    u32 numberOfFileAssociations = 0;

    static constexpr u32 MAX_FAVORITES = 100;
    std::unique_ptr<String<char, 256>[]> favoritePaths;
    u32 numberOfFavorites = 0;
};