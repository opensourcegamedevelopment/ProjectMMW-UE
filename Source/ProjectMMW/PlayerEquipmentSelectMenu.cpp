// @CopyRight 2018 Open Source Game Dev


#include "PlayerEquipmentSelectMenu.h"
#include "PlayerEquipmentMenu.h"
#include "ProjectMMWCharacter.h"
#include "TextBlock.h"

UPlayerEquipmentSelectMenu::UPlayerEquipmentSelectMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UPlayerEquipmentSelectMenu::InitialiseData()
{
	this->AddToViewport(1);
	this->SetVisibility(ESlateVisibility::Hidden);
	playerController = GetWorld()->GetFirstPlayerController();
	AProjectMMWCharacterInstance = (AProjectMMWCharacter*)(playerController->GetCharacter());
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

void UPlayerEquipmentSelectMenu::SelectWeapon1Left()
{
	if (AProjectMMWCharacterInstance->Weapon1_Left != nullptr)
	{
		UTextBlock* textBlock = (UTextBlock*)this->GetWidgetFromName(FName("TxtEquipped"));
		FString weaponName = AProjectMMWCharacterInstance->Weapon1_Left->GetActorLabel();
		textBlock->SetText(FText::FromString(weaponName));
	}
}