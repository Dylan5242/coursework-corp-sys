#include "docgen/utils/StringUtils.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <regex>
#include <sstream>
#include <unordered_set>

namespace docgen::utils {

std::string trim(const std::string& value) {
    auto begin = std::find_if_not(value.begin(), value.end(), [](unsigned char ch) {
        return std::isspace(ch);
    });
    auto end = std::find_if_not(value.rbegin(), value.rend(), [](unsigned char ch) {
        return std::isspace(ch);
    }).base();

    if (begin >= end) {
        return "";
    }
    return std::string(begin, end);
}

std::string htmlEscape(const std::string& value) {
    std::string result;
    result.reserve(value.size());

    for (char ch : value) {
        switch (ch) {
            case '&': result += "&amp;"; break;
            case '<': result += "&lt;"; break;
            case '>': result += "&gt;"; break;
            case '"': result += "&quot;"; break;
            case '\'': result += "&#39;"; break;
            default: result += ch; break;
        }
    }

    return result;
}

std::string getFileExtension(const std::string& path) {
    return std::filesystem::path(path).extension().string();
}

std::string removeCommentMarkers(const std::string& comment) {
    std::istringstream input(comment);
    std::ostringstream output;
    std::string line;
    bool firstLine = true;

    while (std::getline(input, line)) {
        std::string cleaned = trim(line);

        if (cleaned.rfind("///", 0) == 0) {
            cleaned = trim(cleaned.substr(3));
        } else if (cleaned.rfind("/**", 0) == 0) {
            cleaned = trim(cleaned.substr(3));
        } else if (cleaned.rfind("/*", 0) == 0) {
            cleaned = trim(cleaned.substr(2));
        }

        if (cleaned.size() >= 2 && cleaned.substr(cleaned.size() - 2) == "*/") {
            cleaned = trim(cleaned.substr(0, cleaned.size() - 2));
        }

        if (cleaned.rfind("*", 0) == 0) {
            cleaned = trim(cleaned.substr(1));
        }

        if (!cleaned.empty()) {
            if (!firstLine) {
                output << '\n';
            }
            output << cleaned;
            firstLine = false;
        }
    }

    return output.str();
}

std::string extractFunctionName(const std::string& signature) {
    static const std::regex functionRegex(R"(([~A-Za-z_][A-Za-z0-9_:~]*)\s*\([^;{}]*\))");
    std::smatch match;

    if (std::regex_search(signature, match, functionRegex)) {
        std::string candidate = match[1].str();
        const auto pos = candidate.rfind("::");
        if (pos != std::string::npos) {
            candidate = candidate.substr(pos + 2);
        }
        return candidate;
    }

    return "unknown_function";
}

std::string extractClassName(const std::string& signature) {
    static const std::regex classRegex(R"(\b(class|struct)\s+([A-Za-z_][A-Za-z0-9_]*)\b)");
    std::smatch match;

    if (std::regex_search(signature, match, classRegex)) {
        return match[2].str();
    }

    return "unknown_class";
}

bool isSupportedExtension(const std::string& extension) {
    static const std::unordered_set<std::string> supported = {
        ".cpp", ".h", ".hpp", ".cc", ".cxx"
    };
    return supported.count(extension) > 0;
}

} // namespace docgen::utils
