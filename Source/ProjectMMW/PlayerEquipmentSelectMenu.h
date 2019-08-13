// @CopyRight 2018 Open Source Game Dev

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerEquipmentSelectMenu.generated.h"

class UPlayerEquipmentMenu;
class AProjectMMWCharacter;
class AWeapon;
class AGlobalSettings;
/**
 * 
 */
//UENUM(BlueprintType)
//enum class WeaponSlot2 : uint8
//{
//	Weapon1Left UMETA(DisplayName = "Weapon 1 Left"),
//	Weapon1Right UMETA(DisplayName = "Weapon 1 Right"),
//	Weapon2Left UMETA(DisplayName = "Weapon 2 Left"),
//	Weapon2Right UMETA(DisplayName = "Weapon 2 Right"),
//	Weapon3Left UMETA(DisplayName = "Weapon 3 Left"),
//	Weapon3Right UMETA(DisplayName = "Weapon 3 Right")
//};
UCLASS()
class PROJECTMMW_API UPlayerEquipmentSelectMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UPlayerEquipmentSelectMenu(const FObjectInitializer& ObjectInitializer);
	void InitialiseData();

	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void BackButton_Clicked();

	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void CloseButton_Clicked();

	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void EquipButton_Clicked();

		/*void SelectWeapon(WeaponSlot weaponSlot);*/
	void SelectWeapon1Left();

	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void SelectedWeapon1();

	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void SelectedWeapon2();

	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void SelectedWeapon3();

	UPlayerEquipmentMenu* EquipmentMenuInstance;
	APlayerController* playerController;
	AProjectMMWCharacter* AProjectMMWCharacterInstance;
	FString SelectedWeapon;
	FString equippedWeapon;
	FString selectedSlot;
	//AGlobalSettings* GlobalSettings;
	TSubclassOf<AGlobalSettings> GlobalSettings;
	TMap<FString, TSubclassOf<AWeapon>> EquipableWeapons;
};
