#include "Gui.h"

#include "Engine.h"

static bool show_fps = true;

static struct frame_rate {
  int fps = 60;
  bool unlimited = true;
} frame_rate;

static float mouse_sensitivity = .5f;

static struct render_settings {

  bool wire_frame = true;
  bool solid = false;
  bool see_through = true;
} render_settings;

void GUI::draw_engine() {
  if (ImGui::BeginTabItem("Engine")) {
    if (ImGui::Checkbox("Show FPS", &show_fps))
      Engine::get().show_fps(show_fps);

    if (ImGui::SliderInt("FPS", &frame_rate.fps, 10, 144)) {
      Engine::get().set_framerate(frame_rate.fps);
      frame_rate.unlimited = false;
    }
    ImGui::SameLine();
    if (ImGui::Checkbox("Unlimited", &frame_rate.unlimited)) {
      Engine::get().set_framerate(frame_rate.unlimited ? 0 : frame_rate.fps);
    }

    if (ImGui::SliderFloat("Mouse sensitivity", &mouse_sensitivity, 0.f, 1.f)) {
      Engine::get().set_mouse_sensitivyt(mouse_sensitivity);
    }

    if (ImGui::Checkbox("Wireframe", &render_settings.wire_frame)) {
      Engine::get().get_render_settings().render_type ^= RenderType::WIREFRAME;
    }
    ImGui::SameLine();
    if (ImGui::Checkbox("SeeThrough", &render_settings.see_through)) {
      Engine::get().get_render_settings().see_through =
          render_settings.see_through;
    }
    if (ImGui::Checkbox("Solid", &render_settings.solid)) {
      Engine::get().get_render_settings().render_type ^= RenderType::SOLID;
    }

    ImGui::EndTabItem();
  }
}
