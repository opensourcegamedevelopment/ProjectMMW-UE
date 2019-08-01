// @CopyRight 2018 Open Source Game Dev

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerEquipmentMenu.generated.h"

class UPlayerStatusMenu;
class UPlayerEquipmentSelectMenu;

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

	UPlayerStatusMenu* StatusMenuInstance;
	UPlayerEquipmentSelectMenu* EquipmentSelectMenuInstance;
private:
	
};
