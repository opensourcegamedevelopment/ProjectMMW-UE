// @CopyRight 2018 Open Source Game Dev

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GlobalSettings.generated.h"

UCLASS()
class PROJECTMMW_API AGlobalSettings : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGlobalSettings();

	TMap<FString, TSubclassOf<class AShootingWeapon>> GetEquipableShootingWeapons();
	TMap<FString, TSubclassOf<class AMeleeWeapon>> GetEquipableMeleeWeapons();
	TMap<FString, TSubclassOf<class AShield>> GetEquipableShields();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Equipments)
	TArray<TSubclassOf<class ABeamRifle>> EquipableWeapons;*/

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Equipments)
		TMap<FString, TSubclassOf<class AShootingWeapon>> EquipableShootingWeapons;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Equipments)
		TMap<FString, TSubclassOf<class AMeleeWeapon>> EquipableMeleeWeapons;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Equipments)
		TMap<FString, TSubclassOf<class AShield>> EquipableShields;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
