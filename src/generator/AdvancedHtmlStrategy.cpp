#include "docgen/generator/AdvancedHtmlStrategy.h"
#include "docgen/generator/HtmlBuilder.h"

#include <filesystem>
#include <fstream>
#include <stdexcept>

namespace docgen {

void AdvancedHtmlStrategy::generate(const DocumentationModel& model, const std::filesystem::path& outputPath) const {
    std::filesystem::create_directories(outputPath);

    HtmlBuilder builder;
    builder.beginDocument("DocGen Lite Advanced Documentation")
           .addHeading("Расширенная документация проекта", 1)
           .addNavigation(model.files);

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
