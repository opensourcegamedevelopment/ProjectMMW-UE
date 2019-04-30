// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <list>
#include <iterator> 
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.h"
#include "Components/BoxComponent.h"
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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Shoot();
	virtual void Shoot(AActor *actor);

protected:
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* staticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMesh* staticMesh;*/

};
