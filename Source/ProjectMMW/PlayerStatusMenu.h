// @CopyRight 2018 Open Source Game Dev

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Image.h"
#include "PlayerStatusMenu.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMMW_API UPlayerStatusMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPlayerStatusMenu(const FObjectInitializer& ObjectInitializer);
	void InitialiseData();

	void SetAssignedMaxHealthPoints(int points);
	void SetAssignedMaxEnergyPoints(int points);
	void SetAssignedEnergyRegenPoints(int points);
	void SetAssignedFlightPowerPoints(int points);
	void SetAssignedAccelerationPoints(int points);
	void SetAssignedMaxGroundSpeedPoints(int points);
	void SetAssignedMaxFlightSpeedPoints(int points);
protected:

private:

	UWidgetTree* tree;
	UImage* MaxHealthIcon_1_image;
	UImage* MaxHealthIcon_2_image;
	UImage* MaxHealthIcon_3_image;
	UImage* MaxHealthIcon_4_image;
	UImage* MaxHealthIcon_5_image;

	UImage* MaxEnergyIcon_1_image;
	UImage* MaxEnergyIcon_2_image;
	UImage* MaxEnergyIcon_3_image;
	UImage* MaxEnergyIcon_4_image;
	UImage* MaxEnergyIcon_5_image;

	UImage* EnergyRegenIcon_1_image;
	UImage* EnergyRegenIcon_2_image;
	UImage* EnergyRegenIcon_3_image;
	UImage* EnergyRegenIcon_4_image;
	UImage* EnergyRegenIcon_5_image;

	UImage* FlightPowerIcon_1_image;
	UImage* FlightPowerIcon_2_image;
	UImage* FlightPowerIcon_3_image;
	UImage* FlightPowerIcon_4_image;
	UImage* FlightPowerIcon_5_image;

	UImage* AccelerationIcon_1_image;
	UImage* AccelerationIcon_2_image;
	UImage* AccelerationIcon_3_image;
	UImage* AccelerationIcon_4_image;
	UImage* AccelerationIcon_5_image;

	UImage* MaxGroundSpeedIcon_1_image;
	UImage* MaxGroundSpeedIcon_2_image;
	UImage* MaxGroundSpeedIcon_3_image;
	UImage* MaxGroundSpeedIcon_4_image;
	UImage* MaxGroundSpeedIcon_5_image;

	UImage* MaxFlightSpeedIcon_1_image;
	UImage* MaxFlightSpeedIcon_2_image;
	UImage* MaxFlightSpeedIcon_3_image;
	UImage* MaxFlightSpeedIcon_4_image;
	UImage* MaxFlightSpeedIcon_5_image;
};
