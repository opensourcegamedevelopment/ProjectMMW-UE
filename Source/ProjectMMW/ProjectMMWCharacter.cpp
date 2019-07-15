// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ProjectMMWCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "CanvasPanelSlot.h"
#include "Blueprint/WidgetTree.h"
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

	if (GlobalSettingsRef.Succeeded() == true)
	{
		//UE_LOG(LogConfig, Log, TEXT(" Successful Getting GlobalSettingsRef"));
		GlobalSettings = GlobalSettingsRef.Object;
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

	SetDefaultStats();

	//CreateBulletPool(numOfBulletsToPool);

	SetDefaultEquipment();

	if (AimCursorHUDWidgetRef)
	{
		AimCursorHudWidget = CreateWidget<UAimCursorHUDWidget>(GetWorld(), AimCursorHUDWidgetRef);

		if (AimCursorHudWidget) {

			//AimCursorHudWidget->SetDesiredSizeInViewport(FVector2D(400, 400));
			if (!AimCursorHudWidget->GetIsVisible())
			{
				AimCursorHudWidget->AddToViewport();

				//AimCursorHudWidget->SetVisibility(ESlateVisibility::Visible);

				//UE_LOG(LogTemp, Warning, TEXT("HUD added!!"));
			}
		}
	}
	
}


void AProjectMMWCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//MyTimeline.TickTimeline(DeltaTime);

	CurrentDeltaTime += DeltaTime;

	if (CurrentDeltaTime > 0.1)
	{
		CheckEnergy();
		CheckStun(DeltaTime);
		CheckDisabledMovement(DeltaTime);
		ChecDisabledTurning(DeltaTime);

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
	//UE_LOG(LogTemp, Warning, TEXT("DeltaTime: %s"), CurrentDeltaTime);
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

	PlayerInputComponent->BindAction("SubWeaponAction", IE_Pressed, this, &AProjectMMWCharacter::ActivateSubWeapon);
	PlayerInputComponent->BindAction("SubWeaponAction", IE_Released, this, &AProjectMMWCharacter::DeActivateSubWeapon);

	PlayerInputComponent->BindAction("Weapon1", IE_Pressed, this, &AProjectMMWCharacter::SwitchToWeapon1);
	PlayerInputComponent->BindAction("Weapon2", IE_Pressed, this, &AProjectMMWCharacter::SwitchToWeapon2);
	PlayerInputComponent->BindAction("Weapon3", IE_Pressed, this, &AProjectMMWCharacter::SwitchToWeapon3);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AProjectMMWCharacter::Reload);

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
	if ((Controller != NULL) && (Value != 0.0f) && !disabledMovement)
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
	if ( (Controller != NULL) && (Value != 0.0f) && !disabledMovement)
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

		//Check shoot location
		if (AimCursorHudWidgetTree == nullptr)
		{
			AimCursorHudWidgetTree = AimCursorHudWidget->WidgetTree; //Retrieve widgetTree in the HUD
			AimAreaWidget = AimCursorHudWidgetTree->FindWidget("AimArea");
			AimCursorWidget = AimCursorHudWidgetTree->FindWidget("AimCursor");
		}
		UCanvasPanelSlot* AimCursorPanelSlot = (UCanvasPanelSlot*)AimCursorWidget->Slot;
		UCanvasPanelSlot* AimAreaPanelSlot = (UCanvasPanelSlot*)AimAreaWidget->Slot;
		FVector2D currentAimLocation = AimCursorPanelSlot->GetPosition();
		FVector2D AimAreaPosition = AimAreaPanelSlot->GetPosition();
		FVector2D AimAreaSize = AimAreaPanelSlot->GetSize();

		FVector HitLocation = FVector(0);
		FString ObjectHit = "Nothing";

		int32 ViewportSizeX, ViewportSizeY;
		playerController->GetViewportSize(ViewportSizeX, ViewportSizeY);

		bool bHit;
		FVector2D CrosshairPosition = FVector2D(ViewportSizeX / 2, ViewportSizeY / 2);
		//FVector2D CrosshairPosition = currentAimLocation;
		FHitResult HitResult;

		bHit = playerController->GetHitResultAtScreenPosition(CrosshairPosition, ECollisionChannel::ECC_WorldStatic, false, HitResult);

		if (bHit)
		{
			HitLocation = HitResult.ImpactPoint;
			ObjectHit = HitResult.GetActor()->GetName();
		}

		// Draws a red line for debugging purposes
		//DrawDebugLine(GetWorld(), HitResult.TraceStart, HitResult.TraceEnd, FColor::Red,false,1.0f,0,1);

		UE_LOG(LogTemp, Warning, TEXT("Targeting: %s  Location: %s"), *ObjectHit, *HitLocation.ToString());

		//Shoot
		if (this->GetMesh()->GetSocketByName(FName("BulletSpawnSocket")) != NULL)
		{
			const USkeletalMeshSocket* socket = GetMesh()->GetSocketByName("LeftWeaponSocket");

			FVector socketLocation;
			FQuat socketRotation;
			this->GetMesh()->GetSocketWorldLocationAndRotation(FName("BulletSpawnSocket"), socketLocation, socketRotation);

			//DrawDebugLine(GetWorld(), socketLocation, socketRotation, FColor::Red, false, 1.0f, 0, 1);

			if (!disabledTuring)
			{
				

				FRotator newRotator = UKismetMathLibrary::FindLookAtRotation(socketLocation, HitLocation);
				FVector newVector = newRotator.Vector();

				//no change to z - So that player model will look up or down while bullet will fire up or down
				FQuat newRotation = FVector(newVector.X, newVector.Y, 0).ToOrientationQuat();

				//UE_LOG(LogTemp, Warning, TEXT("newVector x: %f y: %f"), newVector.X, newVector.Y);
				 
				//UE_LOG(LogTemp, Warning, TEXT("ForwardVector x: %f y: %f"), this->GetActorRotation().Vector().X, this->GetActorRotation().Vector().Y);
				//UE_LOG(LogTemp, Warning, TEXT("ForwardVector x: %f y: %f"), socketRotation.GetForwardVector().X, socketRotation.GetForwardVector().Y);

				if (newVector.X > socketRotation.GetForwardVector().X + 0.3 || newVector.X < socketRotation.GetForwardVector().X - 0.3 ||
					newVector.Y > socketRotation.GetForwardVector().Y + 0.3 || newVector.Y < socketRotation.GetForwardVector().Y - 0.3)
				{
					//turn character at aim location
					this->SetActorRotation(newRotation, ETeleportType::TeleportPhysics);

					//disable movement for 0.2 second
					disabledMovement = true;
					disabledMovementCountDown = 0.2;
					//disabledTuring = true;
					//disabledTuringCountDown = 1;

				}

				//reset and get new aim location after character turned.
				this->GetMesh()->GetSocketWorldLocationAndRotation(FName("BulletSpawnSocket"), socketLocation, socketRotation);
				newRotator = UKismetMathLibrary::FindLookAtRotation(socketLocation, HitLocation);
				newRotation = newRotator.Quaternion();

				//shoot bullet
				EquippedWeapon_Left->Shoot(socketLocation, newRotation);
			}
			else
			{
				//shoot bullet
				EquippedWeapon_Left->Shoot(socketLocation, socketRotation);
			}
		}
	}
}
void AProjectMMWCharacter::DeActivateMainWeapon()
{

}

