// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShootingWeapon.h"
#include "UObject/ConstructorHelpers.h"
#include "BeamRifle.generated.h"

UCLASS()
class PROJECTMMW_API ABeamRifle : public AShootingWeapon
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABeamRifle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ammo)
		UStaticMeshComponent* AmmoStaticMeshComponent;
		*/
	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ammo)
		UStaticMesh* AmmoStaticMesh;*/

	/*UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Settings)
		TSubclassOf<class ABullet> BulletToSpawn;*/

private:
	

};
