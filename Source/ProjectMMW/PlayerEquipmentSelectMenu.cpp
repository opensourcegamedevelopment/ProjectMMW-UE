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
		playerController = GetWorld()->GetFirstPlayerController();
		
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
		AProjectMMWCharacterInstance->ReEquipWeapon("None", selectedSlot);
	}
	if (SelectedWeapon == "BeamRifle")
	{
		UTextBlock* textBlock = (UTextBlock*)this->GetWidgetFromName(FName("TxtEquipped"));
		textBlock->SetText(FText::FromString("BeamRifle"));
		AProjectMMWCharacterInstance->ReEquipWeapon("BeamRifle", selectedSlot);
	}
	if (SelectedWeapon == "CannonRifle")
	{
		UTextBlock* textBlock = (UTextBlock*)this->GetWidgetFromName(FName("TxtEquipped"));
		textBlock->SetText(FText::FromString("CannonRifle"));
		AProjectMMWCharacterInstance->ReEquipWeapon("CannonRifle", selectedSlot);
	}
}

void UPlayerEquipmentSelectMenu::SelectWeapon(EWeaponSlot weaponSlot)
{
	UTextBlock* textBlock = (UTextBlock*)this->GetWidgetFromName(FName("TxtEquipped"));

	switch (weaponSlot)
	{
		case EWeaponSlot::Weapon1Left:
			if (AProjectMMWCharacterInstance->Weapon1_Left != nullptr)
			{
				FString weaponName = AProjectMMWCharacterInstance->Weapon1_Left->GetClass()->GetDisplayNameText().ToString();
				textBlock->SetText(FText::FromString(weaponName));
			}
			else
			{
				textBlock->SetText(FText::FromString("None"));
			}
			selectedSlot = "Weapon1_Left";
			break;
		case EWeaponSlot::Weapon1Right:
			if (AProjectMMWCharacterInstance->Weapon1_Right != nullptr)
			{
				FString weaponName = AProjectMMWCharacterInstance->Weapon1_Right->GetClass()->GetDisplayNameText().ToString();
				textBlock->SetText(FText::FromString(weaponName));
			}
			else
			{
				textBlock->SetText(FText::FromString("None"));
			}
			selectedSlot = "Weapon1_Right";
			break;
		case EWeaponSlot::Weapon2Left:
			if (AProjectMMWCharacterInstance->Weapon2_Left != nullptr)
			{
				FString weaponName = AProjectMMWCharacterInstance->Weapon2_Left->GetClass()->GetDisplayNameText().ToString();
				textBlock->SetText(FText::FromString(weaponName));
			}
			else
			{
				textBlock->SetText(FText::FromString("None"));
			}
			selectedSlot = "Weapon2_Left";
			break;
		case EWeaponSlot::Weapon2Right:
			if (AProjectMMWCharacterInstance->Weapon2_Right != nullptr)
			{
				FString weaponName = AProjectMMWCharacterInstance->Weapon2_Right->GetClass()->GetDisplayNameText().ToString();
				textBlock->SetText(FText::FromString(weaponName));
			}
			else
			{
				textBlock->SetText(FText::FromString("None"));
			}
			selectedSlot = "Weapon2_Right";
			break;
		case EWeaponSlot::Weapon3Left:
			if (AProjectMMWCharacterInstance->Weapon3_Left != nullptr)
			{
				FString weaponName = AProjectMMWCharacterInstance->Weapon3_Left->GetClass()->GetDisplayNameText().ToString();
				textBlock->SetText(FText::FromString(weaponName));
			}
			else
			{
				textBlock->SetText(FText::FromString("None"));
			}
			selectedSlot = "Weapon3_Left";
			break;
		case EWeaponSlot::Weapon3Right:
			if (AProjectMMWCharacterInstance->Weapon3_Right != nullptr)
			{
				FString weaponName = AProjectMMWCharacterInstance->Weapon3_Right->GetClass()->GetDisplayNameText().ToString();
				textBlock->SetText(FText::FromString(weaponName));
			}
			else
			{
				textBlock->SetText(FText::FromString("None"));
			}
			selectedSlot = "Weapon3_Right";
			break;
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