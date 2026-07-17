#include "common.h"
#include <string.h>
#include <algorithm>
#include "core/PathUtil.h"
#include "SdFolder.h"

SdFolder::SdFolder(FileInfo** files, int fileCount)
    : _files(files), _fileCount(fileCount) { }

SdFolder::~SdFolder()
{
    for (int i = 0; i < _fileCount; i++)
        delete _files[i];
    free(_files);
}

static bool IsFileInFavorites(const FileInfo* file, const SdFolderFilterSortParams& params)
{
    if (!params.showFavoritesOnly || params.favoritePaths == nullptr || params.currentPath == nullptr)
        return true;

    if (file->GetFileType()->GetClassification() == FileTypeClassification::Folder)
        return true;

    char fullPath[256];
    PathUtil::JoinPath(params.currentPath, file->GetFileName(), fullPath, sizeof(fullPath));

    for (u32 i = 0; i < params.numberOfFavorites; i++)
    {
        if (strcmp(fullPath, params.favoritePaths[i].GetString()) == 0)
            return true;
    }
    return false;
}

std::unique_ptr<const FileInfo*[]> SdFolder::FilterAndSort(
    const SdFolderFilterSortParams& filterSortParams, u32& resultCount) const
{
    auto sortedFilteredFiles = std::make_unique<const FileInfo*[]>(_fileCount);
    u32 filteredCount = 0;
    for (int i = 0; i < _fileCount; i++)
    {
        const FileInfo* file = _files[i];
        bool isHidden = file->GetFileName()[0] == '.' || file->IsHidden();
        auto classification = file->GetFileType()->GetClassification();
        if (classification != FileTypeClassification::Unknown &&
            (!isHidden || filterSortParams.includeHiddenFiles) &&
            IsFileInFavorites(file, filterSortParams))
        {
            sortedFilteredFiles[filteredCount++] = file;
        }
    }
    std::sort(sortedFilteredFiles.get(), sortedFilteredFiles.get() + filteredCount,
        [filterSortParams] (const FileInfo*& a, const FileInfo*& b)
        {
            bool result = true;
            if (CompareClassification(a, b, result))
                return result;

            auto sortType = filterSortParams.sortType;
            auto sortDirection = filterSortParams.sortDirection;
            if (a->GetFileType()->GetClassification() == FileTypeClassification::Folder &&
                b->GetFileType()->GetClassification() == FileTypeClassification::Folder)
            {
                if (sortType != SdFolderSortType::Name)
                {
                    sortType = SdFolderSortType::Name;
                    sortDirection = SdFolderSortDirection::Ascending;
                }
            }
            switch (sortType)
            {
                case SdFolderSortType::Name:
                default:
                {
                    result = CompareName(a, b);
                    break;
                }
            }
            return sortDirection == SdFolderSortDirection::Ascending ? result : !result;
        });
    resultCount = filteredCount;
    return sortedFilteredFiles;
}

bool SdFolder::CompareClassification(const FileInfo* a, const FileInfo* b, bool& result)
{
    auto aClassification = a->GetFileType()->GetClassification();
    auto bClassification = b->GetFileType()->GetClassification();
    if (aClassification == bClassification)
        return false;

    if (aClassification == FileTypeClassification::Folder)
    {
        result = true;
        return true;
    }
    else if (bClassification == FileTypeClassification::Folder)
    {
        result = false;
        return true;
    }

    return false;
}

bool SdFolder::CompareName(const FileInfo* a, const FileInfo* b)
{
    return strcasecmp(a->GetFileName(), b->GetFileName()) < 0;
}

void SdFolder::SortByNameInPlace()
{
    std::sort(_files, _files + _fileCount, CompareName);
}

const FileInfo* SdFolder::BinarySearch(const char* fileName) const
{
    if (_fileCount != 0)
    {
        const auto file = std::lower_bound(_files, _files + _fileCount, fileName,
            [] (const FileInfo* entry, const char* value)
            {
                return strcasecmp(entry->GetFileName(), value) < 0;
            });

        if (file != _files + _fileCount && !strcasecmp((*file)->GetFileName(), fileName))
        {
            return *file;
        }
    }

    return nullptr;
}
