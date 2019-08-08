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

		void SelectWeapon1Left();

	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void SelectedWeapon1();

	UPlayerEquipmentMenu* EquipmentMenuInstance;
	APlayerController* playerController;
	AProjectMMWCharacter* AProjectMMWCharacterInstance;
	AWeapon* SelectedWeapon;
	//AGlobalSettings* GlobalSettings;
	TSubclassOf<AGlobalSettings> GlobalSettings;
	TMap<FString, TSubclassOf<AWeapon>> EquipableWeapons;
};
