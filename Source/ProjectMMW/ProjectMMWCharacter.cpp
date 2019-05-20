// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ProjectMMWCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"

//////////////////////////////////////////////////////////////////////////
// AProjectMMWCharacter

AProjectMMWCharacter::AProjectMMWCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	//find GlobalSettings Blueprint 
	ConstructorHelpers::FObjectFinder<UBlueprint> GlobalSettingsRef(TEXT("Blueprint'/Game/Blueprints/BP_GlobalSettings.BP_GlobalSettings'"));

	/*BeamRifle = weapon1->GeneratedClass->GetDefaultObject<ABeamRifle>();
	BeamRifleToSpawn = (UClass*)BeamRifleRef.Object->GeneratedClass;*/

	if (GlobalSettingsRef.Succeeded() == true)
	{
		UE_LOG(LogConfig, Log, TEXT(" Successful Getting GlobalSettingsRef"));
		GlobalSettings = GlobalSettingsRef.Object;

		/*BeamRifle = weapon1->GeneratedClass->GetDefaultObject<ABeamRifle>();
		BeamRifleToSpawn = (UClass*)BeamRifleRef.Object->GeneratedClass;*/
	}
	else
	{
		UE_LOG(LogConfig, Error, TEXT("Failed Getting GlobalSettingsRef"));
	}
}

void AProjectMMWCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	IsOverheat = false;
	IsVerticalBoost = false;
	IsBoosting = false;
	MaxHp = 1000;
	CurrentHp = 1000;
	HealthPercentage = 1.0f;
	MaxEnergy = 1;
	CurrentEnergy = 1;
	EnergyPercentage = 1.0f;
	FlightPower = 0.5f;

	//CreateBulletPool(numOfBulletsToPool);

	/*TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWeapon::StaticClass(), FoundActors);*/


	TSubclassOf<AGlobalSettings> GlobalSettingsActorClass = GlobalSettings->GeneratedClass;
	TMap<FString, TSubclassOf<AWeapon>> EquipableWeapons = GlobalSettingsActorClass.GetDefaultObject()->GetEquipableWeapons();

	/*int32 Count = EquipableWeapons.Num();
	UE_LOG(LogTemp, Warning, TEXT("GlobalSettings itemsTotal: %d"), Count);*/
	for (auto& Elem : EquipableWeapons)
	{
		/*Elem.Key,
			* Elem.Value*/
		UE_LOG(LogTemp, Warning, TEXT("GlobalSettingsItems"));
		UE_LOG(LogTemp, Warning, TEXT("GlobalSettings: %s"), *Elem.Key);

	}

	TSubclassOf<AWeapon>* BeamRiflePtr = EquipableWeapons.Find("BeamRifle");

	FVector actorLocation = GetActorLocation();
	FVector actorForwardVector = GetActorForwardVector() * 200;
	FVector NewLocation = actorForwardVector + actorLocation;
	EquippedWeapon_Left = GetWorld()->SpawnActor<ABeamRifle>(BeamRiflePtr->Get(), NewLocation, FRotator::ZeroRotator);


	if (this->GetMesh()->GetSocketByName(FName("LeftWeaponSocket")) != NULL)
	{
		UE_LOG(LogTemp, Log, TEXT("Character.cpp - Socket Found!!"));
		FName fnWeaponSocket = TEXT("LeftWeaponSocket");

		const USkeletalMeshSocket* socket = GetMesh()->GetSocketByName("LeftWeaponSocket");
		socket->AttachActor(EquippedWeapon_Left, GetMesh());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Character.cpp - Socket Not Found!!"));
	}
}


void AProjectMMWCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//MyTimeline.TickTimeline(DeltaTime);

	CurrentDeltaTime += DeltaTime;

	if (CurrentDeltaTime > 1)
	{
		CheckEnergy();

		if (GetCharacterMovement()->IsFlying() == true)
		{
			if (IsVerticalBoost)
			{
				AddMovementInput(GetActorUpVector(), FlightPower);
			}

			bUseControllerRotationYaw = true;
			//set player character face forward
			APlayerCameraManager *camManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
			FVector camForward = camManager->GetCameraRotation().Vector();

			FRotator rotator = FRotator(GetActorRotation().Pitch, GetActorRotation().Yaw, camForward.Z);

			SetActorRotation(rotator, ETeleportType::TeleportPhysics);
		}
		else
		{
			bUseControllerRotationYaw = false;
		}
		CurrentDeltaTime -= DeltaTime;
	}
	//UE_LOG(LogTemp, Log, TEXT("DeltaTime: %s"), CurrentDeltaTime);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AProjectMMWCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AProjectMMWCharacter::JumpKeyAction); //Jump
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AProjectMMWCharacter::JumpKeyReleasedAction); //StopJumping

	PlayerInputComponent->BindAxis("MoveForward", this, &AProjectMMWCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AProjectMMWCharacter::MoveRight);

	PlayerInputComponent->BindAction("Boost", IE_Pressed, this, &AProjectMMWCharacter::ActivateBoost);
	PlayerInputComponent->BindAction("Boost", IE_Released, this, &AProjectMMWCharacter::DeActivateBoost);

	PlayerInputComponent->BindAction("MainWeaponAction", IE_Pressed, this, &AProjectMMWCharacter::ActivateMainWeapon);
	PlayerInputComponent->BindAction("MainWeaponAction", IE_Released, this, &AProjectMMWCharacter::DeActivateMainWeapon);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AProjectMMWCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AProjectMMWCharacter::LookUpAtRate);
}

#pragma region Character Action Functions

#pragma region Character Action Movements
void AProjectMMWCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}
void AProjectMMWCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AProjectMMWCharacter::MoveForward(float Value)
{
	//UE_LOG(LogTemp, Log, TEXT("%s"),  (IsBoosting ? TEXT("True") : TEXT("False")));
	CheckStats();
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}
void AProjectMMWCharacter::MoveRight(float Value)
{
	CheckStats();
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AProjectMMWCharacter::JumpKeyAction()
{
	if (IsBoosting && !IsOverheat)
	{
		if (GetCharacterMovement()->IsFlying() == false)
		{
			GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		}
		if (IsVerticalBoost == false) IsVerticalBoost = true;
	}
	else 
	{
		AProjectMMWCharacter::Jump();
	}
	characterRotateCheck();
}
void AProjectMMWCharacter::JumpKeyReleasedAction()
{
	if (!IsBoosting || IsOverheat)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Falling);
		AProjectMMWCharacter::StopJumping();
	}
	IsVerticalBoost = false;
}

void AProjectMMWCharacter::ActivateBoost()
{
	if (IsOverheat)
	{
		if( GetCharacterMovement()->IsFlying() == true )  
		{  
			GetCharacterMovement()->SetMovementMode(MOVE_Falling);
		}
		IsBoosting = false;
	}
	else if (CurrentEnergy > 0)
	{
		IsBoosting = true;
		if( GetCharacterMovement()->IsFlying() == false )  
		{  
			GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		}  
		
	}
	characterRotateCheck();
}

void AProjectMMWCharacter::DeActivateBoost()
{
	if( GetCharacterMovement()->IsFlying() == true )  
	{  
		GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	}  
	IsBoosting = false;
	characterRotateCheck();
}

void AProjectMMWCharacter::characterRotateCheck()
{
	if (GetCharacterMovement()->IsFlying() == true)
	{
		if (IsVerticalBoost)
		{
			AddMovementInput(GetActorUpVector(), FlightPower);
		}

		bUseControllerRotationYaw = true;
		//set player character face forward
		APlayerCameraManager *camManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
		FVector camForward = camManager->GetCameraRotation().Vector();

		FRotator rotator = FRotator(GetActorRotation().Pitch, GetActorRotation().Yaw, camForward.Z);

		SetActorRotation(rotator, ETeleportType::TeleportPhysics);
	}
	else
	{
		bUseControllerRotationYaw = false;
	}
}

#pragma endregion

void AProjectMMWCharacter::ActivateMainWeapon()
{
	//ABeamRifle* BeamRifle = weapon1->GeneratedClass->GetDefaultObject<ABeamRifle>();
	if (EquippedWeapon_Left != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Character.cpp6 - Shoot!!"));
		EquippedWeapon_Left->Shoot(this);
	}
}

void AProjectMMWCharacter::DeActivateMainWeapon()
{

}

