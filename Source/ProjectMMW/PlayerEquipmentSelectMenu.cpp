// @CopyRight 2018 Open Source Game Dev


#include "PlayerEquipmentSelectMenu.h"

UPlayerEquipmentSelectMenu::UPlayerEquipmentSelectMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UPlayerEquipmentSelectMenu::InitialiseData()
{
	this->AddToViewport();
	this->SetVisibility(ESlateVisibility::Hidden);
}