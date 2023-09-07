#pragma once

#include "pch.h"

namespace gvar {
	inline bool all_initialized;

	inline CG::UWorld* world;
	inline CG::UGameInstance* owning_game_instance;
	inline CG::APlayerController* local_player_controller;
	inline CG::APlayerCameraManager* local_camera_manager;
	inline CG::ABP_PlayerCharacter_C* local_pawn;
	inline CG::AGameStateBase* game_state;
	inline CG::ULocalPlayer* local_player;
	inline CG::USceneComponent* local_root_component;

	inline CG::ABP_PlayerCharacter_C* target;

	void cache_globals();
}