// @CopyRight 2018 Open Source Game Dev

#pragma once

#include "CoreMinimal.h"
//#include "PlayerStatusMenu.h"
#include "Blueprint/UserWidget.h"
#include "PlayerEquipmentMenu.generated.h"

class UPlayerStatusMenu;

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

	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets")
		TSubclassOf<UUserWidget> PlayerStatusMenuWidget;*/

	UPlayerStatusMenu* StatusMenuInstance;
	//UUserWidget* PlayerStatusMenuWidgetInstance;
private:
	
};
