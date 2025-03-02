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
    matProviderPopup() = default;
    void schedule() { show_ = true; }
    void show(mat& data);
    void setTemp(mat& data);

private:
    static float tempData_s[9];

    bool show_ = false;
    mat temp_ = mat(3, 3, tempData_s);

    static constexpr int padding = 20;
};

//////////////////////////////////////////////////////////////////
/// implementation
//////////////////////////////////////////////////////////////////

float matProviderPopup::tempData_s[9] = {
    0, 0, 0,
    0, 1, 0,
    0, 0, 0
};

inline void matProviderPopup::show(mat& data)
{
    if (show_) {
        show_ = false;
        ImGui::OpenPopup("Provide the matrix");
    }

    if (ImGui::BeginPopupModal("Provide the matrix", nullptr, 0)) {

        ImGui::Text((std::to_string(temp_.getN()) + 'x' + std::to_string(temp_.getM())).c_str());

        if (ImGui::BeginTable("matProviderPopup-matrix-wrapper", 2, ImGuiTableColumnFlags_WidthStretch)) {
            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, ui::buttonHeight);

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
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

            ImGui::TableNextColumn();
            if (ImGui::BeginTable("matProviderPopup-matrix-columns-controls", 1)) {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                float rowHeight = ImGui::GetFontSize() + 2 * ImGui::GetStyle().FramePadding.y;
                if (ui::ButtonFillSpanXRows("+", static_cast<float>(temp_.getN()),
                    static_cast<float>(temp_.getN()) / 2, rowHeight)) {
                    temp_.addEmptyColumn();
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                if (ui::ButtonFillSpanXRows("-", static_cast<float>(temp_.getN()),
                    static_cast<float>(temp_.getN()) / 2, rowHeight)) {
                    temp_.removeLastColumn();
                }

                ImGui::EndTable();
            }

            ImGui::EndTable();
        }

        if (ImGui::BeginTable("matProviderPopup-matrix-row-controls", 3)) {
            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, ui::buttonHeight);

            ImGui::TableNextRow();

            ImGui::TableNextColumn();
            if (ui::ButtonFill("+")) {
                temp_.addEmptyRow();
            }

            ImGui::TableNextColumn();
            if (ui::ButtonFill("-")) {
                temp_.removeLastRow();
            }

            ImGui::EndTable();
        }
        ui::SeparatorWithSpacing();

        if (ImGui::BeginTable("matProviderPopup-matrix-controls", 2)) {
            ImGui::TableNextRow();

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
        ui::SeparatorWithSpacing();

        if (ImGui::BeginTable("matProviderPopup-controls-2", 2)) {
            ImGui::TableNextRow();

            ImGui::TableNextColumn();
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f/255.0f, 200.0f/255.0f, 0.0f/255.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f/255.0f, 150.0f/255.0f, 0.0f/255.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f/255.0f, 100.0f/255.0f, 0.0f/255.0f, 1.0f));
            if (ui::ButtonFill("ok")) {
                data = temp_;
                ImGui::CloseCurrentPopup();
            }
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(200.0f/255.0f, 0.0f/255.0f, 0.0f/255.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(150.0f/255.0f, 0.0f/255.0f, 0.0f/255.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(100.0f/255.0f, 0.0f/255.0f, 0.0f/255.0f, 1.0f));
            ImGui::TableNextColumn();
            if (ui::ButtonFill("cancel")) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();

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
