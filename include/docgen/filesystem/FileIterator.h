#pragma once

#include <filesystem>
#include <vector>

namespace docgen {

class FileIterator {
public:
    explicit FileIterator(std::vector<std::filesystem::path> files);

    std::vector<std::filesystem::path>::const_iterator begin() const;
    std::vector<std::filesystem::path>::const_iterator end() const;
    bool empty() const;
    std::size_t size() const;

private:
    std::vector<std::filesystem::path> files_;
};

} // namespace docgen
