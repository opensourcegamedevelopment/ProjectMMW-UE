// @CopyRight 2018 Open Source Game Dev


#include "PlayerStatusMenu.h"

UPlayerStatusMenu::UPlayerStatusMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UPlayerStatusMenu::InitialiseData()
{
	tree = this->WidgetTree;
	UWidget* MaxHealthIcon_1_Widget = tree->FindWidget("MaxHealthStats1_Image");
	MaxHealthIcon_1_image = (UImage*)MaxHealthIcon_1_Widget;
	UWidget* MaxHealthIcon_2_Widget = tree->FindWidget("MaxHealthStats2_Image");
	MaxHealthIcon_2_image = (UImage*)MaxHealthIcon_2_Widget;
	UWidget* MaxHealthIcon_3_Widget = tree->FindWidget("MaxHealthStats3_Image");
	MaxHealthIcon_3_image = (UImage*)MaxHealthIcon_3_Widget;
	UWidget* MaxHealthIcon_4_Widget = tree->FindWidget("MaxHealthStats4_Image");
	MaxHealthIcon_4_image = (UImage*)MaxHealthIcon_4_Widget;
	UWidget* MaxHealthIcon_5_Widget = tree->FindWidget("MaxHealthStats5_Image");
	MaxHealthIcon_5_image = (UImage*)MaxHealthIcon_5_Widget;

	UWidget* MaxEnergyIcon_1_Widget = tree->FindWidget("MaxEnergyStats1_Image");
	MaxEnergyIcon_1_image = (UImage*)MaxEnergyIcon_1_Widget;
	UWidget* MaxEnergyIcon_2_Widget = tree->FindWidget("MaxEnergyStats2_Image");
	MaxEnergyIcon_2_image = (UImage*)MaxEnergyIcon_2_Widget;
	UWidget* MaxEnergyIcon_3_Widget = tree->FindWidget("MaxEnergyStats3_Image");
	MaxEnergyIcon_3_image = (UImage*)MaxEnergyIcon_3_Widget;
	UWidget* MaxEnergyIcon_4_Widget = tree->FindWidget("MaxEnergyStats4_Image");
	MaxEnergyIcon_4_image = (UImage*)MaxEnergyIcon_4_Widget;
	UWidget* MaxEnergyIcon_5_Widget = tree->FindWidget("MaxEnergyStats5_Image");
	MaxEnergyIcon_5_image = (UImage*)MaxEnergyIcon_5_Widget;
}

void UPlayerStatusMenu::SetAssignedMaxHealthPoints(int points)
{
	switch (points)
	{
	case 1:
		MaxHealthIcon_1_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxHealthIcon_2_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		MaxHealthIcon_3_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		MaxHealthIcon_4_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		MaxHealthIcon_5_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		break;
	case 2:
		MaxHealthIcon_1_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxHealthIcon_2_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxHealthIcon_3_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		MaxHealthIcon_4_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		MaxHealthIcon_5_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		break;
	case 3:
		MaxHealthIcon_1_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxHealthIcon_2_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxHealthIcon_3_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxHealthIcon_4_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		MaxHealthIcon_5_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		break;
	case 4:
		MaxHealthIcon_1_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxHealthIcon_2_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxHealthIcon_3_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxHealthIcon_4_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxHealthIcon_5_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		break;
	case 5:
		MaxHealthIcon_1_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxHealthIcon_2_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxHealthIcon_3_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxHealthIcon_4_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxHealthIcon_5_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		break;
	}
}

void UPlayerStatusMenu::SetAssignedMaxEnergyPoints(int points)
{
	switch (points)
	{
	case 1:
		MaxEnergyIcon_1_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxEnergyIcon_2_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		MaxEnergyIcon_3_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		MaxEnergyIcon_4_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		MaxEnergyIcon_5_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		break;
	case 2:
		MaxEnergyIcon_1_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxEnergyIcon_2_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxEnergyIcon_3_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		MaxEnergyIcon_4_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		MaxEnergyIcon_5_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		break;
	case 3:
		MaxEnergyIcon_1_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxEnergyIcon_2_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxEnergyIcon_3_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxEnergyIcon_4_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		MaxEnergyIcon_5_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		break;
	case 4:
		MaxEnergyIcon_1_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxEnergyIcon_2_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxEnergyIcon_3_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxEnergyIcon_4_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxEnergyIcon_5_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		break;
	case 5:
		MaxEnergyIcon_1_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxEnergyIcon_2_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxEnergyIcon_3_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxEnergyIcon_4_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxEnergyIcon_5_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		break;
	}
}

//void UPlayerStatusMenu::SetMaxHpStats(int points)
//{
//	return currentHealthPercentage;
//}