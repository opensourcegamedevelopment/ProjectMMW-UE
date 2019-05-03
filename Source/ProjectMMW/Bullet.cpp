// Fill out your copyright notice in the Description page of Project Settings.

#include "Bullet.h"
#include "Engine/StaticMesh.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Damageable.h"


// Sets default values
ABullet::ABullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	projectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement(Component)"));
	staticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	GetComponents<UStaticMeshComponent>(components);


	staticMeshComponent->SetupAttachment(RootComponent);

	projectileMovementComponent->Velocity = FVector(0, 0, 0);
	projectileMovementComponent->ProjectileGravityScale = 0.0f;
	DeActivate();		

	staticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnOverlapBegin);
	staticMeshComponent->OnComponentEndOverlap.AddDynamic(this, &ABullet::OnOverlapEnd);
	staticMeshComponent->SetGenerateOverlapEvents(true);
	staticMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	//////////////////////////////////
	// for testing
	//FTransform* transform = new FTransform(FQuat(0, 0, 0, 0), FVector(-1000, 0, 400), FVector(1, 1, 1));
	//SpawnBullet(100, 100000000, *transform);

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
	staticMeshComponent->SetGenerateOverlapEvents(isActive);
	SetActorHiddenInGame(!isActive);
}

void ABullet::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("OverlapBegin"));
	
	IDamageable* theInterface = Cast<IDamageable>( OtherActor );

	if (theInterface) {
		theInterface->DamageObject(this->bulletDamage);
		//theInterface->Execute_DamageObject(this->bulletDamage);
	}
	
	DeActivate();
}

void ABullet::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("OverlapEnd"));

}

bool ABullet::IsActive()
{
	return isActive;
}

void ABullet::DeActivate()
{
	// actor hidden in game instead of destruction.
	// allows for pooling
	projectileMovementComponent->Velocity = FVector(0, 0, 0);

	SetActive(false);
}


void ABullet::SpawnBullet(float bulletSpeed, float bulletDamage, FTransform transform)
{
	this->SetActorLocation(transform.GetLocation(), false);
	this->SetActorRotation(transform.GetRotation());
	this->bulletSpeed = bulletSpeed;
	this->bulletDamage = bulletDamage;
	projectileMovementComponent->Velocity = GetActorForwardVector() * bulletSpeed;
	
	SetActive(true);
}

void ABullet::SpawnBullet(float bulletSpeed, float bulletDamage, FTransform transform, UStaticMesh* newMesh)
{
	staticMeshComponent->SetStaticMesh(newMesh);
	SpawnBullet(bulletSpeed, bulletDamage, transform);
}

void ABullet::SpawnBullet(float bulletSpeed, float bulletDamage, FTransform transform, UStaticMesh* newMesh, float lifespan)
{
	SetLifeSpan(lifespan);
	SpawnBullet(bulletSpeed, bulletDamage, transform, newMesh);
}