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

void ACannonRifle::Shoot(AActor* actor)
{
	UE_LOG(LogTemp, Log, TEXT("ACannonRifle.cpp - Shoot!!"));
	FTransform transform = FTransform(GetActorLocation());

	UE_LOG(LogTemp, Log, TEXT("ACannonRifle.cpp - transform %s"), *transform.ToString());

	FVector actorLocation = actor->GetActorLocation();

	FVector actorForwardVector = actor->GetActorForwardVector() * 10;

	FVector newActorLocation = actorForwardVector + actorLocation;

	UE_LOG(LogTemp, Log, TEXT("ACannonRifle.cpp - actorLocation %s"), *actorLocation.ToString());

	UE_LOG(LogTemp, Log, TEXT("ACannonRifle.cpp - actorForwardVector %s"), *actorForwardVector.ToString());

	UWorld* const World = actor->GetWorld();
	if (World)
	{
		ABullet* bullet = World->SpawnActor<ABullet>(BulletToSpawn, newActorLocation, FRotator::ZeroRotator);
		bullet->SpawnBullet(float(600), float(10), FTransform(newActorLocation));
	}

}

