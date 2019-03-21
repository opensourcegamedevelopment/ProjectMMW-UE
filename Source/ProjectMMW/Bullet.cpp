// Fill out your copyright notice in the Description page of Project Settings.

#include "Bullet.h"
#include "Engine/StaticMesh.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Actor.h"
#include "UObject/ConstructorHelpers.h"


// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	projectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement(Component)"));
	staticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	GetComponents<UStaticMeshComponent>(components);

	projectileMovementComponent->Velocity = FVector(0, 0, 0);
	SetActive(false);

	staticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnOverlapBegin);
	staticMeshComponent->OnComponentEndOverlap.AddDynamic(this, &ABullet::OnOverlapEnd);
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::SetLifeSpan(float InLifespan) {

	lifespan = InLifespan;
	GetWorldTimerManager().SetTimer(lifeSpanTimer, this, &ABullet::DeActivate, lifespan, false);
}

void ABullet::SetActive(bool isActive)
{	
	this->isActive = isActive;
	SetActorHiddenInGame(!isActive);
}

void ABullet::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	SetActive(false);
}

void ABullet::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{

}

bool ABullet::IsActive()
{
	return isActive;
}

void ABullet::DeActivate()
{
	SetActive(false);
}


void ABullet::SpawnBullet(float bulletSpeed, float bulletDamage)
{
	this->bulletSpeed = bulletSpeed;
	this->bulletDamage = bulletDamage;
	projectileMovementComponent->Velocity = GetActorForwardVector() * bulletSpeed;
	SetActive(true);
}

void ABullet::SpawnBullet(float bulletSpeed, float bulletDamage, UStaticMesh* newMesh)
{	
	staticMeshComponent->SetStaticMesh(newMesh);
	SpawnBullet(bulletSpeed, bulletDamage);
}

void ABullet::SpawnBullet(float bulletSpeed, float bulletDamage, UStaticMesh* newMesh, float lifespan)
{	
	SetLifeSpan(lifespan);
	SpawnBullet(bulletSpeed, bulletDamage, newMesh);
}

