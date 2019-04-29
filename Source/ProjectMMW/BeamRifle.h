// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "BeamRifle.generated.h"

UCLASS()
class PROJECTMMW_API ABeamRifle : public AWeapon
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

	//virtual void Shoot() override;

	void Shoot(AActor *actor);

protected:
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* staticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMesh* staticMesh;*/

};
