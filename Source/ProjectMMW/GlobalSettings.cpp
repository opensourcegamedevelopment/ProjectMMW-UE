// @CopyRight 2018 Open Source Game Dev


#include "GlobalSettings.h"

// Sets default values
AGlobalSettings::AGlobalSettings()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGlobalSettings::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGlobalSettings::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

#pragma region getters/setters

#pragma region getters
TMap<FString, TSubclassOf<class AWeapon>> AGlobalSettings::GetEquipableWeapons()
{
	return EquipableWeapons;
}
#pragma endregion

#pragma endregion

