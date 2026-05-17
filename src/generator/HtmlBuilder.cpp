#include "docgen/generator/HtmlBuilder.h"
#include "docgen/utils/StringUtils.h"

namespace docgen {

HtmlBuilder& HtmlBuilder::beginDocument(const std::string& title) {
    html_ << "<!DOCTYPE html>\n<html lang=\"ru\">\n<head>\n"
          << "<meta charset=\"UTF-8\">\n"
          << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
          << "<title>" << utils::htmlEscape(title) << "</title>\n"
          << "<style>"
          << "body{font-family:Arial,sans-serif;max-width:1000px;margin:40px auto;line-height:1.6;}"
          << "h1,h2,h3{color:#222;} code,pre{background:#f4f4f4;padding:3px 6px;border-radius:4px;}"
          << ".file{border:1px solid #ddd;border-radius:8px;padding:16px;margin:20px 0;}"
          << ".entity{margin-left:16px;padding:10px;border-left:4px solid #ddd;}"
          << "nav{background:#f7f7f7;padding:12px;border-radius:8px;}"
          << "</style>\n"
          << "</head>\n<body>\n";
    return *this;
}

HtmlBuilder& HtmlBuilder::addHeading(const std::string& text, int level) {
    if (level < 1 || level > 6) {
        level = 1;
    }
    html_ << "<h" << level << ">" << utils::htmlEscape(text) << "</h" << level << ">\n";
    return *this;
}

HtmlBuilder& HtmlBuilder::addParagraph(const std::string& text) {
    html_ << "<p>" << utils::htmlEscape(text) << "</p>\n";
    return *this;
}

HtmlBuilder& HtmlBuilder::addNavigation(const std::vector<FileDocumentation>& files) {
    html_ << "<nav><strong>Файлы:</strong><ul>\n";
    for (const auto& file : files) {
        html_ << "<li><a href=\"#" << utils::htmlEscape(file.fileName) << "\">"
              << utils::htmlEscape(file.fileName) << "</a></li>\n";
    }
    html_ << "</ul></nav>\n";
    return *this;
}

HtmlBuilder& HtmlBuilder::addFileDocumentation(const FileDocumentation& file) {
    html_ << "<section class=\"file\" id=\"" << utils::htmlEscape(file.fileName) << "\">\n";
    addHeading("Файл: " + file.fileName, 2);

    if (file.classes.empty() && file.functions.empty()) {
        addParagraph("Документированные классы и функции не найдены.");
    }

    if (!file.classes.empty()) {
        addHeading("Классы", 3);
        for (const auto& cls : file.classes) {
            html_ << "<div class=\"entity\">\n"
                  << "<h4>" << utils::htmlEscape(cls.name) << "</h4>\n"
                  << "<pre>" << utils::htmlEscape(cls.signature) << "</pre>\n"
                  << "<p>" << utils::htmlEscape(cls.comment) << "</p>\n"
                  << "</div>\n";
        }
    }

    if (!file.functions.empty()) {
        addHeading("Функции", 3);
        for (const auto& fn : file.functions) {
            html_ << "<div class=\"entity\">\n"
                  << "<h4>" << utils::htmlEscape(fn.name) << "</h4>\n"
                  << "<pre>" << utils::htmlEscape(fn.signature) << "</pre>\n"
                  << "<p>" << utils::htmlEscape(fn.comment) << "</p>\n"
                  << "</div>\n";
        }
    }

    html_ << "</section>\n";
    return *this;
}

HtmlBuilder& HtmlBuilder::endDocument() {
    html_ << "</body>\n</html>\n";
    return *this;
}

std::string HtmlBuilder::build() const {
    return html_.str();
}

} // namespace docgen
