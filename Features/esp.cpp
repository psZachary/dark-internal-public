#include "pch.h"
#include "esp.h"
#include <cvar.h>
#include <iostream>
#include "../globals.h"

using namespace CG;

void features::target_loot_esp(Overlay* overlay) {
    
    if (!gvar::all_initialized || !gvar::target) {
        return;
    }

    auto [health, max] = util::get_health(gvar::target);

    if (health == 0 || max == 0) {
        return;
    }

    constexpr float WIDTH = 400;

    constexpr float HEIGHT = 14.f;

    const static D2D1::ColorF BACKGROUND_COLOR{ 0.047f, 0.047f, 0.047f, 1.f };

    vector2 position = vector2(util::screen_size().x - WIDTH, 0);


    auto inv = gvar::target->EquipmentInventory;
    if (!inv) {
        return;
    }

    auto items = inv->GetCurrentEquippedItemActors();

    overlay->draw_rect_filled(position, WIDTH, HEIGHT * items.Count(), BACKGROUND_COLOR);

    int stack_size = 0;

    for (int i = 0; i < items.Count(); i++) {

        auto loot_item = items[i];
        if (!loot_item) {
            continue;
        }

        auto design_data_item = loot_item->GetDesignDataItem();

        auto tag_name = design_data_item.IdTag.TagName.GetName();
        if (tag_name.size() <= 0) {
            continue;
        }
        auto rarity_type = design_data_item.RarityType.TagName.GetName();
        if (rarity_type.size() <= 0) {
            continue;
        }

        auto item_string = std::format("Item: [{}] Rarity: [{}]", util::split(tag_name, ".").back(), rarities[rarity_type]);

        overlay->draw_text(vector2{ position.x + 5.f, position.y + 12.f * stack_size }, item_string.c_str(), rarity_colors[rarity_type]);

        stack_size++;
    }
}

void features::loot_esp(Overlay* overlay) {
    if (!gvar::all_initialized) {
        return;
    }

    auto g_objects = UObject::GObjects;
    if (!g_objects) return;

    for (int i = 0; i < g_objects->Count(); i++) {
        CG::FVector2D screen_position;

        auto object = g_objects->GetByIndex(i);
        if (!object) continue;

        if (object->IsA(ABP_Chest_Base_C::StaticClass())) {
            ABP_Chest_Base_C* chest = (ABP_Chest_Base_C*)(object);

            auto rootcmp = chest->RootComponent;
            if (!rootcmp) continue;

            if (gvar::local_player_controller->ProjectWorldLocationToScreen(rootcmp->RelativeLocation, &screen_position, false)) {
                overlay->draw_text(convert_vector(screen_position), chest->Name.GetName().c_str(), {255, 255, 255, 255});
            }
        }
    }
}

void features::player_esp(Overlay* overlay) {
    if (!gvar::all_initialized) {
        return;
    }

    auto player_array = gvar::game_state->PlayerArray;
    if (player_array.Count() <= 0) return;

    for (int i = 0; i < player_array.Count(); i++) {
        if (!player_array.IsValidIndex(i)) {
            continue;
        }

        ADCPlayerState* actor = (ADCPlayerState*)player_array[i];
        if (!actor) {
            continue;
        }

        ABP_PlayerCharacter_C* pawn = (ABP_PlayerCharacter_C*)actor->PawnPrivate;
        if (!pawn || pawn->IsLocallyControlled()) {
            continue;
        }
        
        auto [health, max_health] = util::get_health(pawn);
        if (health <= 0 || max_health <= 0) {
            continue;
        }

        auto [name, class_name] = util::get_names(pawn);
        if (name.size() <= 0 || class_name.size() <= 0) continue;

        auto base_location = pawn->K2_GetActorLocation();
        auto is_target = gvar::target == pawn;

        auto [success, box] = util::get_box(gvar::local_player_controller, pawn);
        if (success) {
            int top_stack_size = 1;
            int bottom_stack_size = 0;
            if (cvar::config::player_esp::class_name) {
                overlay->draw_text(vector2{ (float)box.position.X, (float)box.position.Y - (top_stack_size * 14.f) }, class_name.c_str(), { 255, 255, 255, 255 });
                top_stack_size++;
            }
            if (cvar::config::player_esp::name) {
                overlay->draw_text(vector2{ (float)box.position.X, (float)box.position.Y - (top_stack_size * 14.f) }, name.c_str(), { 255, 255, 255, 255 });
                top_stack_size++;
            }
            if (cvar::config::player_esp::distance) {
                overlay->draw_text(vector2{ (float)box.position.X, (float)(box.position.Y + box.height + bottom_stack_size * 14.f) }, std::format("{0:.1f}m", gvar::local_root_component->RelativeLocation.DistanceMeter(base_location)).c_str(), { 255, 255, 255, 255 });
                bottom_stack_size++;
            }
            if (cvar::config::player_esp::level) {
                overlay->draw_text(vector2{ (float)box.position.X, (float)(box.position.Y + box.height + bottom_stack_size * 14.f) }, std::format("Level: {}", util::get_level(pawn)).c_str(), { 255, 255, 255, 255 });
                bottom_stack_size++;
            }

            if (cvar::config::player_esp::box) {
                auto [success, box] = util::get_box(gvar::local_player_controller, pawn);
                if (success) {
                    if (box.width != 0 && box.height != 0) {
                        overlay->draw_rect(convert_vector(box.position), box.width, box.height, is_target ? D2D1::ColorF{ 1.f, 0.f, 0.f, 1.f } : D2D1::ColorF{ 1.f, 1.f, 1.f, 1.f }, 1.0f);
                    }
                }
            }
            if (cvar::config::player_esp::health_bar) {
                float hp_percentage = static_cast<float>(health) / static_cast<float>(max_health);

                float health_bar_height = box.height * hp_percentage;

                float red = 1.0f - hp_percentage;
                float green = hp_percentage;
                float alpha = 1.0f; // 

                // Define the position and size of the health bar.
                vector2 health_bar_position{ (float)(box.position.X - 6.f), (float)(box.position.Y + box.height - health_bar_height) };
                vector2 health_bar_size{ 3.f, health_bar_height };

                // Draw the actual health bar (based on health percentage).
                overlay->draw_rect_filled(health_bar_position, health_bar_size.x, health_bar_size.y, { red, green, 0, alpha });
            }
        }

        if (cvar::config::player_esp::skeleton) {
            for (auto socket_pair : socket_skeleton) {
                auto socket1 = socket_pair.first;
                auto socket2 = socket_pair.second;

                auto socket1_location = util::get_bone_position(pawn, socket1.c_str());
                auto socket2_location = util::get_bone_position(pawn, socket2.c_str());
                CG::FVector2D socket1_screen_location{};
                CG::FVector2D socket2_screen_location{};

                if (gvar::local_player_controller->ProjectWorldLocationToScreen(socket1_location, &socket1_screen_location, 0)
                    && gvar::local_player_controller->ProjectWorldLocationToScreen(socket2_location, &socket2_screen_location, 0)) {
                    overlay->draw_line(convert_vector(socket1_screen_location), convert_vector(socket2_screen_location), { 255, 255, 255, 255 });
                }
            }
        }
    }
}
