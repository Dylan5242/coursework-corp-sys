#include "docgen/observer/GenerationSubject.h"

namespace docgen {

void GenerationSubject::addObserver(IGenerationObserver* observer) {
    if (observer != nullptr) {
        observers_.push_back(observer);
    }
}

void GenerationSubject::notify(const std::string& message) const {
    for (auto* observer : observers_) {
        if (observer != nullptr) {
            observer->onProgress(message);
        }
    }
}

} // namespace docgen
