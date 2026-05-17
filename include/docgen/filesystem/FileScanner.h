#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace docgen {

class FileScanner {
public:
    std::vector<std::filesystem::path> scan(
        const std::filesystem::path& inputPath,
        const std::string& fileType
    ) const;

private:
    bool shouldInclude(const std::filesystem::path& path, const std::string& fileType) const;
};

} // namespace docgen
