#include "draw_manager.hpp"
#include "tahoma.hpp"
#include "tahoma_bold.hpp"
#include <misc/freetype/imgui_freetype.h>
#include "icons.hpp"
#include "utils.hpp"
#include "link.hpp"
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>
#include "verdana.hpp"

using namespace ImGui;
static bool once = false;

void EmbraceTheDarkness()
{
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
	colors[ImGuiCol_Border] = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
	colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
	colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
	colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_DockingPreview] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
	colors[ImGuiCol_DockingEmptyBg] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_TableBorderLight] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
	colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
	colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowPadding = ImVec2(8.00f, 8.00f);
	style.FramePadding = ImVec2(5.00f, 2.00f);
	style.CellPadding = ImVec2(6.00f, 6.00f);
	style.ItemSpacing = ImVec2(6.00f, 6.00f);
	style.ItemInnerSpacing = ImVec2(6.00f, 6.00f);
	style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
	style.IndentSpacing = 25;
	style.ScrollbarSize = 15;
	style.GrabMinSize = 10;
	style.WindowBorderSize = 1;
	style.ChildBorderSize = 1;
	style.PopupBorderSize = 1;
	style.FrameBorderSize = 1;
	style.TabBorderSize = 1;
	style.WindowRounding = 7;
	style.ChildRounding = 4;
	style.FrameRounding = 3;
	style.PopupRounding = 4;
	style.ScrollbarRounding = 9;
	style.GrabRounding = 3;
	style.LogSliderDeadzone = 4;
	style.TabRounding = 4;
}

//setting up of imgui
void render_state_manager::setup(IDirect3DDevice9* device)
{
	if (!once) {
		try {
			CreateContext();
			if (!ImGui_ImplWin32_Init(FindWindowA("Valve001", nullptr)))
				utilities::log("[RenderStateManager] Failed to initialize ImGui Win32 backend");

			if (!ImGui_ImplDX9_Init(device))
				utilities::log("[RenderStateManager] Failed to initialize ImGui DX9 backend");

			utilities::log("[RenderStateManager] initialized imgui");

			StyleColorsDark();
			EmbraceTheDarkness();
			
			ImGuiIO& io = GetIO();
			ImFontConfig cfg{};
			cfg.PixelSnapH = true;
			cfg.OversampleH = cfg.OversampleV = 1;
			cfg.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_ForceAutoHint | ImGuiFreeTypeBuilderFlags_LoadColor | ImGuiFreeTypeBuilderFlags_Bitmap;
		
			cfg.FontDataOwnedByAtlas = false;
			

			utilities::log("[RenderStateManager] adding fonts...");

			//the way the compiler did this is so fucking funny
			auto try_add_font = [&](auto font_data, int size, float font_size, const char* font_name, const ImWchar* glyph_ranges) -> ImFont* {
				ImFont* font = io.Fonts->AddFontFromMemoryTTF(font_data, size, font_size, &cfg, glyph_ranges);
				if (!font)
					utilities::log(std::format("[RenderStateManager] failed to load font: {}", font_name));
				return font;
				};

			//i cant and will not be bothered to give a fuck about verdana being a byte array.
			FONTS::FONT_ESP = try_add_font(verdana, sVerdana, 13.0f, "font_esp", io.Fonts->GetGlyphRangesCyrillic());
			FONTS::FONT_INDICATORS = try_add_font(verdana, sVerdana, 12.0f, "font_indicators", io.Fonts->GetGlyphRangesCyrillic());
			FONTS::FONT_MENU_TABS = try_add_font(verdana, sVerdana, 12.0f, "font_menu_tabs", io.Fonts->GetGlyphRangesCyrillic());
			FONTS::FONT_MENU = try_add_font(verdana, sVerdana, 10.0f, "font_menu", io.Fonts->GetGlyphRangesCyrillic());
			FONTS::FONT_ICOMOON = try_add_font(icomoon, sizeof(icomoon), 20.0f, "font_icomoon", io.Fonts->GetGlyphRangesCyrillic());
			FONTS::FONT_ICOMOON_WIDGET = try_add_font(icomoon_widget, sizeof(icomoon_widget), 15.0f, "font_icomoon_widget", io.Fonts->GetGlyphRangesCyrillic());
			FONTS::FONT_ICOMOON_WIDGET2 = try_add_font(icomoon, sizeof(icomoon), 16.0f, "font_icomoon_widget2", io.Fonts->GetGlyphRangesCyrillic());
			
			utilities::log("[RenderStateManager] font setup completed");
			once = true;
		}
		catch (const std::exception& e) {
			utilities::log(std::format("[RenderStateManager] exception during initialization: {}", e.what()));
		}
		catch (...) {
			utilities::log("[RenderStateManager] unknown fatal error during initialization");
		}
	}
}

