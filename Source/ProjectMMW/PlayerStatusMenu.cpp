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

	UWidget* EnergyRegenIcon_1_Widget = tree->FindWidget("EnergyRegenStats1_Image");
	EnergyRegenIcon_1_image = (UImage*)EnergyRegenIcon_1_Widget;
	UWidget* EnergyRegenIcon_2_Widget = tree->FindWidget("EnergyRegenStats2_Image");
	EnergyRegenIcon_2_image = (UImage*)EnergyRegenIcon_2_Widget;
	UWidget* EnergyRegenIcon_3_Widget = tree->FindWidget("EnergyRegenStats3_Image");
	EnergyRegenIcon_3_image = (UImage*)EnergyRegenIcon_3_Widget;
	UWidget* EnergyRegenIcon_4_Widget = tree->FindWidget("EnergyRegenStats4_Image");
	EnergyRegenIcon_4_image = (UImage*)EnergyRegenIcon_4_Widget;
	UWidget* EnergyRegenIcon_5_Widget = tree->FindWidget("EnergyRegenStats5_Image");
	EnergyRegenIcon_5_image = (UImage*)EnergyRegenIcon_5_Widget;

	/*UWidget* FlightPowerIcon_1_Widget = tree->FindWidget("FlightPowerStats1_Image");
	FlightPowerIcon_1_image = (UImage*)FlightPowerIcon_1_Widget;
	UWidget* FlightPowerIcon_2_Widget = tree->FindWidget("FlightPowerStats2_Image");
	FlightPowerIcon_2_image = (UImage*)FlightPowerIcon_2_Widget;
	UWidget* FlightPowerIcon_3_Widget = tree->FindWidget("FlightPowerStats3_Image");
	FlightPowerIcon_3_image = (UImage*)FlightPowerIcon_3_Widget;
	UWidget* FlightPowerIcon_4_Widget = tree->FindWidget("FlightPowerStats4_Image");
	FlightPowerIcon_4_image = (UImage*)FlightPowerIcon_4_Widget;
	UWidget* FlightPowerIcon_5_Widget = tree->FindWidget("FlightPowerStats5_Image");
	FlightPowerIcon_5_image = (UImage*)FlightPowerIcon_5_Widget;*/
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

void UPlayerStatusMenu::SetAssignedEnergyRegenPoints(int points)
{
	switch (points)
	{
	case 1:
		EnergyRegenIcon_1_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		EnergyRegenIcon_2_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		EnergyRegenIcon_3_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		EnergyRegenIcon_4_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		EnergyRegenIcon_5_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		break;
	case 2:
		EnergyRegenIcon_1_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		EnergyRegenIcon_2_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		EnergyRegenIcon_3_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		EnergyRegenIcon_4_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		EnergyRegenIcon_5_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		break;
	case 3:
		EnergyRegenIcon_1_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		EnergyRegenIcon_2_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		EnergyRegenIcon_3_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		EnergyRegenIcon_4_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		EnergyRegenIcon_5_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		break;
	case 4:
		EnergyRegenIcon_1_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		EnergyRegenIcon_2_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		EnergyRegenIcon_3_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		EnergyRegenIcon_4_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		EnergyRegenIcon_5_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		break;
	case 5:
		EnergyRegenIcon_1_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		EnergyRegenIcon_2_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		EnergyRegenIcon_3_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		EnergyRegenIcon_4_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		EnergyRegenIcon_5_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		break;
	}
}

void UPlayerStatusMenu::SetAssignedFlightPowerPoints(int points)
{
	switch (points)
	{
	case 1:
		FlightPowerIcon_1_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		FlightPowerIcon_2_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		FlightPowerIcon_3_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		FlightPowerIcon_4_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		FlightPowerIcon_5_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		break;
	case 2:
		FlightPowerIcon_1_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		FlightPowerIcon_2_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		FlightPowerIcon_3_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		FlightPowerIcon_4_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		FlightPowerIcon_5_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		break;
	case 3:
		FlightPowerIcon_1_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		FlightPowerIcon_2_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		FlightPowerIcon_3_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		FlightPowerIcon_4_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		FlightPowerIcon_5_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		break;
	case 4:
		FlightPowerIcon_1_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		FlightPowerIcon_2_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		FlightPowerIcon_3_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		FlightPowerIcon_4_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		FlightPowerIcon_5_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		break;
	case 5:
		FlightPowerIcon_1_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		FlightPowerIcon_2_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		FlightPowerIcon_3_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		FlightPowerIcon_4_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		FlightPowerIcon_5_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		break;
	}
}

