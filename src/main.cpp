#include "docgen/core/DocumentationGenerator.h"
#include "docgen/observer/IGenerationObserver.h"

#include <exception>
#include <iostream>
#include <string>

class ConsoleObserver final : public docgen::IGenerationObserver {
public:
    void onProgress(const std::string& message) override {
        std::cout << "[DocGen] " << message << '\n';
    }
};

namespace {

void printUsage(const char* executable) {
    std::cout << "Usage:\n"
              << "  " << executable << " <input_path> <output_path> [file_type] [generation_type]\n\n"
              << "Arguments:\n"
              << "  input_path       File or directory with C++ source files\n"
              << "  output_path      Directory where index.html will be generated\n"
              << "  file_type        auto, .cpp, .h, .hpp, .cc, .cxx. Default: auto\n"
              << "  generation_type  simple or advanced. Default: simple\n\n";
}

} // namespace

int main(int argc, char* argv[]) {
    std::string inputPath;
    std::string outputPath;
    std::string fileType = "auto";
    std::string generationType = "simple";

    if (argc >= 3) {
        inputPath = argv[1];
        outputPath = argv[2];
        if (argc >= 4) {
            fileType = argv[3];
        }
        if (argc >= 5) {
            generationType = argv[4];
        }
    } else {
        printUsage(argv[0]);
        std::cout << "Interactive mode\n";
        std::cout << "Input path: ";
        std::getline(std::cin, inputPath);
        std::cout << "Output path: ";
        std::getline(std::cin, outputPath);
        std::cout << "File type [auto/.cpp/.h/.hpp] (default auto): ";
        std::string typedFileType;
        std::getline(std::cin, typedFileType);
        if (!typedFileType.empty()) {
            fileType = typedFileType;
        }
        std::cout << "Generation type [simple/advanced] (default simple): ";
        std::string typedGenerationType;
        std::getline(std::cin, typedGenerationType);
        if (!typedGenerationType.empty()) {
            generationType = typedGenerationType;
        }
    }

    try {
        ConsoleObserver observer;
        docgen::DocumentationGenerator generator;
        generator.addObserver(&observer);
        generator.generate(inputPath, outputPath, fileType, generationType);
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
}
