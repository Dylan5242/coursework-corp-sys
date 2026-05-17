#pragma once

#include <memory>
#include <string>
#include "docgen/parser/Parser.h"

namespace docgen {

class ParserFactory {
public:
    static std::unique_ptr<Parser> createParser(const std::string& extension);
};

} // namespace docgen
