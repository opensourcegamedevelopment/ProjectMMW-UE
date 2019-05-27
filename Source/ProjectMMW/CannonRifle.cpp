// @CopyRight 2018 Open Source Game Dev


#include "CannonRifle.h"

// Sets default values
ACannonRifle::ACannonRifle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACannonRifle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACannonRifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACannonRifle::Shoot(FVector location, FQuat rotation)
{
	UWorld* const World = GetWorld();
	if (World)
	{
		ABullet* bullet = World->SpawnActor<ABullet>(BulletToSpawn, location, rotation.Rotator());
		bullet->SpawnBullet(float(600), float(10), FTransform(rotation, location, FVector::OneVector));
	}
}

