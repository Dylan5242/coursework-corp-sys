#pragma once

#include <sstream>
#include <string>
#include "docgen/model/FileDocumentation.h"

namespace docgen {

class HtmlBuilder {
public:
    HtmlBuilder& beginDocument(const std::string& title);
    HtmlBuilder& addHeading(const std::string& text, int level = 1);
    HtmlBuilder& addParagraph(const std::string& text);
    HtmlBuilder& addNavigation(const std::vector<FileDocumentation>& files);
    HtmlBuilder& addFileDocumentation(const FileDocumentation& file);
    HtmlBuilder& endDocument();

    std::string build() const;

private:
    std::ostringstream html_;
};

} // namespace docgen
