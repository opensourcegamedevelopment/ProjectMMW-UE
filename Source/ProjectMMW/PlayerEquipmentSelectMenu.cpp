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
	//TSubclassOf<AGlobalSettings> GlobalSettingsActorClass;
	//GlobalSettingsActorClass = GlobalSettings->GeneratedClass;
	//EquipableWeapons = GlobalSettingsActorClass.GetDefaultObject()->GetEquipableWeapons();
	//EquipableWeapons = GlobalSettings->GetEquipableWeapons();
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

void UPlayerEquipmentSelectMenu::EquipButton_Clicked()
{

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

void UPlayerEquipmentSelectMenu::SelectedWeapon1()
{
	UTextBlock* textBlock = (UTextBlock*)this->GetWidgetFromName(FName("TxtSelected"));
	//FString weaponName = AProjectMMWCharacterInstance->Weapon1_Left->GetActorLabel();
	textBlock->SetText(FText::FromString("BeamRifle"));
	TSubclassOf<AWeapon>* BeamRiflePtr = EquipableWeapons.Find("BeamRifle");
	TSubclassOf<AWeapon> BeamRifle = BeamRiflePtr->Get();
	SelectedWeapon = BeamRifle->GetDefaultObject<ABeamRifle>();
}