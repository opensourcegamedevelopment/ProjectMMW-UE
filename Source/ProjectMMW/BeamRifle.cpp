// Fill out your copyright notice in the Description page of Project Settings.


#include "BeamRifle.h"

// Sets default values
ABeamRifle::ABeamRifle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//find and add BeamRifile Bullet
	ConstructorHelpers::FObjectFinder<UBlueprint> BulletRef(TEXT("Blueprint'/Game/Blueprints/BP_DummyBUllet.BP_DummyBUllet'"));
	if (BulletRef.Succeeded() == true)
	{
		//UE_LOG(LogTemp, Log, TEXT("Success Getting Beam Rifle"));
		//weapon1 = BulletRef.Object;

		//BeamRifle = weapon1->GeneratedClass->GetDefaultObject<ABeamRifle>();
		BulletToSpawn = (UClass*)BulletRef.Object->GeneratedClass;
	}
	else
	{
		//UE_LOG(LogTemp, Log, TEXT("Failed Getting Beam Rifle"));
	}
}

// Called when the game starts or when spawned
void ABeamRifle::BeginPlay()
{
	Super::BeginPlay();
	//staticMeshComponent->SetStaticMesh(sta);

	SetRootComponent(staticMeshComponent);
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
		//ABullet* bullet = World->SpawnActor<ABullet>(ABullet::StaticClass(), newActorLocation, FRotator::ZeroRotator);
		//bullet->SpawnBullet(float(10), float(10), FTransform(newActorLocation), AmmoStaticMesh);
		ABullet* bullet = World->SpawnActor<ABullet>(BulletToSpawn, newActorLocation, FRotator::ZeroRotator);
		bullet->SpawnBullet(float(600), float(10), FTransform(newActorLocation));
	}

}

