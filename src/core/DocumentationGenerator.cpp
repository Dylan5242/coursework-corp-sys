#include "docgen/core/DocumentationGenerator.h"
#include "docgen/filesystem/FileIterator.h"
#include "docgen/filesystem/FileScanner.h"
#include "docgen/generator/GenerationStrategyFactory.h"
#include "docgen/model/DocumentationModel.h"
#include "docgen/parser/ParserFactory.h"

#include <stdexcept>

namespace docgen {

void DocumentationGenerator::addObserver(IGenerationObserver* observer) {
    subject_.addObserver(observer);
}

void DocumentationGenerator::generate(
    const std::filesystem::path& inputPath,
    const std::filesystem::path& outputPath,
    const std::string& fileType,
    const std::string& generationType
) {
    subject_.notify("Scanning files...");

    FileScanner scanner;
    FileIterator iterator(scanner.scan(inputPath, fileType));

    if (iterator.empty()) {
        throw std::runtime_error("No supported C++ files were found.");
    }

    DocumentationModel model;

    for (const auto& file : iterator) {
        subject_.notify("Parsing: " + file.string());
        auto parser = ParserFactory::createParser(file.extension().string());
        if (parser) {
            model.files.push_back(parser->parse(file));
        }
    }

    subject_.notify("Generating HTML...");
    auto strategy = GenerationStrategyFactory::create(generationType);
    strategy->generate(model, outputPath);

    subject_.notify("Done. Documentation saved to: " + outputPath.string());
}

} // namespace docgen
