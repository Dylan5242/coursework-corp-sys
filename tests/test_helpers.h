#pragma once

#include <chrono>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

class TempDirectory {
public:
    explicit TempDirectory(const std::string& prefix = "docgen_test") {
        const auto stamp = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        path_ = std::filesystem::temp_directory_path() / (prefix + "_" + std::to_string(stamp));
        std::filesystem::create_directories(path_);
    }

    ~TempDirectory() {
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

    std::filesystem::path makeDir(const std::string& relativePath) const {
        const auto directory = path_ / relativePath;
        std::filesystem::create_directories(directory);
        return directory;
    }

private:
    std::filesystem::path path_;
};

inline std::string readTextFile(const std::filesystem::path& path) {
    std::ifstream input(path);
    std::ostringstream output;
    output << input.rdbuf();
    return output.str();
}
