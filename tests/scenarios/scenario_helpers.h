#pragma once

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class ScenarioTempDirectory {
public:
    explicit ScenarioTempDirectory(const std::string& prefix) {
        const auto stamp = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        path_ = std::filesystem::temp_directory_path() / (prefix + "_" + std::to_string(stamp));
        std::filesystem::create_directories(path_);
    }

    ~ScenarioTempDirectory() {
        std::error_code error;
        std::filesystem::remove_all(path_, error);
    }

    const std::filesystem::path& path() const {
        return path_;
    }

    std::filesystem::path writeFile(const std::string& relativePath, const std::string& content) const {
        const auto filePath = path_ / relativePath;
        std::filesystem::create_directories(filePath.parent_path());
        std::ofstream output(filePath);
        output << content;
        return filePath;
    }

private:
    std::filesystem::path path_;
};

inline std::string scenarioReadFile(const std::filesystem::path& path) {
    std::ifstream input(path);
    std::ostringstream output;
    output << input.rdbuf();
    return output.str();
}

inline int scenarioFail(const std::string& message) {
    std::cerr << "Scenario failed: " << message << '\n';
    return 1;
}
