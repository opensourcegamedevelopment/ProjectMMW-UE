// @CopyRight 2018 Open Source Game Dev

#pragma once

#include "EWeaponSlot.generated.h"

/**
 * 
 */
UENUM()
enum class EWeaponSlot : uint8
{
	Weapon1Left UMETA(DisplayName = "Weapon 1 Left"),
	Weapon1Right UMETA(DisplayName = "Weapon 1 Right"),
	Weapon2Left UMETA(DisplayName = "Weapon 2 Left"),
	Weapon2Right UMETA(DisplayName = "Weapon 2 Right"),
	Weapon3Left UMETA(DisplayName = "Weapon 3 Left"),
	Weapon3Right UMETA(DisplayName = "Weapon 3 Right")
};
