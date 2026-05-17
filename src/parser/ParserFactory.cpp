#include "docgen/parser/ParserFactory.h"
#include "docgen/parser/CppParser.h"
#include "docgen/parser/HeaderParser.h"

namespace docgen {

std::unique_ptr<Parser> ParserFactory::createParser(const std::string& extension) {
    if (extension == ".cpp" || extension == ".cc" || extension == ".cxx") {
        return std::make_unique<CppParser>();
    }

    if (extension == ".h" || extension == ".hpp") {
        return std::make_unique<HeaderParser>();
    }

    return nullptr;
}

} // namespace docgen
