#pragma once

#include <string>

namespace docgen {

struct FunctionInfo {
    std::string name;
    std::string signature;
    std::string comment;
};

} // namespace docgen
