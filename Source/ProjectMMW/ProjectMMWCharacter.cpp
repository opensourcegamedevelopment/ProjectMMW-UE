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
}

void AProjectMMWCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	MaxHp = 1000;
	CurrentHp = 1000;
	HealthPercentage = 1.0f;
	MaxEnergy = 1000;
	CurrentEnergy = 1000;
	EnergyPercentage = 1.0f;
	FlightPower = 10.0f;
}

void AProjectMMWCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*MyTimeline.TickTimeline(DeltaTime);

	CurrentDeltaTime += DeltaTime;

	if (IsBoosting && CurrentDeltaTime > 1)
	{
		UE_LOG(LogTemp, Log, TEXT("DeltaTime: %s"), CurrentDeltaTime);
		CurrentDeltaTime -= DeltaTime;
		CheckEnergy();
	}

	if (GetCharacterMovement()->IsFlying() == true && IsVerticalBoost)
	{
		AddMovementInput(GetActorUpVector(), FlightPower);
	}*/
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

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AProjectMMWCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AProjectMMWCharacter::LookUpAtRate);
}

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

#pragma region Character Action Methods

void AProjectMMWCharacter::MoveForward(float Value)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "debug msg");

	//UE_LOG(LogTemp, Log, TEXT("%s"), (IsBoosting ? TEXT("True") : TEXT("False")));
	IsBoosting = true;
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
	if (!IsBoosting || !IsOverheat)
	{
		AProjectMMWCharacter::Jump();
	}
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
	}
	else
	{
		IsBoosting = true;
		if( GetCharacterMovement()->IsFlying() == false )  
		{  
			GetCharacterMovement()->SetMovementMode(MOVE_Flying);  
			if (CurrentEnergy <= 0)
			{
				GetCharacterMovement()->SetMovementMode(MOVE_Falling);
			}
		}  
	}
}

void AProjectMMWCharacter::DeActivateBoost()
{
	if( GetCharacterMovement()->IsFlying() == true )  
	{  
		GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	}  
}

#pragma endregion

#pragma region Character Status Methods

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
	if (IsBoosting)
	{
		CurrentEnergy--;
	}
	else
	{
		RegenEnergy();
	}
}

void AProjectMMWCharacter::RegenEnergy()
{
	if (!IsBoosting && CurrentEnergy < MaxEnergy)
	{
		CurrentEnergy += 5;
	}
}
#pragma endregion