//destroying the imgui context after we're done with it
void render_state_manager::shutdown()
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	DestroyContext();
	utilities::log("[RenderStateManager] shutdown");
}

//starting to render the frame
void render_state_manager::start()
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	NewFrame();
}

//ending the frame
void render_state_manager::end()
{
	
	ImGui::Render();

	ImDrawData* draw_data = GetDrawData();

	ImGui_ImplDX9_RenderDrawData(draw_data);
}

//setup device states for that color correction
void render_state_manager::setup_states(IDirect3DDevice9* device)
{
	/* 
	yes imgui does set all of these states below
	but im also setting the color correction one
	because fuck imgui
	*/

	device->CreateStateBlock(D3DSBT_ALL, &PixelState);

	PixelState->Capture();

	device->SetRenderState(D3DRS_LIGHTING, FALSE);
	device->SetRenderState(D3DRS_FOGENABLE, FALSE);
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
	device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	device->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
	device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);

	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_INVDESTALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);

	device->SetRenderState(D3DRS_SRGBWRITEENABLE, FALSE);
	device->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
}

//after we do the color correction we set back to the normal values
void render_state_manager::backup_states(IDirect3DDevice9* device)
{
	if (!PixelState)
		return;

	PixelState->Apply();
	PixelState->Release();
}

//erm this is very familiar, because it is lmao, not denying anything
void draw_commands_executor::draw_line(ImDrawList* list, int x1, int y1, int x2, int y2, color_t color)
{
	auto f_x1 = (float)x1;
	auto f_y1 = (float)y1;
	auto f_x2 = (float)x2;
	auto f_y2 = (float)y2;

	ImVec2 from = { f_x1, f_y1 };
	ImVec2 to = { f_x2, f_y2 };

	float alpha = color.to_float(color.a);
	ImColor shadowColor(0.f, 0.f, 0.f, alpha);
	ImColor lineColor(color.r, color.g, color.b, color.a);

	list->AddLine({ from.x + 1.f, from.y + 1.f }, { to.x + 1.f, to.y + 1.f }, shadowColor);
	list->AddLine({ from.x - 1.f, from.y - 1.f }, { to.x - 1.f, to.y - 1.f }, shadowColor);

	// Actual line
	list->AddLine(from, to, lineColor);
}

void draw_commands_executor::draw_string(ImDrawList* list, ImFont* font, int x, int y, color_t color, text_align align, std::string_view text, ...)
{
	if (text.empty())
		return;

	ImGui::PushFont(font);


	float  left, top;
	auto   f_x = static_cast<float>(x);
	auto   f_y = static_cast<float>(y);

	ImVec2 size = ImGui::CalcTextSize(text.data(), nullptr, false);

	if (align == text_align::horizontal) {
		f_x -= size.x / 2;
	}
	if (align == text_align::vertical) {
		f_y -= size.y / 2;
	}

	if (align == text_align::center) {
		f_x -= size.x / 2;
		f_y -= size.y / 2;
	}

	if (align == text_align::reverse_horizontal) {
		f_x -= size.x;
	}

	if (align == text_align::normal) {
		//do nothing
	}

	left = f_x;
	top = f_y + 1.f;
	list->AddText({ left, top }, ImColor(0.f, 0.f, 0.f, color.to_float(color.a)), text.data(), nullptr);
	left = f_x + 1.f;
	top = f_y;
	list->AddText({ left, top }, ImColor(0.f, 0.f, 0.f, color.to_float(color.a)), text.data(), nullptr);
	left = f_x - 1.f;
	top = f_y;
	list->AddText({ left, top }, ImColor(0.f, 0.f, 0.f, color.to_float(color.a)), text.data(), nullptr);
	left = f_x;
	top = f_y - 1.f;
	list->AddText({ left, top }, ImColor(0.f, 0.f, 0.f, color.to_float(color.a)), text.data(), nullptr);
	left = f_x;
	top = f_y;
	list->AddText({ left, top }, ImColor(color.r, color.g, color.b, color.a), text.data(), nullptr);
	ImGui::PopFont();
}

