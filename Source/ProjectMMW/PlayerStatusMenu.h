// @CopyRight 2018 Open Source Game Dev

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Image.h"
#include "PlayerStatusMenu.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMMW_API UPlayerStatusMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPlayerStatusMenu(const FObjectInitializer& ObjectInitializer);
	void InitialiseData();
	void SetMaxHpStats(int points);

	void SetAssignedMaxHealthPoints(int points);
	void SetAssignedMaxEnergyPoints(int points);
protected:

private:

	UWidgetTree* tree;
	UImage* MaxHealthIcon_1_image;
	UImage* MaxHealthIcon_2_image;
	UImage* MaxHealthIcon_3_image;
	UImage* MaxHealthIcon_4_image;
	UImage* MaxHealthIcon_5_image;

	UImage* MaxEnergyIcon_1_image;
	UImage* MaxEnergyIcon_2_image;
	UImage* MaxEnergyIcon_3_image;
	UImage* MaxEnergyIcon_4_image;
	UImage* MaxEnergyIcon_5_image;
};
