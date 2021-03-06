//
// Created by MTesseracT on 2019-12-23.
//

#ifndef VALCANO_VALCANOAPPLICATION_HPP
#define VALCANO_VALCANOAPPLICATION_HPP

#include <aux/logging/Logger.hpp>
#include <gfx/vulkan/VulkanRenderer.hpp>
#include <gfx/window/WindowManager.hpp>
#include <iostream>

#include "gfx/window/ContextWindow.hpp"

namespace mt {
class ValcanoApplication {
    std::shared_ptr<gfx::WindowManager> window_manager;
    std::shared_ptr<gfx::ContextWindow> main_window;
    std::shared_ptr<gfx::Renderer> renderer;

public:
    ValcanoApplication(uint32_t win_width, uint32_t win_height,
        const std::string& app_name);

    void run();
};
} // namespace mt

int main()
{
    mt::ValcanoApplication app(800, 600, "Valcano Testing");

    try {
        app.run();
    } catch (const std::exception& e) {
        mt::Logger::log(e.what(), mt::LogType::Error);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

#endif // VALCANO_VALCANOAPPLICATION_HPP
