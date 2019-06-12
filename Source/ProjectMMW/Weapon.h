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

	//properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		int ClipSize;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		int MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		int AmmoUsePerShot = 1; //default to 1
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Settings)
		float ReloadSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Settings)
		float CurrentReloadTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Settings)
	bool Reloading;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Settings)
		float attackInterval;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Settings)
		float CurrentAttackInterval;

	int GetCurrentClipSize();
	int GetCurrentTotalAmmo();
	float GetReloadPercentage();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//list <ABullet*> bulletPool;
	const int numOfBulletsToPool = 25;


private:
	TArray<UStaticMeshComponent*> components;
	bool isActive;
	
	int CurrentClipSize;
	int CurrentTotalAmmo;
	float reloadPercentage = 0;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	bool IsActive();
	virtual void Shoot();
	virtual void Shoot(FVector location, FQuat rotation);
	void Reload();
	void SetActive(bool isActive);
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* staticMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Settings)
		TSubclassOf<class ABullet> BulletToSpawn;

	void DeActivate();

};
