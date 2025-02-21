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
        ImGui::PushID("matProviderPopup_columns_rowControls");
        ImGui::Columns(5, nullptr, false);
        constexpr auto addRow = "add row";
        constexpr auto removeRow = "remove row";
        constexpr auto addColumn = "add column";
        constexpr auto removeColumn = "remove column";
        ImGui::SetColumnWidth(0, ImGui::CalcTextSize(addRow).x + padding);
        ImGui::SetColumnWidth(1, ImGui::CalcTextSize(removeRow).x + padding);
        ImGui::SetColumnWidth(2, ImGui::CalcTextSize(addColumn).x + padding);
        ImGui::SetColumnWidth(3, ImGui::CalcTextSize(removeColumn).x + padding);
        if (ImGui::Button(addRow)) {
            temp_.addEmptyRow();
        }
        ImGui::NextColumn();
        if (ImGui::Button(removeRow)) {
            temp_.removeLastRow();
        }
        ImGui::NextColumn();
        if (ImGui::Button(addColumn)) {
            temp_.addEmptyColumn();
        }
        ImGui::NextColumn();
        if (ImGui::Button(removeColumn)) {
            temp_.removeLastColumn();
        }
        ImGui::NextColumn();
        ImGui::Text((std::to_string(temp_.getN()) + 'x' + std::to_string(temp_.getM())).c_str());
        ImGui::Columns(1);
        ImGui::PopID();
        ImGui::Separator();

        /* display by columns */
        ImGui::PushID("matProviderPopup_columns_matrixData");
        ImGui::Columns(temp_.getM(), nullptr, false);
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
        ImGui::PopID();
        ImGui::PushID("matProviderPopup_columns_controls");
        ImGui::Columns(4, nullptr, false);

        constexpr auto ok = "ok";
        constexpr auto cancel = "cancel";
        constexpr auto clear = "clear";
        constexpr auto normalize = "normalize";
        ImGui::SetColumnWidth(0, ImGui::CalcTextSize(ok).x + padding);
        ImGui::SetColumnWidth(1, ImGui::CalcTextSize(cancel).x + padding);
        ImGui::SetColumnWidth(2, ImGui::CalcTextSize(clear).x + padding);
        ImGui::SetColumnWidth(3, ImGui::CalcTextSize(normalize).x + padding);
        if (ImGui::Button(ok)) {
            data = temp_;
            ImGui::CloseCurrentPopup();
        }
        ImGui::NextColumn();
        if (ImGui::Button(cancel)) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::NextColumn();
        if (ImGui::Button(clear)) {
            temp_.clear();
        }
        ImGui::NextColumn();
        if (ImGui::Button(normalize)) {
            temp_.normalize();
        }
        ImGui::Columns(1);
        ImGui::PopID();
        ImGui::EndPopup();
    }
}

inline void matProviderPopup::setTemp(mat &data)
{
    temp_ = data;
}

#endif //MATPROVIDERWINDOW_HPP