void AProjectMMWCharacter::ActivateSubWeapon()
{
	//ABeamRifle* BeamRifle = weapon1->GeneratedClass->GetDefaultObject<ABeamRifle>();
	if (EquippedWeapon_Right != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Character.cpp6 - Shoot!!"));

		//Check shoot location
		if (AimCursorHudWidgetTree == nullptr)
		{
			AimCursorHudWidgetTree = AimCursorHudWidget->WidgetTree; //Retrieve widgetTree in the HUD
			AimAreaWidget = AimCursorHudWidgetTree->FindWidget("AimArea");
			AimCursorWidget = AimCursorHudWidgetTree->FindWidget("AimCursor");
		}
		UCanvasPanelSlot* AimCursorPanelSlot = (UCanvasPanelSlot*)AimCursorWidget->Slot;
		UCanvasPanelSlot* AimAreaPanelSlot = (UCanvasPanelSlot*)AimAreaWidget->Slot;
		FVector2D currentAimLocation = AimCursorPanelSlot->GetPosition();
		FVector2D AimAreaPosition = AimAreaPanelSlot->GetPosition();
		FVector2D AimAreaSize = AimAreaPanelSlot->GetSize();

		FVector HitLocation = FVector(0);
		FString ObjectHit = "Nothing";

		int32 ViewportSizeX, ViewportSizeY;
		playerController->GetViewportSize(ViewportSizeX, ViewportSizeY);

		bool bHit;
		FVector2D CrosshairPosition = FVector2D(ViewportSizeX / 2, ViewportSizeY / 2);
		FHitResult HitResult;

		bHit = playerController->GetHitResultAtScreenPosition(CrosshairPosition, ECollisionChannel::ECC_WorldStatic, false, HitResult);

		if (bHit)
		{
			HitLocation = HitResult.ImpactPoint;
			ObjectHit = HitResult.GetActor()->GetName();
		}

		//Shoot
		if (this->GetMesh()->GetSocketByName(FName("BulletSpawnSocket")) != NULL)
		{
			const USkeletalMeshSocket* socket = GetMesh()->GetSocketByName("RightWeaponSocket");

			FVector socketLocation;
			FQuat socketRotation;
			this->GetMesh()->GetSocketWorldLocationAndRotation(FName("BulletSpawnSocket"), socketLocation, socketRotation);

			if (!disabledTuring)
			{
				FRotator newRotator = UKismetMathLibrary::FindLookAtRotation(socketLocation, HitLocation);
				FVector newVector = newRotator.Vector();

				//no change to z - So that player model will look up or down while bullet will fire up or down
				FQuat newRotation = FVector(newVector.X, newVector.Y, 0).ToOrientationQuat();

				if (newVector.X > socketRotation.GetForwardVector().X + 0.3 || newVector.X < socketRotation.GetForwardVector().X - 0.3 ||
					newVector.Y > socketRotation.GetForwardVector().Y + 0.3 || newVector.Y < socketRotation.GetForwardVector().Y - 0.3)
				{
					//turn character at aim location
					this->SetActorRotation(newRotation, ETeleportType::TeleportPhysics);

					//disable movement for 0.2 second
					disabledMovement = true;
					disabledMovementCountDown = 0.2;
					//disabledTuring = true;
					//disabledTuringCountDown = 1;

				}

				//reset and get new aim location after character turned.
				this->GetMesh()->GetSocketWorldLocationAndRotation(FName("BulletSpawnSocket"), socketLocation, socketRotation);
				newRotator = UKismetMathLibrary::FindLookAtRotation(socketLocation, HitLocation);
				newRotation = newRotator.Quaternion();

				//shoot bullet
				EquippedWeapon_Right->Shoot(socketLocation, newRotation);
			}
			else
			{
				//shoot bullet
				EquippedWeapon_Right->Shoot(socketLocation, socketRotation);
			}
		}
	}
}
void AProjectMMWCharacter::DeActivateSubWeapon()
{

}

void AProjectMMWCharacter::SwitchToWeapon1()
{
	UE_LOG(LogTemp, Warning, TEXT("SwitchToWeapon1()"));

	if (EquippedWeapon_Left != Weapon1_Left)
	{
		if (EquippedWeapon_Left != nullptr) EquippedWeapon_Left->SetActive(false);
		EquippedWeapon_Left = Weapon1_Left;
		if (EquippedWeapon_Left != nullptr) EquippedWeapon_Left->SetActive(true);
	}

	if (EquippedWeapon_Right != Weapon1_Right)
	{
		if (EquippedWeapon_Right != nullptr) EquippedWeapon_Right->SetActive(false);
		EquippedWeapon_Right = Weapon1_Right;
		if (EquippedWeapon_Right != nullptr) EquippedWeapon_Right->SetActive(true);
	}
}

void AProjectMMWCharacter::SwitchToWeapon2()
{
	UE_LOG(LogTemp, Warning, TEXT("SwitchToWeapon2()"));

	if (EquippedWeapon_Left != Weapon2_Left)
	{
		if (EquippedWeapon_Left != nullptr) EquippedWeapon_Left->SetActive(false);
		EquippedWeapon_Left = Weapon2_Left;
		if (EquippedWeapon_Left != nullptr) EquippedWeapon_Left->SetActive(true);
	}

	if (EquippedWeapon_Right != Weapon2_Right)
	{
		if (EquippedWeapon_Right != nullptr) EquippedWeapon_Right->SetActive(false);
		EquippedWeapon_Right = Weapon2_Right;
		if (EquippedWeapon_Right != nullptr) EquippedWeapon_Right->SetActive(true);
	}
}

