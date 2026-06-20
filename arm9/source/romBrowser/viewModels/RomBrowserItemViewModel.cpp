#include "common.h"
#include "romBrowser/IRomBrowserController.h"
#include "RomBrowserViewModel.h"
#include "romBrowser/FileType/FileTypeClassification.h"
#include "RomBrowserItemViewModel.h"

void RomBrowserItemViewModel::Activate()
{
    if (_index >= 0)
    {
        const auto& item = _romBrowserController->GetRomBrowserViewModel()->GetFileInfoManager().GetItem(_index);
        if (item.GetFileType()->GetClassification() == FileTypeClassification::Folder)
        {
            _romBrowserController->NavigateToPath(item.GetFileName());
        }
        else
        {
            _romBrowserController->LaunchFile(item);
        }
    }
}

void RomBrowserItemViewModel::ShowGameInfo()
{
    if (_index >= 0)
    {
        const auto& item = _romBrowserController->GetRomBrowserViewModel()->GetFileInfoManager().GetItem(_index);
        const auto classification = item.GetFileType()->GetClassification();
        if (classification != FileTypeClassification::Folder
            && classification != FileTypeClassification::Unknown)
        {
            _romBrowserController->ShowGameInfo(item);
        }
    }
}
