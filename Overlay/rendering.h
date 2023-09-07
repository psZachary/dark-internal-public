
#include "../overlay/overlay.h"
#include <iostream>
#include <format>
#include "overlay_menu.h"
#include "../cvar.h"
#include "globals.h"
#include "util.h"
#include <Features/esp.h>
#define ON_COLOR OverlayCord::Drawing::Pixel{ 0, 255, 0, 255 }
#define OFF_COLOR OverlayCord::Drawing::Pixel{ 0, 0, 255, 255 }
#define MENU_ITEM OverlayCord::Drawing::Pixel{ 255, 0, 0, 255 }
#define WHITE OverlayCord::Drawing::Pixel{ 255, 255, 255, 255 }

inline Overlay* overlay = 0;

void close() {
    overlay->begin_scene();
    overlay->clear_scene();
    overlay->end_scene();
    
    abort();
    exit(0);
}
inline void render() {
    
    // https://www.onlinewebfonts.com/download/f4068538ccc20dbc414f8ef0d0050715
    overlay = new Overlay(L"CoreDodamW01-Regular", 16.0);


    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    if (!overlay->init())
    {
        std::cout << std::format("failed to connect to overlay: {}", 1) << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(6));
        return;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(500));


    if (!overlay->startup_d2d())
    {
        std::cout << std::format("failed to connect to overlay: {}", 1) << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(6));
        return;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    overlay->begin_scene();
    overlay->clear_scene();
    overlay->end_scene();

    /*
    Add your own overlay / menu here.

    static overlay_window* window = new overlay_window(overlay, "bot", vector2(util::screen_size().x / 2 - (WIDTH / 2), util::screen_size().y / 2 - (HEIGHT / 2)));

    window->add_tab("Player ESP", {
        new checkbox("Enabled", &cvar::config::player_esp::enabled),
        new checkbox("Name", &cvar::config::player_esp::name),
        new checkbox("Class Name", &cvar::config::player_esp::class_name),
        new checkbox("Box", &cvar::config::player_esp::box),
        new checkbox("Health Bar", &cvar::config::player_esp::health_bar),
        new checkbox("Skeleton", &cvar::config::player_esp::skeleton),
        new checkbox("Distance", &cvar::config::player_esp::distance),
        new checkbox("Level", &cvar::config::player_esp::level),
        new checkbox("View Target Items", &cvar::config::player_esp::target_loot_esp),
    });
    window->add_tab("Loot ESP", {
        new checkbox("Enabled", &cvar::config::loot_esp::enabled)
    });
    window->add_tab("Misc", {
        new button("Close Cheat", close)
    });
    */


    while (true) {
        /*
        static bool is_ui_open = false;

        if (GetAsyncKeyState(VK_INSERT) & 1) {
            is_ui_open = !is_ui_open;
        }
        */
        
        overlay->begin_scene();
        overlay->clear_scene();

        if (is_ui_open) window->render();

        if (cvar::config::loot_esp::enabled) {
            features::loot_esp(overlay);
        }

        if (cvar::config::player_esp::enabled) {
            features::player_esp(overlay);
        }
        
        if (cvar::config::player_esp::target_loot_esp) {
            features::target_loot_esp(overlay);
        }
  
        overlay->end_scene();

        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
}