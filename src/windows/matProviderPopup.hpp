//
// Created by rdkgs on 2/20/2025.
//

#ifndef MATPROVIDERWINDOW_HPP
#define MATPROVIDERWINDOW_HPP

#include <string>
#include <imGui/imgui.h>
#include "mat.hpp"
#include "popupBase.hpp"

class matProviderPopup : popupBase<matProviderPopup> {
public:
    matProviderPopup();
    void schedule() { show_ = true; }
    void show(mat& data);
    void setTemp(mat& data);

private:
    bool show_ = false;
    mat temp_;
};

//////////////////////////////////////////////////////////////////
/// implementation
//////////////////////////////////////////////////////////////////

inline matProviderPopup::matProviderPopup()
{
    temp_.resize(1, 1);
}

inline void matProviderPopup::show(mat& data)
{
    if (show_) {
        show_ = false;
        ImGui::OpenPopup("Provide the matrix");
    }

    if (ImGui::BeginPopupModal("Provide the matrix", nullptr, 0)) {
        ImGui::Columns(4);
        if (ImGui::Button("Add row")) {
            temp_.addEmptyRow();
        }
        ImGui::NextColumn();
        if (ImGui::Button("Remove row")) {
            temp_.removeLastRow();
        }
        ImGui::NextColumn();
        if (ImGui::Button("Add column")) {
            temp_.addEmptyColumn();
        }
        ImGui::NextColumn();
        if (ImGui::Button("Remove column")) {
            temp_.removeLastColumn();
        }

        ImGui::Columns(1);
        ImGui::Separator();

        /* display by columns */
        ImGui::Columns(temp_.getM());
        for (int j = 0; j < temp_.getM(); ++j) {
            for (int i = 0; i < temp_.getN(); ++i) {
                ImGui::InputFloat(
                ("##" + std::to_string(i * temp_.getM() + j)).c_str(),
                temp_.getData() + i * temp_.getM() + j,
                    0.0f, 0.0f, "%.3f");
            }
            ImGui::NextColumn();
        }


        ImGui::Columns(1);
        if (ImGui::Button("OK")) {
            data = temp_;
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

inline void matProviderPopup::setTemp(mat &data)
{
    temp_ = data;
}

#endif //MATPROVIDERWINDOW_HPP
