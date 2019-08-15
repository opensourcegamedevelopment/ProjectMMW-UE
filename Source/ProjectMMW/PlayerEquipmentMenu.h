// @CopyRight 2018 Open Source Game Dev

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EWeaponSlot.h"
#include "PlayerEquipmentMenu.generated.h"

class UPlayerStatusMenu;
class UPlayerEquipmentSelectMenu;
class AProjectMMWCharacter;
/**
 * 
 */
UENUM(BlueprintType)
enum class WeaponSlot : uint8
{
	Weapon1Left UMETA(DisplayName = "Weapon 1 Left"),
	Weapon1Right UMETA(DisplayName = "Weapon 1 Right"),
	Weapon2Left UMETA(DisplayName = "Weapon 2 Left"),
	Weapon2Right UMETA(DisplayName = "Weapon 2 Right"),
	Weapon3Left UMETA(DisplayName = "Weapon 3 Left"),
	Weapon3Right UMETA(DisplayName = "Weapon 3 Right")
};
UCLASS()
class PROJECTMMW_API UPlayerEquipmentMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UPlayerEquipmentMenu(const FObjectInitializer& ObjectInitializer);
	void InitialiseData();

	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void ToggleStatusInventory();

	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void ToggleEquipmentSelectMenu();

	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void CloseButton_Clicked();

	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void SelectWeapon(EWeaponSlot weaponSlot);

	void PopulateUIData();

	UPlayerStatusMenu* StatusMenuInstance;
	UPlayerEquipmentSelectMenu* EquipmentSelectMenuInstance;
	APlayerController* playerController;
	AProjectMMWCharacter* AProjectMMWCharacterInstance;
private:
};
