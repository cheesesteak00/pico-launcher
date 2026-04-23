#pragma once
#include "SdFolderSortType.h"
#include "SdFolderSortDirection.h"
#include "core/String.h"

class SdFolderFilterSortParams
{
public:
    SdFolderSortType sortType = SdFolderSortType::Name;
    SdFolderSortDirection sortDirection = SdFolderSortDirection::Ascending;
    bool includeHiddenFiles = false;

    bool showFavoritesOnly = false;
    const char* currentPath = nullptr;
    const String<char, 256>* favoritePaths = nullptr;
    u32 numberOfFavorites = 0;

    SdFolderFilterSortParams() { }

    SdFolderFilterSortParams(SdFolderSortType sortType, SdFolderSortDirection sortDirection, bool includeHiddenFiles)
        : sortType(sortType), sortDirection(sortDirection), includeHiddenFiles(includeHiddenFiles) { }
};