void UPlayerStatusMenu::SetAssignedAccelerationPoints(int points)
{
	switch (points)
	{
	case 1:
		AccelerationIcon_1_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		AccelerationIcon_2_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		AccelerationIcon_3_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		AccelerationIcon_4_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		AccelerationIcon_5_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		break;
	case 2:
		AccelerationIcon_1_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		AccelerationIcon_2_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		AccelerationIcon_3_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		AccelerationIcon_4_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		AccelerationIcon_5_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		break;
	case 3:
		AccelerationIcon_1_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		AccelerationIcon_2_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		AccelerationIcon_3_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		AccelerationIcon_4_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		AccelerationIcon_5_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		break;
	case 4:
		AccelerationIcon_1_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		AccelerationIcon_2_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		AccelerationIcon_3_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		AccelerationIcon_4_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		AccelerationIcon_5_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		break;
	case 5:
		AccelerationIcon_1_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		AccelerationIcon_2_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		AccelerationIcon_3_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		AccelerationIcon_4_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		AccelerationIcon_5_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		break;
	}
}

void UPlayerStatusMenu::SetAssignedMaxGroundSpeedPoints(int points)
{
	switch (points)
	{
	case 1:
		MaxGroundSpeedIcon_1_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxGroundSpeedIcon_2_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		MaxGroundSpeedIcon_3_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		MaxGroundSpeedIcon_4_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		MaxGroundSpeedIcon_5_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		break;
	case 2:
		MaxGroundSpeedIcon_1_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxGroundSpeedIcon_2_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxGroundSpeedIcon_3_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		MaxGroundSpeedIcon_4_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		MaxGroundSpeedIcon_5_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		break;
	case 3:
		MaxGroundSpeedIcon_1_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxGroundSpeedIcon_2_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxGroundSpeedIcon_3_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxGroundSpeedIcon_4_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		MaxGroundSpeedIcon_5_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		break;
	case 4:
		MaxGroundSpeedIcon_1_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxGroundSpeedIcon_2_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxGroundSpeedIcon_3_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxGroundSpeedIcon_4_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxGroundSpeedIcon_5_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		break;
	case 5:
		MaxGroundSpeedIcon_1_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxGroundSpeedIcon_2_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxGroundSpeedIcon_3_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxGroundSpeedIcon_4_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxGroundSpeedIcon_5_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		break;
	}
}

void UPlayerStatusMenu::SetAssignedMaxFlightSpeedPoints(int points)
{
	switch (points)
	{
	case 1:
		MaxFlightSpeedIcon_1_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxFlightSpeedIcon_2_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		MaxFlightSpeedIcon_3_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		MaxFlightSpeedIcon_4_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		MaxFlightSpeedIcon_5_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		break;
	case 2:
		MaxFlightSpeedIcon_1_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxFlightSpeedIcon_2_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxFlightSpeedIcon_3_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		MaxFlightSpeedIcon_4_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		MaxFlightSpeedIcon_5_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		break;
	case 3:
		MaxFlightSpeedIcon_1_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxFlightSpeedIcon_2_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxFlightSpeedIcon_3_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxFlightSpeedIcon_4_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		MaxFlightSpeedIcon_5_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		break;
	case 4:
		MaxFlightSpeedIcon_1_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxFlightSpeedIcon_2_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxFlightSpeedIcon_3_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxFlightSpeedIcon_4_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxFlightSpeedIcon_5_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.35f));
		break;
	case 5:
		MaxFlightSpeedIcon_1_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxFlightSpeedIcon_2_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxFlightSpeedIcon_3_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxFlightSpeedIcon_4_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		MaxFlightSpeedIcon_5_image->SetColorAndOpacity(FLinearColor(1, 1, 1, 1.0f));
		break;
	}
}
