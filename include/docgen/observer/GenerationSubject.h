#pragma once

#include <string>
#include <vector>
#include "docgen/observer/IGenerationObserver.h"

namespace docgen {

class GenerationSubject {
public:
    void addObserver(IGenerationObserver* observer);
    void notify(const std::string& message) const;

private:
    std::vector<IGenerationObserver*> observers_;
};

} // namespace docgen
