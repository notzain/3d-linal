#include "Gui.h"

#include "Engine.h"

static bool show_fps = true;

static struct frame_rate {
  int fps = 60;
  bool unlimited = true;
} frame_rate;

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

    ImGui::EndTabItem();
  }
}
