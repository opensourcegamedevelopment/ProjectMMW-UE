// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once
#include <list>
#include <iterator> 
#include "Weapon.h"
#include "CannonRifle.h"
#include "BeamRifle.h"
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
	//UFUNCTION(BlueprintCallable, Category = Stats)
	//	void SetMaxEnergyStats(float points);
	//UFUNCTION(BlueprintCallable, Category = Stats)
	//	void SetEnergyRegenStats(float points);
	//UFUNCTION(BlueprintCallable, Category = Stats)
	//	void SetMassStats(float points);
	//UFUNCTION(BlueprintCallable, Category = Stats)
	//	void SetMaxAccelerationStats(float points);
	//UFUNCTION(BlueprintCallable, Category = Stats)
	//	void SetMaxGroundSpeedStats(float points);
	//UFUNCTION(BlueprintCallable, Category = Stats)
	//	void SetMaxFlightSpeedStats(float points);
	//UFUNCTION(BlueprintCallable, Category = Stats)
	//	void SetMaxFlightPowerStats(float points); // default 0.5f

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
		float FlightPower; //Default: 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
		float MaxWalkSpeed; //Default: 600
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
		float MaxFlySpeed; //Default: 600
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
		float OverheatMaxWalkSpeed; //Default: 1000
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
		float OverheatMaxFlySpeed; //Default: 1000
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = States)
		bool IsVerticalBoost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
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
		TSubclassOf<UUserWidget> PlayerStatusMenuWidget;

	
private:
	FVector2D previousMouseLocation;
	FVector2D currentMouseLocation;
	APlayerController* playerController;
	float Location_X_Changes;
	float Location_Y_Changes;
	UWidgetTree* AimCursorHudWidgetTree;
	UWidget* AimCursorWidget;
	UWidget* AimAreaWidget;
	//UCanvasPanelSlot* AimCursorPanelSlot;
	//UCanvasPanelSlot* AimAreaPanelSlot;

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
	UUserWidget* PlayerStatusMenuWidgetInstance;
	
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

