// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <list>
#include <iterator> 
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
//#include "Engine/Classes/Engine//SkeletalMesh.h"
#include "Weapon.generated.h"

UCLASS()
class PROJECTMMW_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	virtual void CreateBulletPool(int howMany);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//list <ABullet*> bulletPool;
	const int numOfBulletsToPool = 25;

private:
	TArray<UStaticMeshComponent*> components;
	bool isActive;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	bool IsActive();
	virtual void Shoot();
	virtual void Shoot(FVector location, FQuat rotation);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* staticMeshComponent;

	void SetActive(bool isActive);
	void DeActivate();

};
