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
			EquipmentMenuInstance->PopulateUIData();
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
	if (SelectedWeapon == "None")
	{
		UTextBlock* textBlock = (UTextBlock*)this->GetWidgetFromName(FName("TxtEquipped"));
		textBlock->SetText(FText::FromString("None"));
		AProjectMMWCharacterInstance->ReEquipWeapon("None", "Weapon1_Left");
	}
	if (SelectedWeapon == "BeamRifle")
	{
		UTextBlock* textBlock = (UTextBlock*)this->GetWidgetFromName(FName("TxtEquipped"));
		textBlock->SetText(FText::FromString("BeamRifle"));
		AProjectMMWCharacterInstance->ReEquipWeapon("BeamRifle", "Weapon1_Left");
	}
	if (SelectedWeapon == "CannonRifle")
	{
		UTextBlock* textBlock = (UTextBlock*)this->GetWidgetFromName(FName("TxtEquipped"));
		textBlock->SetText(FText::FromString("CannonRifle"));
		AProjectMMWCharacterInstance->ReEquipWeapon("CannonRifle", "Weapon1_Left");
	}
}

void UPlayerEquipmentSelectMenu::SelectWeapon1Left()
{
	if (AProjectMMWCharacterInstance->Weapon1_Left != nullptr)
	{
		UTextBlock* textBlock = (UTextBlock*)this->GetWidgetFromName(FName("TxtEquipped"));
		FString weaponName = AProjectMMWCharacterInstance->Weapon1_Left->GetActorLabel();
		textBlock->SetText(FText::FromString(weaponName));
		selectedSlot = "Weapon1_Left";
	}
}

void UPlayerEquipmentSelectMenu::SelectedWeapon1()
{
	UTextBlock* textBlock = (UTextBlock*)this->GetWidgetFromName(FName("TxtSelected"));
	textBlock->SetText(FText::FromString("None"));
	SelectedWeapon = "None";
}

void UPlayerEquipmentSelectMenu::SelectedWeapon2()
{
	UTextBlock* textBlock = (UTextBlock*)this->GetWidgetFromName(FName("TxtSelected"));
	textBlock->SetText(FText::FromString("BeamRifle"));
	SelectedWeapon = "BeamRifle";
}

void UPlayerEquipmentSelectMenu::SelectedWeapon3()
{
	UTextBlock* textBlock = (UTextBlock*)this->GetWidgetFromName(FName("TxtSelected"));
	textBlock->SetText(FText::FromString("CannonRifle"));
	SelectedWeapon = "CannonRifle";
}