void AProjectMMWCharacter::SwitchToWeapon3()
{
	UE_LOG(LogTemp, Warning, TEXT("SwitchToWeapon3()"));

	if (EquippedWeapon_Left != Weapon3_Left)
	{
		if (EquippedWeapon_Left != nullptr) EquippedWeapon_Left->SetActive(false);
		EquippedWeapon_Left = Weapon3_Left;
		if (EquippedWeapon_Left != nullptr) EquippedWeapon_Left->SetActive(true);
	}

	if (EquippedWeapon_Right != Weapon3_Right)
	{
		if (EquippedWeapon_Right != nullptr) EquippedWeapon_Right->SetActive(false);
		EquippedWeapon_Right = Weapon3_Right;
		if (EquippedWeapon_Right != nullptr) EquippedWeapon_Right->SetActive(true);
	}
}


void AProjectMMWCharacter::Reload()
{
	EquippedWeapon_Left->Reload();
}

FText AProjectMMWCharacter::GetWeaponLeft_CurrentClipSize()
{
	if (EquippedWeapon_Left != nullptr)
	{
		return FText::AsNumber(EquippedWeapon_Left->GetCurrentClipSize());
	}
	else
	{
		return FText::FromString("0");
	}
}

FText AProjectMMWCharacter::GetWeaponLeft_CurrentTotalAmmo()
{
	if (EquippedWeapon_Left != nullptr)
	{
		return FText::AsNumber(EquippedWeapon_Left->GetCurrentTotalAmmo());
	}
	else
	{
		return FText::FromString("0");
	}
}

FText AProjectMMWCharacter::GetWeaponRight_CurrentClipSize()
{
	if (EquippedWeapon_Right != nullptr)
	{
		return FText::AsNumber(EquippedWeapon_Right->GetCurrentClipSize());
	}
	else
	{
		return FText::FromString("0");
	}
}

FText AProjectMMWCharacter::GetWeaponRight_CurrentTotalAmmo()
{
	if (EquippedWeapon_Right != nullptr)
	{
		return FText::AsNumber(EquippedWeapon_Right->GetCurrentTotalAmmo());
	}
	else
	{
		return FText::FromString("0");
	}
}

float AProjectMMWCharacter::GetWeaponLeft_ReloadPercentage()
{
	if (EquippedWeapon_Left != nullptr)
	{
		return EquippedWeapon_Left->GetReloadPercentage();
	}
	else
	{
		return 0;
	}
}

float AProjectMMWCharacter::GetWeaponRight_ReloadPercentage()
{
	if (EquippedWeapon_Right != nullptr)
	{
		return EquippedWeapon_Right->GetReloadPercentage();
	}
	else
	{
		return 0;
	}
}

//void AProjectMMWCharacter::CreateBulletPool(int howMany) {
//	for (int i = 0; i < howMany; i++) {
//		ABullet* tempGo = GetWorld()->SpawnActor<ABullet>(ABullet::StaticClass(), FVector(-9999999, -9999999, -9999999), FRotator::ZeroRotator);
//		bulletPool.push_back(tempGo);
//	}
//}
#pragma endregion

#pragma region Character Status Functions
void AProjectMMWCharacter::SetDefaultStats()
{
	IsOverheat = false;
	IsVerticalBoost = false;
	IsBoosting = false;
	CurrentHp = MaxHp;
	HealthPercentage = 1.0f;
	CurrentEnergy = MaxEnergy;
	EnergyPercentage = 1.0f;
	FlightPower = 0.5f;
	playerController = Cast<APlayerController>(GetController());
	Location_X_Changes = 0;
	Location_Y_Changes = 0;
	if (playerController != nullptr)
	{
		playerController->GetMousePosition(previousMouseLocation.X, previousMouseLocation.Y);
		playerController->GetMousePosition(currentMouseLocation.X, currentMouseLocation.Y);
	}
}

