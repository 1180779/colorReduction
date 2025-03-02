//
// Created by rdkgs on 2/20/2025.
//

#ifndef MATPROVIDERWINDOW_HPP
#define MATPROVIDERWINDOW_HPP

#include <string>
#include <imGui/imgui.h>
#include "mat.hpp"
#include "popupBase.hpp"
#include "ui.hpp"
#include "../../deps/imGui/src/imgui_internal.h"

class matProviderPopup : popupBase<matProviderPopup> {
public:
    matProviderPopup();
    void schedule() { show_ = true; }
    void show(mat& data);
    void setTemp(mat& data);

private:
    bool show_ = false;
    mat temp_;

    static constexpr int padding = 20;
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
        if (ImGui::BeginTable("matProviderPopup-controls-1", 5)) {
            ImGui::TableNextRow();

            ImGui::TableNextColumn();
            if (ui::ButtonFill("add row")) {
                temp_.addEmptyRow();
            }

            ImGui::TableNextColumn();
            if (ui::ButtonFill("remove row")) {
                temp_.removeLastRow();
            }

            ImGui::TableNextColumn();
            if (ui::ButtonFill("add column")) {
                temp_.addEmptyColumn();
            }

            ImGui::TableNextColumn();
            if (ui::ButtonFill("remove column")) {
                temp_.removeLastColumn();
            }

            ImGui::TableNextColumn();
            ImGui::Text((std::to_string(temp_.getN()) + 'x' + std::to_string(temp_.getM())).c_str());

            ImGui::EndTable();
        }
        ImGui::Separator();

        if (ImGui::BeginTable("matProviderPopup-matrix", temp_.getM())) {
            for (int i = 0; i < temp_.getN(); ++i) {
                ImGui::TableNextRow();
                for (int j = 0; j < temp_.getM(); ++j) {
                    ImGui::TableSetColumnIndex(j);
                    ui::InputFloatFill(
                        ("##" + std::to_string(i * temp_.getM() + j)).c_str(),
                        temp_.getData() + i * temp_.getM() + j,
                        0.0f, 0.0f, "%.3f");
                }
            }
            ImGui::EndTable();
        }

        if (ImGui::BeginTable("matProviderPopup-controls-2", 4)) {\
            ImGui::TableNextRow();

            ImGui::TableNextColumn();
            if (ui::ButtonFill("ok")) {
                data = temp_;
                ImGui::CloseCurrentPopup();
            }

            ImGui::TableNextColumn();
            if (ui::ButtonFill("cancel")) {
                ImGui::CloseCurrentPopup();
            }

            ImGui::TableNextColumn();
            if (ui::ButtonFill("clear")) {
                temp_.clear();
            }

            ImGui::TableNextColumn();
            if (ui::ButtonFill("normalize")) {
                temp_.normalize();
            }

            ImGui::EndTable();
        }
        ImGui::EndPopup();
    }
}

inline void matProviderPopup::setTemp(mat &data)
{
    temp_ = data;
}

#endif //MATPROVIDERWINDOW_HPP
