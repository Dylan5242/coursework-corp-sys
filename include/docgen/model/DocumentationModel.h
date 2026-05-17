#pragma once

#include <vector>
#include "docgen/model/FileDocumentation.h"

namespace docgen {

struct DocumentationModel {
    std::vector<FileDocumentation> files;

    bool empty() const {
        return files.empty();
    }
};

} // namespace docgen
