#pragma once
#include "imconfig.hpp"
#define IMGUI_DEFINE_MATH_OPERATORS
#include <d3d9.h>
#include <mutex>
#include <variant>
#include <vector>
#include <string_view>

#include <imgui.h>
#include "color.hpp"

enum text_align {
    horizontal,
    vertical,
    reverse_horizontal,
    center,
    normal
};

namespace FONTS {
    inline ImFont* FONT_ESP = nullptr;
    inline ImFont* FONT_INDICATORS = nullptr;
    inline ImFont* FONT_MENU = nullptr;
    inline ImFont* FONT_MENU_TABS = nullptr;
    inline ImFont* FONT_ICOMOON = nullptr;
    inline ImFont* FONT_ICOMOON_WIDGET = nullptr;
    inline ImFont* FONT_ICOMOON_WIDGET2 = nullptr;
}

class render_state_manager {
    IDirect3DStateBlock9* PixelState = nullptr;

public:
    void setup(IDirect3DDevice9* device);
    void shutdown();
    void start();
    void end();
    void setup_states(IDirect3DDevice9* device);
    void backup_states(IDirect3DDevice9* device);
};

class draw_commands_executor : public render_state_manager {
public:
    void draw_line(ImDrawList* list, int x1, int y1, int x2, int y2, color_t color);
    void draw_string(ImDrawList* list, ImFont* font, int x, int y, color_t color, text_align align, std::string_view text, ...);
    void draw_cirlce(ImDrawList* list, int x, int y, float radius, color_t color);
    void draw_rect(ImDrawList* list, int x, int y, int w, int h, color_t color);
    void draw_filled_rect(ImDrawList* list, int x, int y, int w, int h, color_t color);
};

class render_command_queue {
private:
    struct string_t {
        ImFont* font{};
        int x{}, y{};
        color_t col{};
        text_align align{};
        std::string_view input{};
    };

    struct rect_t {
        int x{}, y{}, w{}, h{};
        color_t col{};
    };

    struct filledrect_t {
        int x{}, y{}, w{}, h{};
        color_t col{};
    };

    struct line_t {
        int x1{}, y1{}, x2{}, y2{};
        color_t col{};
    };

    struct circle_t {
        int x{}, y{};
        float radius{};
        color_t col{};
    };

    std::mutex m_mutex{};
    std::vector<std::variant<string_t, rect_t, filledrect_t, line_t, circle_t>> m_cmds{}, m_safe_cmds{};

public:
    // execution
    void render(ImDrawList* list);
    void swap_commands();

    // command queue
    void line(int x1, int y1, int x2, int y2, color_t color);
    void string(ImFont* font, int x, int y, color_t color, text_align align, std::string_view text, ...);
    void circle(int x, int y, float radius, color_t color);
    void rect(int x, int y, int w, int h, color_t color);
    void filled_rect(int x, int y, int w, int h, color_t color);
};

