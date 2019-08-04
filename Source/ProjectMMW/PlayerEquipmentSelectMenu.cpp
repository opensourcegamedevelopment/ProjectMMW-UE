// @CopyRight 2018 Open Source Game Dev


#include "PlayerEquipmentSelectMenu.h"
#include "PlayerEquipmentMenu.h"
#include "ProjectMMWCharacter.h"

UPlayerEquipmentSelectMenu::UPlayerEquipmentSelectMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UPlayerEquipmentSelectMenu::InitialiseData()
{
	this->AddToViewport();
	this->SetVisibility(ESlateVisibility::Hidden);
}

void UPlayerEquipmentSelectMenu::BackButton_Clicked()
{
	if (EquipmentMenuInstance != nullptr)
	{

		if (!EquipmentMenuInstance->IsVisible())
		{
			EquipmentMenuInstance->SetVisibility(ESlateVisibility::Visible);
		}

		if (IsVisible())
		{
			this->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UPlayerEquipmentSelectMenu::CloseButton_Clicked()
{
	if (IsVisible())
	{
		this->SetVisibility(ESlateVisibility::Hidden);
		APlayerController* playerController = GetWorld()->GetFirstPlayerController();
		
		playerController->bShowMouseCursor = false;
		playerController->bEnableClickEvents = false;
		playerController->bEnableMouseOverEvents = false;
		playerController->SetIgnoreLookInput(false);
		AProjectMMWCharacterInstance = (AProjectMMWCharacter*)(playerController->GetCharacter());
		AProjectMMWCharacterInstance->SetInMenu(false);
	}
}