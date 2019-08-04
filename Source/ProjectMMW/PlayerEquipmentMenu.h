// @CopyRight 2018 Open Source Game Dev

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerEquipmentMenu.generated.h"

class UPlayerStatusMenu;
class UPlayerEquipmentSelectMenu;
class AProjectMMWCharacter;
/**
 * 
 */
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

	UPlayerStatusMenu* StatusMenuInstance;
	UPlayerEquipmentSelectMenu* EquipmentSelectMenuInstance;
	AProjectMMWCharacter* AProjectMMWCharacterInstance;
private:
	
};
