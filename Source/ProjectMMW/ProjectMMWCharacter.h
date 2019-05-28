// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once
#include <list>
#include <iterator> 
#include "Weapon.h"
#include "CannonRifle.h"
#include "BeamRifle.h"
#include "GlobalSettings.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/Character.h"
#include "ProjectMMWCharacter.generated.h"

using namespace std;

UCLASS(config=Game)
class AProjectMMWCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	AProjectMMWCharacter();

	FTimeline MyTimeline;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=States)
	bool IsOverheat;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=States)
	bool IsBoosting;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Stats)
	float MaxHp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Stats)
	float CurrentHp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Stats)
	float PreviousHp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Stats)
	float HealthPercentage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Stats)
	float MaxEnergy;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Stats)
	float CurrentEnergy;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stats)
	float PreviousEnergy;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Stats)
	float EnergyPercentage;

	UFUNCTION(BlueprintPure, Category=Stats)
	float GetHealth();
	UFUNCTION(BlueprintPure, Category=Stats)
	float GetEnergy();
	UFUNCTION(BlueprintPure, Category=Stats)
	FText GetHealthIntText();
	UFUNCTION(BlueprintPure, Category=Stats)
	FText GetEnergyIntText();
	UFUNCTION(BlueprintCallable, Category=Stats)
	void UpdateHp(float HealthChange);
	UFUNCTION(BlueprintCallable, Category=Stats)
	void UpdateEnergy(float EnergyChange);

	UFUNCTION(BlueprintPure, Category = Stats)
		FText GetWeaponLeftCurrentClipSize();
	UFUNCTION(BlueprintPure, Category = Stats)
		FText GetWeaponLeftCurrentTotalAmmo();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stats)
	float FlightPower;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = States)
	bool IsVerticalBoost;

	//use to determine when to check stats
	float CurrentDeltaTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Settings)
	UBlueprint* GlobalSettings;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipments)
	AWeapon* Weapon1_Left;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipments)
	AWeapon* Weapon1_Right;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipments)
	AWeapon* Weapon2_Left;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipments)
	AWeapon* Weapon2_Right;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipments)
	AWeapon* Weapon3_Left;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipments)
	AWeapon* Weapon3_Right;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipments)
	AWeapon* Weapon4_Left;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipments)
	AWeapon* Weapon4_Right;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Settings)
	AWeapon* EquippedWeapon_Left;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Settings)
	AWeapon* EquippedWeapon_Right;

private:

protected:
	
	//Character Status Functions
	void CheckStats();
	void CheckEnergy();
	void RegenEnergy(float regenRate);

	// Character Movment Action Functions
	void ActivateBoost();
	void DeActivateBoost();
	void characterRotateCheck();
	void JumpKeyAction();
	void JumpKeyReleasedAction();
	void MoveForward(float Value);
	void MoveRight(float Value);

	// Character Movment Action Functions
	void ActivateMainWeapon();
	void DeActivateMainWeapon();
	void Reload();
	void SwitchToWeapon1();
	void SwitchToWeapon2();

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

