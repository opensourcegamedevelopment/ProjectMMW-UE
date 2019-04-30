// Fill out your copyright notice in the Description page of Project Settings.


#include "BeamRifle.h"

// Sets default values
ABeamRifle::ABeamRifle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABeamRifle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABeamRifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABeamRifle::Shoot(AActor *actor)
{
	UE_LOG(LogTemp, Log, TEXT("BeamRifle.cpp - Shoot!!"));
	FTransform transform = FTransform(GetActorLocation());

	UE_LOG(LogTemp, Log, TEXT("BeamRifle.cpp - transform %s"), *transform.ToString());

	FVector actorLocation = actor->GetActorLocation();

	FVector actorForwardVector = actor->GetActorForwardVector() * 10;

	FVector newActorLocation = actorForwardVector + actorLocation;

	UE_LOG(LogTemp, Log, TEXT("BeamRifle.cpp - actorLocation %s"), *actorLocation.ToString());

	UE_LOG(LogTemp, Log, TEXT("BeamRifle.cpp - actorForwardVector %s"), *actorForwardVector.ToString());

	UWorld* const World = actor->GetWorld();
	if (World)
	{
		ABullet* bullet = World->SpawnActor<ABullet>(ABullet::StaticClass(), newActorLocation, FRotator::ZeroRotator);
		bullet->SpawnBullet(float(10), float(10), FTransform(newActorLocation), AmmoStaticMesh);
	}

	//GetWorld()->SpawnActor<ABullet>(ABullet::StaticClass(), FVector(100, 100, 100), FRotator::ZeroRotator);

	//ABullet* bullet = GetWorld()->SpawnActor<ABullet>(ABullet::StaticClass(), FVector(100, 100, 100), FRotator::ZeroRotator);

	//bullet->SpawnBullet(float(100), float(100), transform);
}

