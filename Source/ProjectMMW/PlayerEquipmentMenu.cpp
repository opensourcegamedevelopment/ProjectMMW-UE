// @CopyRight 2018 Open Source Game Dev


#include "PlayerEquipmentMenu.h"
#include "PlayerStatusMenu.h"
#include "PlayerEquipmentSelectMenu.h"
#include "ProjectMMWCharacter.h"
#include "TextBlock.h"

UPlayerEquipmentMenu::UPlayerEquipmentMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UPlayerEquipmentMenu::InitialiseData()
{
	AddToViewport(1);
	this->SetVisibility(ESlateVisibility::Hidden);
	playerController = GetWorld()->GetFirstPlayerController();
	AProjectMMWCharacterInstance = (AProjectMMWCharacter*)(playerController->GetCharacter());
	PopulateUIData();
}

void UPlayerEquipmentMenu::PopulateUIData()
{
	if (AProjectMMWCharacterInstance->Weapon1_Left != nullptr)
	{
		UTextBlock* textBlock = (UTextBlock*)this->GetWidgetFromName(FName("TxtWeapon1Left"));
		FString weaponName = AProjectMMWCharacterInstance->Weapon1_Left->GetActorLabel();
		textBlock->SetText(FText::FromString(weaponName));
	}
	if (AProjectMMWCharacterInstance->Weapon1_Right != nullptr)
	{
		UTextBlock* textBlock = (UTextBlock*)this->GetWidgetFromName(FName("TxtWeapon1Right"));
		FString weaponName = AProjectMMWCharacterInstance->Weapon1_Right->GetActorLabel();
		textBlock->SetText(FText::FromString(weaponName));
	}
	if (AProjectMMWCharacterInstance->Weapon2_Left != nullptr)
	{
		UTextBlock* textBlock = (UTextBlock*)this->GetWidgetFromName(FName("TxtWeapon2Left"));
		FString weaponName = AProjectMMWCharacterInstance->Weapon2_Left->GetActorLabel();
		textBlock->SetText(FText::FromString(weaponName));
	}
	if (AProjectMMWCharacterInstance->Weapon2_Right != nullptr)
	{
		UTextBlock* textBlock = (UTextBlock*)this->GetWidgetFromName(FName("TxtWeapon2Right"));
		FString weaponName = AProjectMMWCharacterInstance->Weapon2_Right->GetActorLabel();
		textBlock->SetText(FText::FromString(weaponName));
	}
	if (AProjectMMWCharacterInstance->Weapon3_Left != nullptr)
	{
		UTextBlock* textBlock = (UTextBlock*)this->GetWidgetFromName(FName("TxtWeapon3Left"));
		FString weaponName = AProjectMMWCharacterInstance->Weapon3_Left->GetActorLabel();
		textBlock->SetText(FText::FromString(weaponName));
	}
	if (AProjectMMWCharacterInstance->Weapon3_Right != nullptr)
	{
		UTextBlock* textBlock = (UTextBlock*)this->GetWidgetFromName(FName("TxtWeapon3Right"));
		FString weaponName = AProjectMMWCharacterInstance->Weapon3_Right->GetActorLabel();
		textBlock->SetText(FText::FromString(weaponName));
	}
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

void UPlayerEquipmentMenu::CloseButton_Clicked()
{
	if (IsVisible())
	{
		this->SetVisibility(ESlateVisibility::Hidden);
		playerController->bShowMouseCursor = false;
		playerController->bEnableClickEvents = false;
		playerController->bEnableMouseOverEvents = false;
		playerController->SetIgnoreLookInput(false);
		
		AProjectMMWCharacterInstance = (AProjectMMWCharacter*)(playerController->GetCharacter());
		AProjectMMWCharacterInstance->SetInMenu(false);
	}
}

void UPlayerEquipmentMenu::SelectWeapon(WeaponSlot weaponSlot)
{
	if (EquipmentSelectMenuInstance != nullptr)
	{
		switch (weaponSlot) 
		{
			case WeaponSlot::Weapon1Left : EquipmentSelectMenuInstance->SelectWeapon1Left();  break;
			case WeaponSlot::Weapon1Right: EquipmentSelectMenuInstance->SelectWeapon1Left();  break;
			case WeaponSlot::Weapon2Left: EquipmentSelectMenuInstance->SelectWeapon1Left();  break;
			case WeaponSlot::Weapon2Right: EquipmentSelectMenuInstance->SelectWeapon1Left();  break;
			case WeaponSlot::Weapon3Left: EquipmentSelectMenuInstance->SelectWeapon1Left();  break;
			case WeaponSlot::Weapon3Right: EquipmentSelectMenuInstance->SelectWeapon1Left();  break;
		}

	}
}