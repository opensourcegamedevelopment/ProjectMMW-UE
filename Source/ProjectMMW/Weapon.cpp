// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	staticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	GetComponents<UStaticMeshComponent>(components);

	staticMeshComponent->SetupAttachment(RootComponent);

	//DeActivate();
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::CreateBulletPool(int howMany) {
	for (int i = 0; i < howMany; i++) {
		ABullet* tempGo = GetWorld()->SpawnActor<ABullet>(ABullet::StaticClass(), FVector(-9999999, -9999999, -9999999), FRotator::ZeroRotator);
		//bulletPool.push_back(tempGo);
	}
}

void AWeapon::SetActive(bool isActive)
{
	this->isActive = isActive;
	SetActorHiddenInGame(!isActive);
}

bool AWeapon::IsActive()
{
	return isActive;
}

void AWeapon::Shoot()
{						
	//FTransform transform = FTransform(GetActorLocation());

	//ABullet* bullet = GetWorld()->SpawnActor<ABullet>(ABullet::StaticClass(), FVector(-9999999, -9999999, -9999999), FRotator::ZeroRotator);

	//ABullet* bullet;
	//bullet->SpawnBullet(bullet->getBulletSpeed,bullet->getBulletDamage, transform);

	UE_LOG(LogTemp, Log, TEXT("Weapon.cpp - Shoot!!"));
}

void AWeapon::Shoot(AActor *actor)
{
	UE_LOG(LogTemp, Log, TEXT("Weapon.cpp - Shoot!! - To Be Implemented in Parent Class"));
}

void AWeapon::DeActivate()
{
	// actor hidden in game instead of destruction.
	SetActive(false);
}
