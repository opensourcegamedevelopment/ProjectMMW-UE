// @CopyRight 2018 Open Source Game Dev


#include "PlayerEquipmentMenu.h"
#include "PlayerStatusMenu.h"
#include "PlayerEquipmentSelectMenu.h"

UPlayerEquipmentMenu::UPlayerEquipmentMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UPlayerEquipmentMenu::InitialiseData()
{
	AddToViewport();
	this->SetVisibility(ESlateVisibility::Hidden);
}

void UPlayerEquipmentMenu::ToggleStatusInventory()
{
	if (StatusMenuInstance != nullptr)
	{

		if (!StatusMenuInstance->IsVisible())
		{
			StatusMenuInstance->SetVisibility(ESlateVisibility::Visible);
			//StatusMenuInstance->AddToViewport();
		}

		if (IsVisible())
		{
			//RemoveFromViewport();
			this->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerStatusMenuWidgetInstance does not exists!!"));
	}
}

void UPlayerEquipmentMenu::ToggleEquipmentSelectMenu()
{
	if (EquipmentSelectMenuInstance != nullptr)
	{

		if (!EquipmentSelectMenuInstance->IsVisible())
		{
			EquipmentSelectMenuInstance->SetVisibility(ESlateVisibility::Visible);
		}

		if (IsVisible())
		{
			this->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerStatusMenuWidgetInstance does not exists!!"));
	}
}