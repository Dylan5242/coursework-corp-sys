#include "docgen/parser/CppParser.h"
#include "docgen/utils/StringUtils.h"

#include <fstream>
#include <regex>
#include <sstream>

namespace docgen {

namespace {

bool looksLikeClass(const std::string& line) {
    static const std::regex classRegex(R"(^\s*(class|struct)\s+[A-Za-z_][A-Za-z0-9_]*\b)");
    return std::regex_search(line, classRegex);
}

bool looksLikeFunction(const std::string& line) {
    const std::string trimmed = utils::trim(line);

    if (trimmed.empty()) {
        return false;
    }

    if (trimmed.rfind("if", 0) == 0 || trimmed.rfind("for", 0) == 0 ||
        trimmed.rfind("while", 0) == 0 || trimmed.rfind("switch", 0) == 0 ||
        trimmed.rfind("catch", 0) == 0) {
        return false;
    }

    static const std::regex functionRegex(R"([A-Za-z_][A-Za-z0-9_:<>~*&\s]+\s+[~A-Za-z_][A-Za-z0-9_:~]*\s*\([^;{}]*\)\s*(const)?\s*(;|\{|$))");
    return std::regex_search(trimmed, functionRegex);
}

std::string collectSignature(const std::vector<std::string>& lines, std::size_t startIndex) {
    std::ostringstream signature;

    for (std::size_t i = startIndex; i < lines.size(); ++i) {
        const std::string line = utils::trim(lines[i]);
        if (line.empty()) {
            continue;
        }

        if (signature.tellp() > 0) {
            signature << ' ';
        }
        signature << line;

        if (line.find(';') != std::string::npos || line.find('{') != std::string::npos) {
            break;
        }
    }

    return signature.str();
}

} // namespace

FileDocumentation CppParser::parse(const std::filesystem::path& filePath) const {
    FileDocumentation documentation;
    documentation.filePath = filePath.string();
    documentation.fileName = filePath.filename().string();

    std::ifstream input(filePath);
    if (!input.is_open()) {
        return documentation;
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(input, line)) {
        lines.push_back(line);
    }

    for (std::size_t i = 0; i < lines.size(); ++i) {
        std::string current = utils::trim(lines[i]);
        std::string rawComment;

        if (current.rfind("///", 0) == 0) {
            while (i < lines.size() && utils::trim(lines[i]).rfind("///", 0) == 0) {
                rawComment += lines[i] + "\n";
                ++i;
            }
        } else if (current.rfind("/**", 0) == 0) {
            while (i < lines.size()) {
                rawComment += lines[i] + "\n";
                if (lines[i].find("*/") != std::string::npos) {
                    ++i;
                    break;
                }
                ++i;
            }
        } else {
            continue;
        }

        const std::string comment = utils::removeCommentMarkers(rawComment);

        while (i < lines.size() && utils::trim(lines[i]).empty()) {
            ++i;
        }

        if (i >= lines.size()) {
            break;
        }

        const std::string signature = collectSignature(lines, i);

        if (looksLikeClass(signature)) {
            ClassInfo info;
            info.name = utils::extractClassName(signature);
            info.signature = signature;
            info.comment = comment;
            documentation.classes.push_back(info);
        } else if (looksLikeFunction(signature)) {
            FunctionInfo info;
            info.name = utils::extractFunctionName(signature);
            info.signature = signature;
            info.comment = comment;
            documentation.functions.push_back(info);
        }
    }

    return documentation;
}

} // namespace docgen
