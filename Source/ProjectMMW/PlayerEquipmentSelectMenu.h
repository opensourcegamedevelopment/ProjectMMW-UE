// @CopyRight 2018 Open Source Game Dev

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EWeaponSlot.h"
#include "PlayerEquipmentSelectMenu.generated.h"

class UPlayerEquipmentMenu;
class AProjectMMWCharacter;
class AWeapon;
class AGlobalSettings;
/**
 * 
 */
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

	
	void SelectWeapon(EWeaponSlot weaponSlot);

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
	TSubclassOf<AGlobalSettings> GlobalSettings;
	TMap<FString, TSubclassOf<AWeapon>> EquipableWeapons;
};
