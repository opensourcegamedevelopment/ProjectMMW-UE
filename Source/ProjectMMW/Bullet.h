// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class PROJECTMMW_API ABullet : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	TArray<UStaticMeshComponent*> components;
	UStaticMesh* newMesh;
	FTimerHandle lifeSpanTimer;
	float lifespan = 3.0f;
	bool isActive;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Settings)
		UStaticMeshComponent* staticMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Settings)
		UProjectileMovementComponent* projectileMovementComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		float bulletSpeed = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		float bulletDamage = 100.0f;

public:
	virtual void SetLifeSpan(float InLifespan) override;
	bool IsActive();
	float getLifeSpan();
	float getBulletSpeed();
	float getBulletDamage();

	//void SpawnBullet();
	//void SpawnBullet(float bulletSpeed);
	void SpawnBullet(float bulletSpeed, float bulletDamage, FTransform transform);
	void SpawnBullet(float bulletSpeed, float bulletDamage, FTransform transform, UStaticMesh* newMesh);
	void SpawnBullet(float bulletSpeed, float bulletDamage, FTransform transform, UStaticMesh* newMesh, float lifespan);

protected:
	void DeActivate();
	void SetActive(bool isActive);

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};