void AProjectMMWCharacter::SetDefaultEquipment()
{
	//Get EquiableWeapons from globalSettings
	TSubclassOf<AGlobalSettings> GlobalSettingsActorClass;
	GlobalSettingsActorClass = GlobalSettings->GeneratedClass;
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

	//look for Beam Rifle
	TSubclassOf<AWeapon>* BeamRiflePtr = EquipableWeapons.Find("BeamRifle");
	TSubclassOf<AWeapon> BeamRifle = BeamRiflePtr->Get();
	Weapon1_Left = BeamRifle->GetDefaultObject<ABeamRifle>();


	TSubclassOf<AWeapon>* CannonRiflePtr = EquipableWeapons.Find("CannonRifle");
	TSubclassOf<AWeapon> CannonRifle = CannonRiflePtr->Get();
	Weapon2_Left = CannonRifle->GetDefaultObject<ACannonRifle>();


	//Equip BeamRifle to current Equipped Weapon_Left
	FVector actorLocation = GetActorLocation();
	FVector actorForwardVector = GetActorForwardVector() * 200;
	FVector NewLocation = actorForwardVector + actorLocation;


	if (this->GetMesh()->GetSocketByName(FName("LeftWeaponSocket")) != NULL)
	{
		UE_LOG(LogTemp, Log, TEXT("Character.cpp - Socket Found!!"));
		const USkeletalMeshSocket* LeftWeaponSocket = GetMesh()->GetSocketByName("LeftWeaponSocket");
		const USkeletalMeshSocket* RightWeaponSocket = GetMesh()->GetSocketByName("RightWeaponSocket");

		//Spawn Weapons
		Weapon1_Left = GetWorld()->SpawnActor<ABeamRifle>(BeamRiflePtr->Get(), GetActorLocation(), FRotator::ZeroRotator);
		LeftWeaponSocket->AttachActor(Weapon1_Left, GetMesh());
		Weapon1_Left->SetActive(false);

		Weapon2_Left = GetWorld()->SpawnActor<ACannonRifle>(CannonRiflePtr->Get(), GetActorLocation(), FRotator::ZeroRotator);
		LeftWeaponSocket->AttachActor(Weapon2_Left, GetMesh());
		Weapon2_Left->SetActive(false);

		Weapon3_Left = GetWorld()->SpawnActor<ABeamRifle>(BeamRiflePtr->Get(), GetActorLocation(), FRotator::ZeroRotator);
		LeftWeaponSocket->AttachActor(Weapon3_Left, GetMesh());
		Weapon3_Left->SetActive(false);

		Weapon3_Right = GetWorld()->SpawnActor<ACannonRifle>(CannonRiflePtr->Get(), GetActorLocation(), FRotator::ZeroRotator);
		RightWeaponSocket->AttachActor(Weapon3_Right, GetMesh());
		Weapon3_Right->SetActive(false);

		EquippedWeapon_Left = Weapon1_Left;
		EquippedWeapon_Left->SetActive(true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Character.cpp - Socket Not Found!!"));
	}
}

#pragma region health and energy
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
		CurrentEnergy -= EnergyUsage;
	}
	else
	{
		if (IsOverheat)
		{
			RegenEnergy(OverheatEnergyRegen);
		}
		else
		{
			RegenEnergy(EnergyRegen);
		}
	}

	EnergyPercentage = CurrentEnergy / MaxEnergy;
}

void AProjectMMWCharacter::RegenEnergy(float regenRate)
{
	if (!IsBoosting && CurrentEnergy < MaxEnergy)
	{
		CurrentEnergy += regenRate;
	}
}
void AProjectMMWCharacter::CheckStun(float DeltaTime)
{
	if (stunned)
	{
		stunnedCountDown -= DeltaTime;
		
		if (stunnedCountDown <= 0)
		{
			stunned = false;
		}
	}
}
void AProjectMMWCharacter::CheckDisabledMovement(float DeltaTime)
{
	if (disabledMovement)
	{
		disabledMovementCountDown -= DeltaTime;

		if (disabledMovementCountDown <= 0)
		{
			disabledMovement = false;
		}
	}
}
void AProjectMMWCharacter::ChecDisabledTurning(float DeltaTime)
{
	if (disabledTuring)
	{
		disabledTuringCountDown -= DeltaTime;

		if (disabledTuringCountDown <= 0)
		{
			disabledTuring = false;
		}
	}
}
#pragma endregion
#pragma endregion