void draw_commands_executor::draw_cirlce(ImDrawList* list, int x, int y, float radius, color_t color)
{
	auto f_x = static_cast<float>(x);
	auto f_y = static_cast<float>(y);

	list->AddCircle({ f_x, f_y }, radius - 1.f, ImColor(0.f, 0.f, 0.f, color.to_float(color.a)));
	list->AddCircle({ f_x, f_y }, radius + 1.f, ImColor(0.f, 0.f, 0.f, color.to_float(color.a)));
	list->AddCircle({ f_x, f_y }, radius, ImColor(color.r, color.g, color.b, color.a));
}

void draw_commands_executor::draw_rect(ImDrawList* list, int x, int y, int w, int h, color_t color)
{
	auto f_x = static_cast<float>(x);
	auto f_y = static_cast<float>(y);
	auto f_w = static_cast<float>(w);
	auto f_h = static_cast<float>(h);

	list->AddRect({ f_x + 1.f, f_y + 1.f }, { f_x + f_w - 1.f, f_y + f_h - 1.f },
		ImColor(0.f, 0.f, 0.f, color.to_float(color.a)));
	list->AddRect({ f_x - 1.f, f_y - 1.f }, { f_x + f_w + 1.f, f_y + f_h + 1.f },
		ImColor(0.f, 0.f, 0.f, color.to_float(color.a)));

	list->AddRect({ f_x, f_y }, { f_x + f_w, f_y + f_h }, ImColor(color.r, color.g, color.b, color.a));
}

void draw_commands_executor::draw_filled_rect(ImDrawList* list, int x, int y, int w, int h, color_t color)
{
	auto f_x = static_cast<float>(x);
	auto f_y = static_cast<float>(y);
	auto f_w = static_cast<float>(w);
	auto f_h = static_cast<float>(h);

	list->AddRectFilled({ f_x + 1.f, f_y + 1.f }, { f_x + f_w + 1.f, f_y + f_h + 1.f }, ImColor(color.r, color.g, color.b, color.a));
}

void render_command_queue::render(ImDrawList* list)
{
	std::scoped_lock _{ m_mutex };

	for (auto&& cmds : m_safe_cmds) {
		if (auto* string = std::get_if<string_t>(&cmds)) {
			ctx.renderer.command_executor.draw_string(list, string->font, string->x, string->y, string->col, string->align, string->input);
		}
		if (auto* rect = std::get_if<rect_t>(&cmds)) {
			ctx.renderer.command_executor.draw_rect(list, rect->x, rect->y, rect->w, rect->h, rect->col);
		}
		if (auto* filledrect = std::get_if<filledrect_t>(&cmds)) {
			ctx.renderer.command_executor.draw_filled_rect(list, filledrect->x, filledrect->y, filledrect->w, filledrect->h, filledrect->col);
		}
		if (auto* line = std::get_if<line_t>(&cmds)) {
			ctx.renderer.command_executor.draw_line(list, line->x1, line->y1, line->x2, line->y2, line->col);
		}
		if (auto* circle = std::get_if<circle_t>(&cmds)) {
			ctx.renderer.command_executor.draw_cirlce(list, circle->x, circle->y, circle->radius, circle->col);
		}
	}
}

void render_command_queue::swap_commands()
{
	{
		std::scoped_lock _{ m_mutex };
		m_cmds.swap(m_safe_cmds);
	}

	m_cmds.clear();
}

void render_command_queue::line(int x1, int y1, int x2, int y2, color_t color)
{
	m_cmds.emplace_back(line_t{ x1, y1,x2, y2, color });
}

void render_command_queue::string(ImFont* font, int x, int y, color_t color, text_align align, std::string_view text, ...)
{
	m_cmds.emplace_back(string_t{ font,x, y, color,align, text });
}

void render_command_queue::circle(int x, int y, float radius, color_t color)
{
	m_cmds.emplace_back(circle_t{ x, y, radius, color });
}

void render_command_queue::rect(int x, int y, int w, int h, color_t color)
{
	m_cmds.emplace_back(rect_t{ x, y,w, h, color });
}

void render_command_queue::filled_rect(int x, int y, int w, int h, color_t color)
{
	m_cmds.emplace_back(filledrect_t{ x, y,w, h, color });
}
