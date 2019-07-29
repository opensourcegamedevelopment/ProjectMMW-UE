// @CopyRight 2018 Open Source Game Dev


#include "PlayerEquipmentMenu.h"
#include "PlayerStatusMenu.h"

UPlayerEquipmentMenu::UPlayerEquipmentMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UPlayerEquipmentMenu::InitialiseData()
{
	//if (PlayerStatusMenuWidgetInstance == nullptr)
	//{
	//	PlayerStatusMenuWidgetInstance = CreateWidget(GetWorld(), PlayerStatusMenuWidget);

	//	if (StatusMenuInstance == nullptr)
	//	{
	//		StatusMenuInstance = (UPlayerStatusMenu*)PlayerStatusMenuWidgetInstance;
	//		//StatusMenuInstance->InitialiseData();
	//	}
	//}
}

void UPlayerEquipmentMenu::ToggleStatusInventory()
{
	//if (PlayerStatusMenuWidget) {

		if (PlayerStatusMenuWidgetInstance != nullptr)
		{

			if (!PlayerStatusMenuWidgetInstance->GetIsVisible())
			{
				PlayerStatusMenuWidgetInstance->AddToViewport();
			}

			if (GetIsVisible())
			{
				RemoveFromViewport();
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerStatusMenuWidgetInstance does not exists!!"));
		}
	//}
}