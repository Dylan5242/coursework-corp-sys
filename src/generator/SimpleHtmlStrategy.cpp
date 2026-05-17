#include "docgen/generator/SimpleHtmlStrategy.h"
#include "docgen/generator/HtmlBuilder.h"

#include <filesystem>
#include <fstream>
#include <stdexcept>

namespace docgen {

void SimpleHtmlStrategy::generate(const DocumentationModel& model, const std::filesystem::path& outputPath) const {
    std::filesystem::create_directories(outputPath);

    HtmlBuilder builder;
    builder.beginDocument("DocGen Lite Documentation")
           .addHeading("Документация проекта", 1);

    for (const auto& file : model.files) {
        builder.addFileDocumentation(file);
    }

    builder.endDocument();

    std::ofstream output(outputPath / "index.html");
    if (!output.is_open()) {
        throw std::runtime_error("Cannot create output file: " + (outputPath / "index.html").string());
    }

    output << builder.build();
}

} // namespace docgen
