// @CopyRight 2018 Open Source Game Dev

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerEquipmentSelectMenu.generated.h"

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
};
