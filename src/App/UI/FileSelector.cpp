//
// Created by Lorenz Saalmann on 07.12.25.
//

#include "FileSelector.h"

#include <iostream>

#include "imgui.h"

FileSelector::FileSelector(std::string path): directory_(std::move(path)), selectedIndex_(-1) {
    RefreshFiles();
}

void FileSelector::RefreshFiles() {
    files_.clear();
    for (const auto& entry : fs::directory_iterator(directory_)) {
        if (entry.is_regular_file()) {
            files_.push_back(entry.path().filename().string());
        }
    }
}

void FileSelector::Draw(const char* selectedFile) {
    RefreshFiles();

    const float itemHeight = ImGui::GetTextLineHeightWithSpacing();
    float height = files_.empty() ? itemHeight : files_.size() * itemHeight;
    height = std::min(height, 50.0f); // optional max height

    // find selected index
    if (selectedFile != "" ) {
        selectedIndex_ = -1;
        for (int i = 0; i < files_.size(); ++i) {
            if (files_[i] == selectedFile) {
                selectedIndex_ = i;
                break;
            }
        }
    }

    if (ImGui::BeginListBox("##files", ImVec2(-FLT_MIN, height * 1.1f))) {
        for (int i = 0; i < files_.size(); ++i)
        {
            bool isSelected = selectedIndex_ == i;
            if (ImGui::Selectable(files_[i].c_str(), isSelected)) {
                // user changed selection
                selectedIndex_ = i;
            }
            if (isSelected)
                ImGui::SetItemDefaultFocus(); // scroll to selected
        }
        ImGui::EndListBox();
    }
}

std::string FileSelector::GetSelectedFile() const {
    if (selectedIndex_ >= 0 && selectedIndex_ < files_.size())
        return directory_ + std::string("/") + files_[selectedIndex_];
    return "";
}


