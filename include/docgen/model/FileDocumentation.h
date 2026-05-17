#pragma once

#include <string>
#include <vector>
#include "docgen/model/FunctionInfo.h"
#include "docgen/model/ClassInfo.h"

namespace docgen {

struct FileDocumentation {
    std::string filePath;
    std::string fileName;
    std::vector<FunctionInfo> functions;
    std::vector<ClassInfo> classes;
};

} // namespace docgen