//void AProjectMMWCharacter::CreateBulletPool(int howMany) {
//	for (int i = 0; i < howMany; i++) {
//		ABullet* tempGo = GetWorld()->SpawnActor<ABullet>(ABullet::StaticClass(), FVector(-9999999, -9999999, -9999999), FRotator::ZeroRotator);
//		bulletPool.push_back(tempGo);
//	}
//}

//void AProjectMMWCharacter::FireWeapon() {
//	float bulletSpeed = 100.0f;								//
//	float bulletDamage = 100.0f;							// those are test variables, delete when implented
//	FTransform transform = FTransform(GetActorLocation());	// 
//	//
//	// Check Bullet.h
//	// the intented function to use is
//	// void SpawnBullet(float bulletSpeed, float bulletDamage, FTransform transform, UStaticMesh* newMesh, float lifespan);
//	// variables are to be drawn from equiped weapon
//	//
//	bulletPool.front()->SpawnBullet(bulletSpeed, bulletDamage, transform);
//	bulletPool.push_back(bulletPool.front());
//	bulletPool.pop_front();
//}

#pragma endregion

#pragma region Character Status Functions

void AProjectMMWCharacter::CheckStats()
{
	UCharacterMovementComponent *MovementPtr =  Cast<UCharacterMovementComponent>(GetCharacterMovement());
	if (IsOverheat)
	{
		if (MovementPtr->MaxWalkSpeed != 600)
		{
			MovementPtr->MaxWalkSpeed = 600;
		}
		if (MovementPtr->MaxFlySpeed != 600)
		{
			MovementPtr->MaxFlySpeed = 600;
		}
	}
	else
	{
		if (MovementPtr->MaxWalkSpeed != 1000)
		{
			MovementPtr->MaxWalkSpeed = 1000;
		}
		if (MovementPtr->MaxFlySpeed != 1000)
		{
			MovementPtr->MaxFlySpeed = 1000;
		}
	}
}

float AProjectMMWCharacter::GetHealth()
{
	return CurrentHp;
}

float AProjectMMWCharacter::GetEnergy()
{
	return CurrentEnergy;
}

FText AProjectMMWCharacter::GetHealthIntText()
{
	int32 HP = FMath::RoundHalfFromZero(HealthPercentage * 100);
	FString HPS = FString::FromInt(HP);
	FString HealthHUD = HPS + FString(TEXT("%"));
	FText HPText = FText::FromString(HealthHUD);
	return HPText;
}

FText AProjectMMWCharacter::GetEnergyIntText()
{
	int32 Energy = FMath::RoundHalfFromZero(EnergyPercentage * 100);
	FString EnergyS = FString::FromInt(Energy);
	FString EnergyHUD = EnergyS + FString(TEXT("%"));
	FText EnergyText = FText::FromString(EnergyHUD);
	return EnergyText;
}

void AProjectMMWCharacter::UpdateHp(float HealthChange)
{
	CurrentHp += HealthChange;
	CurrentHp = FMath::Clamp(CurrentHp, 0.0f, MaxHp);
	PreviousHp = HealthPercentage;
	HealthPercentage = CurrentHp / MaxHp;
}

void AProjectMMWCharacter::UpdateEnergy(float MagicChange)
{
	CurrentEnergy += MagicChange;
	CurrentEnergy = FMath::Clamp(CurrentEnergy, 0.0f, MaxEnergy);
	PreviousEnergy = EnergyPercentage;
	EnergyPercentage = CurrentEnergy / MaxEnergy;
}

void AProjectMMWCharacter::CheckEnergy()
{
	if (CurrentEnergy <= 0)
	{
		IsOverheat = true;
		GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	}
	else if (CurrentEnergy >= MaxEnergy)
	{
		IsOverheat = false;
	}

	if (IsBoosting)
	{
		CurrentEnergy -= 0.003;
	}
	else
	{
		if (IsOverheat)
		{
			RegenEnergy(0.004);
		}
		else
		{
			RegenEnergy(0.01);
		}
	}
}

void AProjectMMWCharacter::RegenEnergy(float regenRate)
{
	if (!IsBoosting && CurrentEnergy < MaxEnergy)
	{
		CurrentEnergy += regenRate;
	}
}
#pragma endregion