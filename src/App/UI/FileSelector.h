#pragma once

#include <filesystem>
#include <vector>
#include <string>

namespace fs = std::filesystem;

class FileSelector {
public:
    explicit FileSelector(std::string  path);

    explicit FileSelector(const std::filesystem::path& path,
                      std::string title = "Select File")
    : FileSelector(path.string(), std::move(title)) {}

    void RefreshFiles();

    void Draw();

    std::string GetSelectedFile() const;

private:
    std::string directory_;
    std::vector<std::string> files_;
    int selectedIndex_;
};