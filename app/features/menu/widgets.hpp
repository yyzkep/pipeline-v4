#pragma once
#include <imgui.h>

namespace edited {
    bool SliderScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, float power);

}
