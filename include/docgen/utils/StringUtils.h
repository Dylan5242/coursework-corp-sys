#pragma once

#include <string>

namespace docgen::utils {

std::string trim(const std::string& value);
std::string htmlEscape(const std::string& value);
std::string getFileExtension(const std::string& path);
std::string removeCommentMarkers(const std::string& comment);
std::string extractFunctionName(const std::string& signature);
std::string extractClassName(const std::string& signature);
bool isSupportedExtension(const std::string& extension);

} // namespace docgen::utils
