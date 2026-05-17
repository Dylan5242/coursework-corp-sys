#include "docgen/filesystem/FileScanner.h"
#include "docgen/utils/StringUtils.h"

#include <algorithm>
#include <stdexcept>

namespace docgen {

std::vector<std::filesystem::path> FileScanner::scan(
    const std::filesystem::path& inputPath,
    const std::string& fileType
) const {
    if (!std::filesystem::exists(inputPath)) {
        throw std::runtime_error("Input path does not exist: " + inputPath.string());
    }

    std::vector<std::filesystem::path> result;

    if (std::filesystem::is_regular_file(inputPath)) {
        if (shouldInclude(inputPath, fileType)) {
            result.push_back(inputPath);
        }
        return result;
    }

    for (const auto& entry : std::filesystem::recursive_directory_iterator(inputPath)) {
        if (entry.is_regular_file() && shouldInclude(entry.path(), fileType)) {
            result.push_back(entry.path());
        }
    }

    std::sort(result.begin(), result.end());
    return result;
}

bool FileScanner::shouldInclude(const std::filesystem::path& path, const std::string& fileType) const {
    const std::string extension = path.extension().string();

    if (fileType == "auto") {
        return utils::isSupportedExtension(extension);
    }

    return extension == fileType;
}

} // namespace docgen
