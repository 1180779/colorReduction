//
// Created by rdkgs on 3/2/2025.
//

#ifndef UI_HPP
#define UI_HPP

#include <imGui/imgui.h>

/* wrappers around some ImGui controls */

namespace ui {
    static constexpr int buttonHeight = 25;

    inline void SeparatorWithSpacing()
    {
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
    }

    inline bool ButtonFillSpanXRows(const char* label, float numberOfRows, float spanRows, float rowHeight)
    {
        ImVec2 regionSize;
        float rowSpacing = ImGui::GetStyle().ItemSpacing.y;
        regionSize.y = (spanRows / numberOfRows) *
            (rowHeight * numberOfRows + rowSpacing * (numberOfRows - 1.0f));
        regionSize.x = buttonHeight;
        return ImGui::Button(label, regionSize);
    }

    inline bool ButtonFill(const char* label)
    {
        // fill whole available space with button
        ImVec2 regionSize = ImGui::GetContentRegionAvail();
        regionSize.y = buttonHeight;
        return ImGui::Button(label, regionSize);
    }

    inline bool ButtonFillY(const char* label)
    {
        // fill whole available space with button
        ImVec2 regionSize = ImGui::GetContentRegionAvail();
        regionSize.x = buttonHeight;
        regionSize.y = buttonHeight;
        return ImGui::Button(label, regionSize);
    }

    inline bool ComboFill(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items = -1)
    {
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        return ImGui::Combo(label, current_item, items, items_count, height_in_items);
    }

    inline bool InputFloatFill(const char* label, float* v, float step = 0, float step_fast = 0, const char* format = "%.3f", ImGuiInputTextFlags flags = 0)
    {
        ImVec2 regionSize = ImGui::GetContentRegionAvail();
        ImGui::SetNextItemWidth(regionSize.x);
        return ImGui::InputFloat(label, v, step, step_fast, format, flags);
    }
}

#endif //UI_HPP
