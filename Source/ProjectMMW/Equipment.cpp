// @CopyRight 2018 Open Source Game Dev


#include "Equipment.h"

// Sets default values
AEquipment::AEquipment()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEquipment::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEquipment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEquipment::SetActive(bool isActive)
{
	this->isActive = isActive;
	SetActorHiddenInGame(!isActive);
}

bool AEquipment::IsActive()
{
	return isActive;
}

void AEquipment::DeActivate()
{
	// actor hidden in game instead of destruction.
	SetActive(false);
}