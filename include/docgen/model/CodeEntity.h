#pragma once

#include <string>

namespace docgen {

enum class EntityType {
    Function,
    Class,
    Unknown
};

struct CodeEntity {
    EntityType type = EntityType::Unknown;
    std::string name;
    std::string signature;
    std::string comment;
};

} // namespace docgen
