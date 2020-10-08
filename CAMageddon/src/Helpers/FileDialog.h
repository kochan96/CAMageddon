#pragma once

namespace CAMageddon
{
	class FileDialog
	{
	public:
		static const char* SaveFile(
			const char* title,
			const char* defaultName,
			int numberOfFilters,
			const char* const* filterPatterns,
			const char* singleFilterDescription);

		static const char* OpenFile(
			const char* title,
			const char* defaultPath,
			int numberOfFilters,
			const char* const* filterPatterns,
			const char* singleFilterDescription = nullptr,
			bool allowMultipleSelect = false);
	};
}