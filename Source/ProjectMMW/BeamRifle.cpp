// Fill out your copyright notice in the Description page of Project Settings.


#include "BeamRifle.h"

// Sets default values
ABeamRifle::ABeamRifle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//find and add BeamRifile Bullet
	/*ConstructorHelpers::FObjectFinder<UBlueprint> BulletRef(TEXT("Blueprint'/Game/Blueprints/BP_DummyBUllet.BP_DummyBUllet'"));
	if (BulletRef.Succeeded() == true)
	{
		BulletToSpawn = (UClass*)BulletRef.Object->GeneratedClass;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed Getting Beam Rifle"));
	}*/
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