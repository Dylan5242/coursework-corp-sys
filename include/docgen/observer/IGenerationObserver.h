#pragma once

#include <string>

namespace docgen {

class IGenerationObserver {
public:
    virtual void onProgress(const std::string& message) = 0;
    virtual ~IGenerationObserver() = default;
};

} // namespace docgen
