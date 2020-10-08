#include "FileDialog.h"
#include "tinyfiledialogs.h"

namespace CAMageddon
{
	const char* FileDialog::SaveFile(
        const char* title,
        const char* defaultName,
        int numberOfFilters,
        const char* const* filterPatterns,
        const char* singleFilterDescription)
	{
        return tinyfd_saveFileDialog(
            title,
            defaultName,
            numberOfFilters,
            filterPatterns,
            singleFilterDescription);
	}
    const char* FileDialog::OpenFile(
        const char* title,
        const char* defaultPath,
        int numberOfFilters,
        const char* const* filterPatterns,
        const char* singleFilterDescription,
        bool allowMultipleSelect)
    {
        return tinyfd_openFileDialog(
            title,
            defaultPath,
            numberOfFilters,
            filterPatterns,
            singleFilterDescription,
            allowMultipleSelect);
    }
}
