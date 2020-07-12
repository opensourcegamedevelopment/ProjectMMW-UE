// @CopyRight 2018 Open Source Game Dev

#pragma once

#include "CoreMinimal.h"
#include "Bullet.h"
#include "Weapon.h"
#include "ShootingWeapon.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMMW_API AShootingWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AShootingWeapon();

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
	int CurrentClipSize;
	int CurrentTotalAmmo;
	float reloadPercentage = 0;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Shoot();
	virtual void Shoot(FVector location, FQuat rotation);
	void Reload();
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Settings)
		TSubclassOf<class ABullet> BulletToSpawn;
};
