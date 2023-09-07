#pragma once

#include "mathtypes.h"
#include "pch.h"
#include <tuple>
#include <vector>
#include <sstream>
#include "globals.h"
#include <string>

namespace util {
    inline vector2 screen_size() {
        static auto size = vector2{ (float)GetSystemMetrics(SM_CXSCREEN), (float)GetSystemMetrics(SM_CYSCREEN) };
        return size;
    }
    inline FVector get_bone_position(ABP_PlayerCharacter_C* pawn, const char* bonename) {
		FVector socket_pos{ 0,0,0 };

		if (!pawn || !bonename) return socket_pos;

        auto mesh = pawn->Mesh;
        if (!mesh) return socket_pos;

        return mesh->GetSocketLocation(bonename);
    };
	inline int get_level(ABP_PlayerCharacter_C* pawn) {
		auto replicated_data = pawn->AccountDataReplication;
		return replicated_data.Level;
	}
	inline std::tuple<std::string, std::string> get_names(ABP_PlayerCharacter_C* pawn) {
		std::tuple<std::string, std::string> ret_tuple{"", ""};

		if (!pawn) return ret_tuple;

		auto replicated_data = pawn->AccountDataReplication;
		auto streaming_nickname = replicated_data.Nickname.StreamingModeNickName;
		auto original_nickname = replicated_data.Nickname.OriginalNickName;

		if (!original_nickname.IsValid() || !streaming_nickname.IsValid()) {
			return ret_tuple;
		}

		auto name = std::wstring(original_nickname.wc_str());
		if (name.length() <= 0) {
			return ret_tuple;
		}
		auto name_ascii = std::string(name.begin(), name.end());
		if (name_ascii.length() <= 0) {
			return ret_tuple;
		}		
		
		auto class_name = std::wstring(streaming_nickname.wc_str());
		if (class_name.length() <= 0) {
			return ret_tuple;
		}
		auto class_name_ascii = std::string(class_name.begin(), class_name.end());
		if (class_name_ascii.length() <= 0) {
			return ret_tuple;
		}

		return std::tuple<std::string, std::string>(name_ascii, class_name_ascii.substr(0, class_name_ascii.find("#")));

	}
	inline std::tuple<float, float> get_health(ABP_PlayerCharacter_C* pawn) {
		if (!pawn) return std::tuple<float, float>{};

		auto asc = pawn->AbilitySystemComponent;
		if (!asc) return std::tuple<float, float>{};

		auto attributes = asc->SpawnedAttributes[0];
		if (!attributes) return std::tuple<float, float>{};

		return std::tuple<float, float>{attributes->Health.CurrentValue, attributes->MaxHealth.CurrentValue};

	}
	inline std::vector<std::string> split(std::string s, std::string delimiter) {
		size_t pos_start = 0, pos_end, delim_len = delimiter.length();
		std::string token;
		std::vector<std::string> res;

		while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
			token = s.substr(pos_start, pos_end - pos_start);
			pos_start = pos_end + delim_len;
			res.push_back(token);
		}

		res.push_back(s.substr(pos_start));
		return res;
	}

	inline ABP_PlayerCharacter_C* get_closest_to_center(const char* bonename) {
		if (!gvar::all_initialized) return nullptr;

		FVector2D center_screen = util::screen_size() / 2.0f;

		ABP_PlayerCharacter_C* closest = nullptr;
		float closest_dist = FLT_MAX;
		auto player_array = gvar::game_state->PlayerArray;

		for (int i = 0; i < player_array.Count(); i++) {
			auto player = player_array[i];
			if (!player) continue;
			auto pawn = (ABP_PlayerCharacter_C*)player->PawnPrivate;
			if (!pawn) continue;

			FVector bone_pos = get_bone_position(pawn, bonename);
			FVector2D screen_pos;
			if (gvar::local_player_controller->ProjectWorldLocationToScreen(bone_pos, &screen_pos, 0)) {
				float dist = screen_pos.Distance(center_screen);
		
				if (dist < closest_dist) {
					closest_dist = dist;
					closest = pawn;
				}
			}
		}

		return closest;
	}


	
	inline std::vector<AItemActor*> get_items(ABP_PlayerCharacter_C* pawn) {
		std::vector<AItemActor*> items;
		
		auto equipment_inv = pawn->EquipmentInventory;
		if (!equipment_inv) return items;

	    auto items_tarr = equipment_inv->GetCurrentEquippedItemActors();

        for (int i = 0; i < items_tarr.Count(); i++) {
            AItemActor* item = items_tarr[i];
            if (item) {
                // Assuming AItemActor has a GetName() or similar function
				std::string itemName = item->GetDesignDataItem().Name.ToString();

                // Print the name of the item
                std::cout << "Item Name: " << itemName << std::endl;

                //items.push_back(item);
            }
        }
		return items;
	}
	
	inline std::tuple<bool, box> get_box(APlayerController* local_player_controller, ABP_PlayerCharacter_C* pawn) {
		box ret_box{};
		std::tuple<bool, box> return_tuple{ false, ret_box };

		if (!pawn) return return_tuple;

		auto rootcmp = pawn->RootComponent;
		if (!rootcmp) return return_tuple;

		auto location = rootcmp->RelativeLocation;

		CG::FVector origin, extent;
		pawn->GetActorBounds(true, &origin, &extent, 0);

		CG::FVector2D headpos;
		if (!local_player_controller->ProjectWorldLocationToScreen({ origin.X, origin.Y, location.Z + extent.Z + 20.f }, &headpos, 0)) {
			return return_tuple;
		}

		CG::FVector2D footpos;
		if (!local_player_controller->ProjectWorldLocationToScreen({ location.X, location.Y, location.Z - extent.Z - 20.f }, &footpos, 0)) {
			return return_tuple;
		}

		ret_box.height = (footpos - headpos).Y;
		ret_box.width = ret_box.height * 0.4f;
		ret_box.position = CG::FVector2D{ headpos.X - (ret_box.width / 2), headpos.Y};

		return std::tuple<bool, box>{true, ret_box};
	}
}