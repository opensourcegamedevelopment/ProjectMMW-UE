// @CopyRight 2018 Open Source Game Dev


#include "ShootingWeapon.h"

// Sets default values
AShootingWeapon::AShootingWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	/*staticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	GetComponents<UStaticMeshComponent>(components);

	staticMeshComponent->SetupAttachment(RootComponent);*/

	CurrentClipSize = ClipSize;
	CurrentTotalAmmo = MaxAmmo;

	//DeActivate();
}

// Called when the game starts or when spawned
void AShootingWeapon::BeginPlay()
{
	Super::BeginPlay();
	CurrentClipSize = ClipSize;
	CurrentTotalAmmo = MaxAmmo;
}

// Called every frame
void AShootingWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//attack interval settings
	if (CurrentAttackInterval <= attackInterval)
	{
		CurrentAttackInterval += DeltaTime;
	}

	//reload settings
	if (Reloading && CurrentReloadTime >= ReloadSpeed)
	{
		int reloadAmount = ClipSize - CurrentClipSize;
		//only reload up to the maximum amount of ammo left
		if (CurrentTotalAmmo < reloadAmount)
		{
			CurrentClipSize += CurrentTotalAmmo;
			CurrentTotalAmmo -= CurrentTotalAmmo;
		}
		else
		{
			CurrentClipSize = ClipSize;
			CurrentTotalAmmo -= reloadAmount;
		}
		reloadPercentage = 0;
		Reloading = false;
	}
	else if (Reloading && isActive)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Weapon.cpp - Reloading: %d"), CurrentReloadTime);
		CurrentReloadTime += DeltaTime;
		reloadPercentage = 1 - (CurrentReloadTime / ReloadSpeed);
	}
}

#pragma region getters
int AShootingWeapon::GetCurrentClipSize()
{
	return CurrentClipSize;
}
int AShootingWeapon::GetCurrentTotalAmmo()
{
	return CurrentTotalAmmo;
}
float AShootingWeapon::GetReloadPercentage()
{
	return reloadPercentage;
}
#pragma endregion

void AShootingWeapon::CreateBulletPool(int howMany) {
	for (int i = 0; i < howMany; i++) {
		ABullet* tempGo = GetWorld()->SpawnActor<ABullet>(ABullet::StaticClass(), FVector(-9999999, -9999999, -9999999), FRotator::ZeroRotator);
		//bulletPool.push_back(tempGo);
	}
}

void AShootingWeapon::Shoot()
{
	UE_LOG(LogTemp, Log, TEXT("Weapon.cpp - Shoot!!"));
}

void AShootingWeapon::Shoot(FVector location, FQuat rotation)
{
	//AmmoCheck
	if (Reloading || CurrentAttackInterval < attackInterval)
	{
		//Cannot shoot if:
		//1. Reloading
		//2. Within the set interval from last attack
	}
	else if (CurrentClipSize <= 0)
	{
		Reload();
	}
	else
	{
		//shoot
		UWorld* const World = GetWorld();
		if (World)
		{
			ABullet* bullet = World->SpawnActor<ABullet>(BulletToSpawn, location, rotation.Rotator());
			bullet->SpawnBullet(bullet->getBulletSpeed(), bullet->getBulletDamage(), FTransform(rotation, location, FVector::OneVector));
		}
		CurrentClipSize -= AmmoUsePerShot; //use 1 ammo at a time.
		CurrentAttackInterval = 0;
	}
}

void AShootingWeapon::Reload()
{
	UE_LOG(LogTemp, Log, TEXT("Reload"));
	//check if need to reload
	if (!Reloading && CurrentClipSize < ClipSize)
	{
		Reloading = true;
		CurrentReloadTime = 0;
	}
}