// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once
#include <list>
#include <iterator> 
#include "Weapon.h"
#include "CannonRifle.h"
#include "BeamRifle.h"
#include "PlayerStatusMenu.h"
#include "PlayerEquipmentMenu.h"
#include "PlayerEquipmentSelectMenu.h"
#include "AimCursorHUDWidget.h"
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

	UPROPERTY(EditAnywhere, Category = "CustomWidgets")
		TSubclassOf<UAimCursorHUDWidget> AimCursorHUDWidgetRef;

	UPROPERTY(VisibleAnywhere, Category = "CustomWidgets")
		UAimCursorHUDWidget* AimCursorHudWidget;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Stats)
		float MaxHp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Stats)
		float CurrentHp;
	float PreviousHp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Stats)
		float HealthPercentage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Stats)
		float MaxEnergy; //Default: 1000
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Stats)
		float CurrentEnergy;
	float PreviousEnergy;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Stats)
		float EnergyPercentage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
		float EnergyUsage; //Default: 2
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
		float EnergyRegen; //Default: 1
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
		float OverheatEnergyRegen; //Default: 0.5

	UFUNCTION(BlueprintPure, Category=Stats)
		FText GetCurrentHealth();
	UFUNCTION(BlueprintPure, Category = Stats)
		FText GetMaxHealth();
	UFUNCTION(BlueprintPure, Category = Stats)
		float GetHealthPercentage();

	UFUNCTION(BlueprintPure, Category=Stats)
		FText GetCurrentEnergy();
	UFUNCTION(BlueprintPure, Category = Stats)
		FText GetMaxEnergy();
	UFUNCTION(BlueprintPure, Category=Stats)
		FText GetHealthIntText();
	UFUNCTION(BlueprintPure, Category=Stats)
		FText GetEnergyIntText();
	UFUNCTION(BlueprintCallable, Category=Stats)
		void UpdateHp(float HealthChange);
	UFUNCTION(BlueprintCallable, Category=Stats)
		void UpdateEnergy(float EnergyChange);

	UFUNCTION(BlueprintCallable, Category = Stats)
		void SetMaxHpStats(int points);
	UFUNCTION(BlueprintCallable, Category = Stats)
		void SetMaxEnergyStats(int points);
	UFUNCTION(BlueprintCallable, Category = Stats)
		void SetEnergyRegenStats(int points);
	/*UFUNCTION(BlueprintCallable, Category = Stats)
		void SetMassStats(int points);*/
	UFUNCTION(BlueprintCallable, Category = Stats)
		void SetAccelerationStats(int points);
	UFUNCTION(BlueprintCallable, Category = Stats)
		void SetMaxGroundSpeedStats(int points);
	UFUNCTION(BlueprintCallable, Category = Stats)
		void SetMaxFlightSpeedStats(int points);
	UFUNCTION(BlueprintCallable, Category = Stats)
		void SetMaxFlightPowerStats(int points);

	UFUNCTION(BlueprintCallable, Category = Stats)
		void SetMechRotateOnGround(bool enable);
	UFUNCTION(BlueprintCallable, Category = Stats)
		void SetEnableAimRange(bool enable);
	
	UFUNCTION(BlueprintPure, Category = Stats)
		FText GetWeaponLeft_CurrentClipSize();
	UFUNCTION(BlueprintPure, Category = Stats)
		FText GetWeaponRight_CurrentClipSize();
	UFUNCTION(BlueprintPure, Category = Stats)
		FText GetWeaponLeft_CurrentTotalAmmo();
	UFUNCTION(BlueprintPure, Category = Stats)
		FText GetWeaponRight_CurrentTotalAmmo();
	UFUNCTION(BlueprintPure, Category = Stats)
		float GetWeaponLeft_ReloadPercentage();
	UFUNCTION(BlueprintPure, Category = Stats)
		float GetWeaponRight_ReloadPercentage();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stats)
		float FlightPower; //Default: 0.5f; This is the vertical flight height capability
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stats)
		float BoostPower; //This will modify the base speed x BoostPower
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stats)
		float AccelerationPower; 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stats)
		float MaxGroundSpeed; //Default: 1000
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stats)
		float MaxFlightSpeed; //Default: 1000
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stats)
	//	float OverheatMaxWalkSpeed; //Default: 500
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stats)
	//	float OverheatMaxFlySpeed; //Default: 500
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = States)
		bool IsVerticalBoost;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stats)
		float acceptableTargeRange = 0.6; //Default: 0.3

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipments)
		AWeapon* EquippedWeapon_Left;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipments)
		AWeapon* EquippedWeapon_Right;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets")
		TSubclassOf<UPlayerStatusMenu> PlayerStatusMenuWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets")
		TSubclassOf<UPlayerEquipmentMenu> PlayerEquipmentMenuWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets")
		TSubclassOf<UPlayerEquipmentSelectMenu> PlayerEquipmentSelectMenuWidget;
	
private:
	FVector2D previousMouseLocation;
	FVector2D currentMouseLocation;
	APlayerController* playerController;
	float Location_X_Changes;
	float Location_Y_Changes;
	UWidgetTree* AimCursorHudWidgetTree;
	UWidget* AimCursorWidget;
	UWidget* AimAreaWidget;

	bool inMenu;
	bool stunned;
	bool disabledMovement;
	bool disabledTuring;
	float stunnedCountDown; // in seconds
	float disabledMovementCountDown; // in seconds
	float disabledTuringCountDown; // in seconds

	bool bMechRotateOnGround = false;
	bool bEnableAimRange = false;

protected:
	
	//Character Status Functions
	void SetDefaultStats();
	void SetDefaultEquipment();
	void CheckStats();
	void CheckEnergy();
	void RegenEnergy(float regenRate);
	void CheckStun(float DeltaTime);
	void CheckDisabledMovement(float DeltaTime);
	void CheckDisabledTurning(float DeltaTime);

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
	void ActivateSubWeapon();
	void DeActivateSubWeapon();
	void Reload();
	void SwitchToWeapon1();
	void SwitchToWeapon2();
	void SwitchToWeapon3();

	void ToggleInventory();

	UPlayerStatusMenu* StatusMenuInstance;
	UPlayerEquipmentMenu* EquipmentMenuInstance;
	UPlayerEquipmentSelectMenu* EquipmentSelectMenuInstance;

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

