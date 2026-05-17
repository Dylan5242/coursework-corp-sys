#pragma once

#include "docgen/parser/Parser.h"

namespace docgen {

class CppParser : public Parser {
public:
    FileDocumentation parse(const std::filesystem::path& filePath) const override;
};

} // namespace docgen
