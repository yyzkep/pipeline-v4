#include "widgets.hpp"

//pasted from somwhere
bool edited::SliderScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, float power)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const float w = CalcItemWidth();
    const float h = GetFrameHeight();
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    const ImRect frame_bb(window->DC.CursorPos + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + h : 0.0f, label_size.x > 0.0f ? label_size.y : 0.0f), window->DC.CursorPos + ImVec2(w, (label_size.x > 0.0f ? label_size.y : 0.0f) + h));
    const ImRect total_bb(window->DC.CursorPos, frame_bb.Max - ImVec2((label_size.x > 0.0f ? style.ItemInnerSpacing.x + h : 0.0f), 0.0f));

    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id, &frame_bb))
        return false;

    // Default format string when passing NULL
    if (format == NULL)
        format = DataTypeGetInfo(data_type)->PrintFmt;
    else if (data_type == ImGuiDataType_S32 && strcmp(format, "%d") != 0) // (FIXME-LEGACY: Patch old "%.0f" format string to use "%d", read function more details.)
        format = PatchFormatStringFloatToInt(format);

    // Tabbing or CTRL-clicking on Slider turns it into an input box
    const bool hovered = ItemHoverable(frame_bb, id);
    bool temp_input_is_active = TempInputTextIsActive(id);
    bool temp_input_start = false;
    if (!temp_input_is_active)
    {
        const bool focus_requested = FocusableItemRegister(window, id);
        const bool clicked = (hovered && g.IO.MouseClicked[0]);
        if (focus_requested || clicked || g.NavActivateId == id || g.NavInputId == id)
        {
            SetActiveID(id, window);
            SetFocusID(id, window);
            FocusWindow(window);
            g.ActiveIdUsingNavDirMask |= (1 << ImGuiDir_Left) | (1 << ImGuiDir_Right);
            if (focus_requested || (clicked && g.IO.KeyCtrl) || g.NavInputId == id)
            {
                temp_input_start = true;
                FocusableItemUnregister(window);
            }
        }
    }
    if (temp_input_is_active || temp_input_start)
    {
        return TempInputTextScalar(total_bb, id, label, data_type, p_data, format);
    }

    // Draw frame
    RenderNavHighlight(frame_bb, id);
    PushStyleColor(ImGuiCol_Border, GetColorU32(g.ActiveId == id ? ImGuiCol_ControlBgActive : g.HoveredId == id ? ImGuiCol_ControlBgHovered : ImGuiCol_Border));
    RenderFrame(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_ControlBg), true, g.Style.FrameRounding);
    PopStyleColor();

    // Slider behavior
    ImRect grab_bb;
    const bool value_changed = SliderBehavior(frame_bb, id, data_type, p_data, p_min, p_max, format, power, ImGuiSliderFlags_None, &grab_bb);
    if (value_changed)
        MarkItemEdited(id);

    // Render grab
    if (grab_bb.Max.x > grab_bb.Min.x)
    {
        // slider grab
        window->DrawList->AddRectFilled(grab_bb.Min, grab_bb.Max, GetColorU32(g.ActiveId == id ? ImGuiCol_SliderGrabActive : ImGuiCol_SliderGrab), style.FrameRounding);
        // gradient
        window->DrawList->AddRectFilledMultiColor(grab_bb.Min, grab_bb.Max, GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, 0.05f)), GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, 0.05f)), GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, 0.38f)), GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, 0.38f)));
    }

    // Display value using user-provided display format so user can add prefix/suffix/decorations to the value.
    char value_buf[64];
    const char* value_buf_end = value_buf + DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), data_type, p_data, format);
    RenderText(ImVec2(frame_bb.Max.x - CalcTextSize(value_buf).x - 1.0f, total_bb.Min.y + style.FramePadding.y), value_buf, value_buf_end);

    if (label_size.x > 0.0f)
        RenderText(ImVec2(frame_bb.Min.x, total_bb.Min.y + style.FramePadding.y), label);

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, window->DC.ItemFlags);
    return value_changed;
}