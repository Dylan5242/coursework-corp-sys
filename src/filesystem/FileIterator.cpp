#include "docgen/filesystem/FileIterator.h"

namespace docgen {

FileIterator::FileIterator(std::vector<std::filesystem::path> files)
    : files_(std::move(files)) {}

std::vector<std::filesystem::path>::const_iterator FileIterator::begin() const {
    return files_.begin();
}

std::vector<std::filesystem::path>::const_iterator FileIterator::end() const {
    return files_.end();
}

bool FileIterator::empty() const {
    return files_.empty();
}

std::size_t FileIterator::size() const {
    return files_.size();
}

} // namespace docgen
