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
#include "image.h"
#include "GlobalSettings.h"


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
	//ConstructorHelpers::FObjectFinder<UBlueprint> GlobalSettingsRef(TEXT("Blueprint'/Game/Blueprints/BP_GlobalSettings.BP_GlobalSettings'"));

	//if (GlobalSettingsRef.Succeeded() == true)
	//{
	//	//UE_LOG(LogConfig, Log, TEXT(" Successful Getting GlobalSettingsRef"));
	//	GlobalSettings = GlobalSettingsRef.Object;
	//}
	//else
	//{
	//	UE_LOG(LogConfig, Error, TEXT("Failed Getting GlobalSettingsRef"));
	//}
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

	if (AimCursorHudWidgetTree == nullptr)
	{
		AimCursorHudWidgetTree = AimCursorHudWidget->WidgetTree; //Retrieve widgetTree in the HUD
		AimAreaWidget = AimCursorHudWidgetTree->FindWidget("AimArea");
		AimCursorWidget = AimCursorHudWidgetTree->FindWidget("AimCursor");
	}

	if (StatusMenuInstance == nullptr)
	{
		StatusMenuInstance = (UPlayerStatusMenu*) CreateWidget(GetWorld(), PlayerStatusMenuWidget);
		StatusMenuInstance->InitialiseData();
	}

	if (EquipmentMenuInstance == nullptr)
	{
		EquipmentMenuInstance = (UPlayerEquipmentMenu*) CreateWidget(GetWorld(), PlayerEquipmentMenuWidget);
		EquipmentMenuInstance->InitialiseData();
	}

	if (EquipmentSelectMenuInstance == nullptr)
	{
		EquipmentSelectMenuInstance = (UPlayerEquipmentSelectMenu*)CreateWidget(GetWorld(), PlayerEquipmentSelectMenuWidget);
		EquipmentSelectMenuInstance->InitialiseData();
	}

	if (StatusMenuInstance != nullptr && EquipmentMenuInstance != nullptr && EquipmentSelectMenuInstance != nullptr)
	{
		StatusMenuInstance->EquipmentMenuInstance = EquipmentMenuInstance;
		EquipmentMenuInstance->StatusMenuInstance = StatusMenuInstance;
		EquipmentMenuInstance->EquipmentSelectMenuInstance = EquipmentSelectMenuInstance;
		EquipmentSelectMenuInstance->EquipmentMenuInstance = EquipmentMenuInstance;
		//EquipmentSelectMenuInstance->GlobalSettings = GlobalSettings;
		EquipmentSelectMenuInstance->EquipableShootingWeapons = EquipableShootingWeapons;

		/*StatusMenuInstance->AProjectMMWCharacterInstance = (AProjectMMWCharacter*) this->GetClass();
		EquipmentMenuInstance->AProjectMMWCharacterInstance = (AProjectMMWCharacter*)this->GetClass();
		EquipmentSelectMenuInstance->AProjectMMWCharacterInstance = (AProjectMMWCharacter*)this->GetClass();*/
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
		CheckDisabledTurning(DeltaTime);

		if (GetCharacterMovement()->IsFlying() == true)
		{
			if (IsVerticalBoost)
			{
				AddMovementInput(GetActorUpVector(), FlightPower);
			}

			bUseControllerRotationYaw = true;
			//set player character face forward
			APlayerCameraManager* camManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
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

	if (bEnableAimRange)
	{
		if (playerController != nullptr)
		{
			playerController->GetMousePosition(currentMouseLocation.X, currentMouseLocation.Y);
			//UE_LOG(LogTemp, Warning, TEXT("Mouse Position -> %f %f"), LocationX, LocationY);


			if (previousMouseLocation.X > 0)
			{
				Location_X_Changes = previousMouseLocation.X - currentMouseLocation.X;
			}

			if (previousMouseLocation.Y > 0)
			{
				Location_Y_Changes = previousMouseLocation.Y - currentMouseLocation.Y;
			}
			//UE_LOG(LogTemp, Warning, TEXT("previousMouseLocation Position -> %f %f"), previousMouseLocation.X, previousMouseLocation.Y);
			//UE_LOG(LogTemp, Warning, TEXT("currentMouseLocation Position -> %f %f"), currentMouseLocation.X, currentMouseLocation.Y);

			//if (AimCursorHudWidgetTree == nullptr)
			//{
			//	AimCursorHudWidgetTree = AimCursorHudWidget->WidgetTree; //Retrieve widgetTree in the HUD
			//	AimAreaWidget = AimCursorHudWidgetTree->FindWidget("AimArea");
			//	AimCursorWidget = AimCursorHudWidgetTree->FindWidget("AimCursor");
			//}

			UCanvasPanelSlot* AimCursorPanelSlot = (UCanvasPanelSlot*)AimCursorWidget->Slot;
			UCanvasPanelSlot* AimAreaPanelSlot = (UCanvasPanelSlot*)AimAreaWidget->Slot;
			FVector2D currentAimLocation = AimCursorPanelSlot->GetPosition();
			FVector2D AimAreaPosition = AimAreaPanelSlot->GetPosition();
			FVector2D AimAreaSize = AimAreaPanelSlot->GetSize();

			float newPositionX = currentAimLocation.X - Location_X_Changes;
			float newPositionY = currentAimLocation.Y - Location_Y_Changes;

			if (newPositionX > (AimAreaPosition.X + AimAreaSize.X) / 2)
			{
				newPositionX = AimAreaPosition.X + AimAreaSize.X / 2;
			}
			else if (newPositionX < AimAreaPosition.X - AimAreaSize.X / 2)
			{
				newPositionX = AimAreaPosition.X - AimAreaSize.X / 2;
			}
			else if (newPositionY > (AimAreaPosition.Y + AimAreaSize.Y) / 2)
			{
				newPositionY = AimAreaPosition.Y + AimAreaSize.Y / 2;
			}
			else if (newPositionY < AimAreaPosition.Y - AimAreaSize.Y / 2)
			{
				newPositionY = AimAreaPosition.Y - AimAreaSize.Y / 2;
			}
			else
			{
				//this->bUseControllerRotationYaw = false;
			}

			AimCursorPanelSlot->SetPosition(FVector2D(newPositionX, newPositionY));
			previousMouseLocation = currentMouseLocation;
		}
	}
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

	PlayerInputComponent->BindAction("MainWeaponAction", IE_Pressed, this, &AProjectMMWCharacter::ActivateLeftWeapon);
	PlayerInputComponent->BindAction("MainWeaponAction", IE_Released, this, &AProjectMMWCharacter::DeActivateLeftWeapon);

	PlayerInputComponent->BindAction("SubWeaponAction", IE_Pressed, this, &AProjectMMWCharacter::ActivateRightWeapon);
	PlayerInputComponent->BindAction("SubWeaponAction", IE_Released, this, &AProjectMMWCharacter::DeActivateRightWeapon);

	PlayerInputComponent->BindAction("Weapon1", IE_Pressed, this, &AProjectMMWCharacter::SwitchToWeapon1);
	PlayerInputComponent->BindAction("Weapon2", IE_Pressed, this, &AProjectMMWCharacter::SwitchToWeapon2);
	PlayerInputComponent->BindAction("Weapon3", IE_Pressed, this, &AProjectMMWCharacter::SwitchToWeapon3);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AProjectMMWCharacter::Reload);

	PlayerInputComponent->BindAction("ToggleInventory", IE_Pressed, this, &AProjectMMWCharacter::ToggleInventory);

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

		if (!bMechRotateOnGround)
		{
			characterRotateCheck();
		}
	}
}
void AProjectMMWCharacter::MoveRight(float Value)
{
	CheckStats();
	if ((Controller != NULL) && (Value != 0.0f) && !disabledMovement)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);

		if (!bMechRotateOnGround)
		{
			characterRotateCheck();
		}
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

	if (!bMechRotateOnGround)
	{
		characterRotateCheck();
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
		if (GetCharacterMovement()->IsFlying() == true)
		{
			GetCharacterMovement()->SetMovementMode(MOVE_Falling);
		}
		IsBoosting = false;
	}
	else if (CurrentEnergy > 0)
	{
		IsBoosting = true;
		if (GetCharacterMovement()->IsFlying() == false)
		{
			GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		}

	}
	characterRotateCheck();
}

void AProjectMMWCharacter::DeActivateBoost()
{
	if (GetCharacterMovement()->IsFlying() == true)
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
		APlayerCameraManager* camManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
		FVector camForward = camManager->GetCameraRotation().Vector();

		FRotator rotator = FRotator(GetActorRotation().Pitch, GetActorRotation().Yaw, camForward.Z);

		SetActorRotation(rotator, ETeleportType::TeleportPhysics);
	}
	else
	{
		if (bMechRotateOnGround)
		{
			bUseControllerRotationYaw = false;
		}
		else
		{
			//set player character face forward
			bUseControllerRotationYaw = true;
			APlayerCameraManager* camManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
			FVector camForward = camManager->GetCameraRotation().Vector();

			FRotator rotator = FRotator(GetActorRotation().Pitch, GetActorRotation().Yaw, camForward.Z);

			SetActorRotation(rotator, ETeleportType::TeleportPhysics);
		}
	}
}

#pragma endregion
#pragma endregion
#pragma region weapons
void AProjectMMWCharacter::ActivateLeftWeapon()
{
	if (EquippedWeapon_Left != nullptr && !inMenu)
	{
		AShootingWeapon* shootingWeaponPtr = dynamic_cast<AShootingWeapon*>(EquippedWeapon_Left);
		if (shootingWeaponPtr)
		{

			//Check Aim Panel ------------------------------------------------------------------------------------

			UCanvasPanelSlot* AimCursorPanelSlot = (UCanvasPanelSlot*)AimCursorWidget->Slot;
			UCanvasPanelSlot* AimAreaPanelSlot = (UCanvasPanelSlot*)AimAreaWidget->Slot;
			FVector2D currentAimLocation = AimCursorPanelSlot->GetPosition();

			FVector HitLocation = FVector(0);
			FString ObjectHit = "Nothing";

			int32 ViewportSizeX, ViewportSizeY;
			playerController->GetViewportSize(ViewportSizeX, ViewportSizeY);

			bool bHit;
			FVector2D CrosshairPosition;
			if (bEnableAimRange)
			{
				// aim cursor at cursor location <--- NOTE (ZEROKIRI): position not correct atm.
				CrosshairPosition = currentAimLocation;
			}
			else
			{
				// aim cursor at center of the screen
				CrosshairPosition = FVector2D(ViewportSizeX / 2, ViewportSizeY / 2);
			}
			FHitResult HitResult;

			bHit = playerController->GetHitResultAtScreenPosition(CrosshairPosition, ECollisionChannel::ECC_WorldStatic, false, HitResult);

			if (bHit)
			{
				HitLocation = HitResult.ImpactPoint;
				ObjectHit = HitResult.GetActor()->GetName();
			}

			if (this->GetMesh()->GetSocketByName(FName("BulletSpawnSocket")) != NULL)
			{
				FVector socketLocation;
				FQuat socketRotation;
				this->GetMesh()->GetSocketWorldLocationAndRotation(FName("BulletSpawnSocket"), socketLocation, socketRotation);

				//DrawDebugLine(GetWorld(), socketLocation, HitLocation, FColor::Red, false, 5.0f, 0, 2);

				if (!disabledTuring)
				{
					FRotator newRotator = UKismetMathLibrary::FindLookAtRotation(socketLocation, HitLocation);
					FVector newVector = newRotator.Vector();

					//no change to z - So that player model will look up or down while bullet will fire up or down
					FQuat newRotation = FVector(newVector.X, newVector.Y, 0).ToOrientationQuat();

					if (newVector.X > socketRotation.GetForwardVector().X + acceptableTargeRange || newVector.X < socketRotation.GetForwardVector().X - acceptableTargeRange ||
						newVector.Y > socketRotation.GetForwardVector().Y + acceptableTargeRange || newVector.Y < socketRotation.GetForwardVector().Y - acceptableTargeRange)
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
					TArray<UStaticMeshComponent*> weaponMeshComponents;

					shootingWeaponPtr->GetComponents<UStaticMeshComponent>(weaponMeshComponents);
					for (UStaticMeshComponent* weaponMeshComponent : weaponMeshComponents)
					{
						if (weaponMeshComponent->GetName() == "BeamRifleMesh")
						{
							TArray<UStaticMeshSocket*> sockets = weaponMeshComponent->GetStaticMesh()->Sockets;

							for (UStaticMeshSocket* socket : sockets)
							{
								if (socket->SocketName.ToString() == FString("BulletSpawnSocket"))
								{
									socketLocation = shootingWeaponPtr->GetActorLocation() +
										UKismetMathLibrary::Multiply_VectorFloat(shootingWeaponPtr->GetActorForwardVector(), socket->RelativeLocation.X);

								}
							}

						}
					}

					//DrawDebugLine(GetWorld(), this->GetActorLocation(), socketLocation, FColor::Green, false, 5.0f, 0, 2);

					newRotator = UKismetMathLibrary::FindLookAtRotation(socketLocation, HitLocation);
					newRotation = newRotator.Quaternion();

					//shoot bullet
					shootingWeaponPtr->Shoot(socketLocation, newRotation);
				}
				else
				{
					TArray<UStaticMeshComponent*> weaponMeshComponents;

					shootingWeaponPtr->GetComponents<UStaticMeshComponent>(weaponMeshComponents);
					for (UStaticMeshComponent* weaponMeshComponent : weaponMeshComponents)
					{
						if (weaponMeshComponent->GetName() == "BeamRifleMesh")
						{
							TArray<UStaticMeshSocket*> sockets = weaponMeshComponent->GetStaticMesh()->Sockets;

							for (UStaticMeshSocket* socket : sockets)
							{
								if (socket->SocketName.ToString() == FString("BulletSpawnSocket"))
								{
									socketLocation = shootingWeaponPtr->GetActorLocation() +
										UKismetMathLibrary::Multiply_VectorFloat(shootingWeaponPtr->GetActorForwardVector(), socket->RelativeLocation.X);

								}
							}

						}
					}

					this->GetMesh()->GetSocketWorldLocationAndRotation(FName("BulletSpawnSocket"), socketLocation, socketRotation);
					FRotator newRotator = UKismetMathLibrary::FindLookAtRotation(socketLocation, HitLocation);
					FQuat newRotation = newRotator.Quaternion();

					//shoot bullet
					shootingWeaponPtr->Shoot(socketLocation, newRotation);
				}
			}
		}
	}
}
void AProjectMMWCharacter::DeActivateLeftWeapon()
{

}

void AProjectMMWCharacter::ActivateRightWeapon()
{
	if (EquippedWeapon_Right != nullptr && !inMenu)
	{
		AShootingWeapon* shootingWeaponPtr = dynamic_cast<AShootingWeapon*>(EquippedWeapon_Right);
		if (shootingWeaponPtr)
		{

			//Check Aim Panel ------------------------------------------------------------------------------------

			UCanvasPanelSlot* AimCursorPanelSlot = (UCanvasPanelSlot*)AimCursorWidget->Slot;
			UCanvasPanelSlot* AimAreaPanelSlot = (UCanvasPanelSlot*)AimAreaWidget->Slot;
			FVector2D currentAimLocation = AimCursorPanelSlot->GetPosition();

			FVector HitLocation = FVector(0);
			FString ObjectHit = "Nothing";

			int32 ViewportSizeX, ViewportSizeY;
			playerController->GetViewportSize(ViewportSizeX, ViewportSizeY);

			bool bHit;
			FVector2D CrosshairPosition;
			if (bEnableAimRange)
			{
				// aim cursor at cursor location <--- NOTE (ZEROKIRI): position not correct atm.
				CrosshairPosition = currentAimLocation;
			}
			else
			{
				// aim cursor at center of the screen
				CrosshairPosition = FVector2D(ViewportSizeX / 2, ViewportSizeY / 2);
			}
			FHitResult HitResult;

			bHit = playerController->GetHitResultAtScreenPosition(CrosshairPosition, ECollisionChannel::ECC_WorldStatic, false, HitResult);

			if (bHit)
			{
				HitLocation = HitResult.ImpactPoint;
				ObjectHit = HitResult.GetActor()->GetName();
			}

			if (this->GetMesh()->GetSocketByName(FName("BulletSpawnSocket")) != NULL)
			{
				FVector socketLocation;
				FQuat socketRotation;
				this->GetMesh()->GetSocketWorldLocationAndRotation(FName("BulletSpawnSocket"), socketLocation, socketRotation);

				//DrawDebugLine(GetWorld(), socketLocation, HitLocation, FColor::Red, false, 5.0f, 0, 2);

				if (!disabledTuring)
				{
					FRotator newRotator = UKismetMathLibrary::FindLookAtRotation(socketLocation, HitLocation);
					FVector newVector = newRotator.Vector();

					//no change to z - So that player model will look up or down while bullet will fire up or down
					FQuat newRotation = FVector(newVector.X, newVector.Y, 0).ToOrientationQuat();

					if (newVector.X > socketRotation.GetForwardVector().X + acceptableTargeRange || newVector.X < socketRotation.GetForwardVector().X - acceptableTargeRange ||
						newVector.Y > socketRotation.GetForwardVector().Y + acceptableTargeRange || newVector.Y < socketRotation.GetForwardVector().Y - acceptableTargeRange)
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
					TArray<UStaticMeshComponent*> weaponMeshComponents;

					shootingWeaponPtr->GetComponents<UStaticMeshComponent>(weaponMeshComponents);
					for (UStaticMeshComponent* weaponMeshComponent : weaponMeshComponents)
					{
						if (weaponMeshComponent->GetName() == "BeamRifleMesh")
						{
							TArray<UStaticMeshSocket*> sockets = weaponMeshComponent->GetStaticMesh()->Sockets;

							for (UStaticMeshSocket* socket : sockets)
							{
								if (socket->SocketName.ToString() == FString("BulletSpawnSocket"))
								{
									socketLocation = shootingWeaponPtr->GetActorLocation() +
										UKismetMathLibrary::Multiply_VectorFloat(shootingWeaponPtr->GetActorForwardVector(), socket->RelativeLocation.X);

								}
							}

						}
					}

					//DrawDebugLine(GetWorld(), this->GetActorLocation(), socketLocation, FColor::Green, false, 5.0f, 0, 2);

					newRotator = UKismetMathLibrary::FindLookAtRotation(socketLocation, HitLocation);
					newRotation = newRotator.Quaternion();

					//shoot bullet
					shootingWeaponPtr->Shoot(socketLocation, newRotation);
				}
				else
				{
					TArray<UStaticMeshComponent*> weaponMeshComponents;

					shootingWeaponPtr->GetComponents<UStaticMeshComponent>(weaponMeshComponents);
					for (UStaticMeshComponent* weaponMeshComponent : weaponMeshComponents)
					{
						if (weaponMeshComponent->GetName() == "BeamRifleMesh")
						{
							TArray<UStaticMeshSocket*> sockets = weaponMeshComponent->GetStaticMesh()->Sockets;

							for (UStaticMeshSocket* socket : sockets)
							{
								if (socket->SocketName.ToString() == FString("BulletSpawnSocket"))
								{
									socketLocation = shootingWeaponPtr->GetActorLocation() +
										UKismetMathLibrary::Multiply_VectorFloat(shootingWeaponPtr->GetActorForwardVector(), socket->RelativeLocation.X);

								}
							}

						}
					}

					this->GetMesh()->GetSocketWorldLocationAndRotation(FName("BulletSpawnSocket"), socketLocation, socketRotation);
					FRotator newRotator = UKismetMathLibrary::FindLookAtRotation(socketLocation, HitLocation);
					FQuat newRotation = newRotator.Quaternion();

					//shoot bullet
					shootingWeaponPtr->Shoot(socketLocation, newRotation);
				}
			}
		}
	}
}
void AProjectMMWCharacter::DeActivateRightWeapon()
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
	currentSelectedWeaponSlot = 1;
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
	currentSelectedWeaponSlot = 2;
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
	currentSelectedWeaponSlot = 3;
}


void AProjectMMWCharacter::Reload()
{
	AShootingWeapon* shootingWeaponPtr = dynamic_cast<AShootingWeapon*>(EquippedWeapon_Right);
	if (shootingWeaponPtr)
	{
		shootingWeaponPtr->Reload();
	}
}
//void AProjectMMWCharacter::CreateBulletPool(int howMany) {
//	for (int i = 0; i < howMany; i++) {
//		ABullet* tempGo = GetWorld()->SpawnActor<ABullet>(ABullet::StaticClass(), FVector(-9999999, -9999999, -9999999), FRotator::ZeroRotator);
//		bulletPool.push_back(tempGo);
//	}
//}
#pragma endregion

#pragma region Character Stats Functions
void AProjectMMWCharacter::SetDefaultStats()
{
	inMenu = false;
	IsOverheat = false;
	IsVerticalBoost = false;
	IsBoosting = false;
	CurrentHp = MaxHp;
	MaxGroundSpeed = 400;
	MaxFlightSpeed = 400;
	AccelerationPower = 1.0f;
	BoostPower = 2;
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
	EquipableMeleeWeapons = GlobalSettingsActorClass.GetDefaultObject()->GetEquipableMeleeWeapons();
	EquipableShootingWeapons = GlobalSettingsActorClass.GetDefaultObject()->GetEquipableShootingWeapons();
	EquipableShields = GlobalSettingsActorClass.GetDefaultObject()->GetEquipableShields();

	/*int32 Count = EquipableShootingWeapons.Num();
	UE_LOG(LogTemp, Warning, TEXT("GlobalSettings itemsTotal: %d"), Count);*/
	//for (auto& Elem : EquipableShootingWeapons)
	//{
	//	/*Elem.Key,
	//	* Elem.Value*/
	//	UE_LOG(LogTemp, Warning, TEXT("GlobalSettingsItems"));
	//	UE_LOG(LogTemp, Warning, TEXT("GlobalSettings: %s"), *Elem.Key);
	//}

	//look for Beam Rifle
	TSubclassOf<AShootingWeapon>* BeamRiflePtr = EquipableShootingWeapons.Find("BeamRifle");
	TSubclassOf<AShootingWeapon> BeamRifle = BeamRiflePtr->Get();
	//Weapon1_Left = BeamRifle->GetDefaultObject<ABeamRifle>();


	TSubclassOf<AShootingWeapon>* CannonRiflePtr = EquipableShootingWeapons.Find("CannonRifle");
	TSubclassOf<AShootingWeapon> CannonRifle = CannonRiflePtr->Get();
	//Weapon2_Left = CannonRifle->GetDefaultObject<ACannonRifle>();

	TSubclassOf<AShield>* ShieldPtr = EquipableShields.Find("Shield");
	TSubclassOf<AShield> Shield = ShieldPtr->Get();

	//Equip BeamRifle to current Equipped Weapon_Left
	FVector actorLocation = GetActorLocation();
	FVector actorForwardVector = GetActorForwardVector() * 200;
	FVector NewLocation = actorForwardVector + actorLocation;


	if (this->GetMesh()->GetSocketByName(FName("LeftHandSocket")) != NULL)
	{
		UE_LOG(LogTemp, Log, TEXT("Character.cpp - LeftHandSocket Found!!"));
		const USkeletalMeshSocket* LeftHandSocket = GetMesh()->GetSocketByName("LeftHandSocket");

		//Spawn Weapons
		if (BeamRiflePtr)
		{
			Weapon1_Left = GetWorld()->SpawnActor<ABeamRifle>(BeamRiflePtr->Get(), GetActorLocation(), FRotator::ZeroRotator);
			LeftHandSocket->AttachActor(Weapon1_Left, GetMesh());
			Weapon1_Left->SetActive(false);
		}

		if (CannonRiflePtr)
		{
			Weapon2_Left = GetWorld()->SpawnActor<ACannonRifle>(CannonRiflePtr->Get(), GetActorLocation(), FRotator::ZeroRotator);
			LeftHandSocket->AttachActor(Weapon2_Left, GetMesh());
			Weapon2_Left->SetActive(false);
		}

		if (BeamRiflePtr)
		{
			Weapon3_Left = GetWorld()->SpawnActor<ABeamRifle>(BeamRiflePtr->Get(), GetActorLocation(), FRotator::ZeroRotator);
			LeftHandSocket->AttachActor(Weapon3_Left, GetMesh());
			Weapon3_Left->SetActive(false);
		}

		EquippedWeapon_Left = Weapon1_Left;
		EquippedWeapon_Left->SetActive(true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Character.cpp - LeftHandSocket Not Found!!"));
	}

	if (this->GetMesh()->GetSocketByName(FName("RightHandSocket")) != NULL)
	{
		UE_LOG(LogTemp, Log, TEXT("Character.cpp - RightHandSocket Found!!"));
		const USkeletalMeshSocket* RightHandSocket = GetMesh()->GetSocketByName("RightHandSocket");

		//Spawn Weapons
		if (CannonRiflePtr)
		{
			Weapon3_Right = GetWorld()->SpawnActor<ACannonRifle>(CannonRiflePtr->Get(), GetActorLocation(), FRotator::ZeroRotator);
			RightHandSocket->AttachActor(Weapon3_Right, GetMesh());
			Weapon3_Right->SetActive(false);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Character.cpp - RightHandSocket Not Found!!"));
	}

	if (this->GetMesh()->GetSocketByName(FName("LeftForeArmSocket")) != NULL)
	{
		UE_LOG(LogTemp, Log, TEXT("Character.cpp - LeftForeArmSocket Found!!"));
		const USkeletalMeshSocket* LeftForeArmSocket = GetMesh()->GetSocketByName("LeftForeArmSocket");

		//Spawn Weapons
		if (ShieldPtr)
		{
			Equipped_LeftForeArm = GetWorld()->SpawnActor<AShield>(ShieldPtr->Get(), GetActorLocation(), FRotator::ZeroRotator);
			LeftForeArmSocket->AttachActor(Equipped_LeftForeArm, GetMesh());
			Equipped_LeftForeArm->SetActive(true);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Character.cpp - RightHandSocket Not Found!!"));
	}
	currentSelectedWeaponSlot = 1;
}
void AProjectMMWCharacter::ReEquipWeapon(string weapon, FString weaponLocation)
{

	TSubclassOf<AShootingWeapon>* BeamRiflePtr = EquipableShootingWeapons.Find("BeamRifle");
	//TSubclassOf<AWeapon> BeamRifle = BeamRiflePtr->Get();
	TSubclassOf<AShootingWeapon>* CannonRiflePtr = EquipableShootingWeapons.Find("CannonRifle");
	//TSubclassOf<AWeapon> CannonRifle = CannonRiflePtr->Get();


	if (weaponLocation == "Weapon1_Left")
	{
		const USkeletalMeshSocket* LeftHandSocket = GetMesh()->GetSocketByName("LeftHandSocket");
		if (Weapon1_Left) Weapon1_Left->K2_DestroyActor();

		if (weapon == "None")
		{
			Weapon1_Left = nullptr;
		}
		else if (weapon == "BeamRifle")
		{
			Weapon1_Left = GetWorld()->SpawnActor<ABeamRifle>(BeamRiflePtr->Get(), GetActorLocation(), FRotator::ZeroRotator);
		}
		else if (weapon == "CannonRifle")
		{
			Weapon1_Left = GetWorld()->SpawnActor<ACannonRifle>(CannonRiflePtr->Get(), GetActorLocation(), FRotator::ZeroRotator);
		}

		if (Weapon1_Left != nullptr)
		{
			LeftHandSocket->AttachActor(Weapon1_Left, GetMesh());
			Weapon1_Left->SetActive(false);
		}
	}
	else if (weaponLocation == "Weapon1_Right")
	{
		const USkeletalMeshSocket* RightHandSocket = GetMesh()->GetSocketByName("RightHandSocket");
		if (Weapon1_Right) Weapon1_Right->K2_DestroyActor();

		if (weapon == "None")
		{
			Weapon1_Right = nullptr;
		}
		else if (weapon == "BeamRifle")
		{
			Weapon1_Right = GetWorld()->SpawnActor<ABeamRifle>(BeamRiflePtr->Get(), GetActorLocation(), FRotator::ZeroRotator);
		}
		else if (weapon == "CannonRifle")
		{
			Weapon1_Right = GetWorld()->SpawnActor<ACannonRifle>(CannonRiflePtr->Get(), GetActorLocation(), FRotator::ZeroRotator);
		}

		if (Weapon1_Right != nullptr)
		{
			RightHandSocket->AttachActor(Weapon1_Right, GetMesh());
			Weapon1_Right->SetActive(false);
		}
	}
	else if (weaponLocation == "Weapon2_Left")
	{
		const USkeletalMeshSocket* LeftHandSocket = GetMesh()->GetSocketByName("LeftHandSocket");
		if (Weapon2_Left) Weapon2_Left->K2_DestroyActor();

		if (weapon == "None")
		{
			Weapon2_Left = nullptr;
		}
		else if (weapon == "BeamRifle")
		{
			Weapon2_Left = GetWorld()->SpawnActor<ABeamRifle>(BeamRiflePtr->Get(), GetActorLocation(), FRotator::ZeroRotator);
		}
		else if (weapon == "CannonRifle")
		{
			Weapon2_Left = GetWorld()->SpawnActor<ACannonRifle>(CannonRiflePtr->Get(), GetActorLocation(), FRotator::ZeroRotator);
		}

		if (Weapon2_Left != nullptr)
		{
			LeftHandSocket->AttachActor(Weapon2_Left, GetMesh());
			Weapon2_Left->SetActive(false);
		}
	}
	else if (weaponLocation == "Weapon2_Right")
	{
		const USkeletalMeshSocket* RightHandSocket = GetMesh()->GetSocketByName("RightHandSocket");
		if (Weapon2_Right) Weapon2_Right->K2_DestroyActor();

		if (weapon == "None")
		{
			Weapon2_Right = nullptr;
		}
		else if (weapon == "BeamRifle")
		{
			Weapon2_Right = GetWorld()->SpawnActor<ABeamRifle>(BeamRiflePtr->Get(), GetActorLocation(), FRotator::ZeroRotator);
		}
		else if (weapon == "CannonRifle")
		{
			Weapon2_Right = GetWorld()->SpawnActor<ACannonRifle>(CannonRiflePtr->Get(), GetActorLocation(), FRotator::ZeroRotator);
		}

		if (Weapon2_Right != nullptr)
		{
			RightHandSocket->AttachActor(Weapon2_Right, GetMesh());
			Weapon2_Right->SetActive(false);
		}
	}
	else if (weaponLocation == "Weapon3_Left")
	{
		const USkeletalMeshSocket* LeftHandSocket = GetMesh()->GetSocketByName("LeftHandSocket");
		if (Weapon3_Left) Weapon3_Left->K2_DestroyActor();

		if (weapon == "None")
		{
			Weapon3_Left = nullptr;
		}
		else if (weapon == "BeamRifle")
		{
			Weapon3_Left = GetWorld()->SpawnActor<ABeamRifle>(BeamRiflePtr->Get(), GetActorLocation(), FRotator::ZeroRotator);
		}
		else if (weapon == "CannonRifle")
		{
			Weapon3_Left = GetWorld()->SpawnActor<ACannonRifle>(CannonRiflePtr->Get(), GetActorLocation(), FRotator::ZeroRotator);
		}

		if (Weapon3_Left != nullptr)
		{
			LeftHandSocket->AttachActor(Weapon3_Left, GetMesh());
			Weapon3_Left->SetActive(false);
		}
	}
	else if (weaponLocation == "Weapon3_Right")
	{
		const USkeletalMeshSocket* RightHandSocket = GetMesh()->GetSocketByName("RightHandSocket");
		if (Weapon3_Right) Weapon3_Right->K2_DestroyActor();

		if (weapon == "None")
		{
			Weapon3_Right = nullptr;
		}
		else if (weapon == "BeamRifle")
		{
			Weapon3_Right = GetWorld()->SpawnActor<ABeamRifle>(BeamRiflePtr->Get(), GetActorLocation(), FRotator::ZeroRotator);
		}
		else if (weapon == "CannonRifle")
		{
			Weapon3_Right = GetWorld()->SpawnActor<ACannonRifle>(CannonRiflePtr->Get(), GetActorLocation(), FRotator::ZeroRotator);
		}

		if (Weapon3_Right != nullptr)
		{
			RightHandSocket->AttachActor(Weapon3_Right, GetMesh());
			Weapon3_Right->SetActive(false);
		}
	}

	if (currentSelectedWeaponSlot == 1)
	{
		SwitchToWeapon1();
	}
	else if (currentSelectedWeaponSlot == 2)
	{
		SwitchToWeapon2();
	}
	else if (currentSelectedWeaponSlot == 3)
	{
		SwitchToWeapon3();
	}
}

void AProjectMMWCharacter::ToggleInventory()
{
	UE_LOG(LogTemp, Warning, TEXT("ToggleInventory"));
	if (PlayerStatusMenuWidget) {

		if (StatusMenuInstance != nullptr)
		{

			if (!StatusMenuInstance->IsVisible())
			{
				UE_LOG(LogTemp, Warning, TEXT("ToggleInventory - turn on"));
				StatusMenuInstance->SetVisibility(ESlateVisibility::Visible);
				playerController->bShowMouseCursor = true;
				playerController->bEnableClickEvents = true;
				playerController->bEnableMouseOverEvents = true;
				playerController->SetIgnoreLookInput(true);
				inMenu = true;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("ToggleInventory - turn off"));
				StatusMenuInstance->SetVisibility(ESlateVisibility::Hidden);
				playerController->bShowMouseCursor = false;
				playerController->bEnableClickEvents = false;
				playerController->bEnableMouseOverEvents = false;
				playerController->SetIgnoreLookInput(false);
				inMenu = false;
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerStatusMenuWidgetInstance does not exists!!"));
		}
	}
}

void AProjectMMWCharacter::CheckStats()
{
	UCharacterMovementComponent *MovementPtr =  Cast<UCharacterMovementComponent>(GetCharacterMovement());
	if (!IsOverheat && IsBoosting)
	{
		if (MovementPtr->MaxWalkSpeed != MaxGroundSpeed * BoostPower)
		{
			MovementPtr->MaxWalkSpeed = MaxGroundSpeed * BoostPower;
		}
		if (MovementPtr->MaxFlySpeed != MaxFlightSpeed * BoostPower)
		{
			MovementPtr->MaxFlySpeed = MaxFlightSpeed * BoostPower;
		}
	}
	else
	{
		if (MovementPtr->MaxWalkSpeed != MaxFlightSpeed)
		{
			MovementPtr->MaxWalkSpeed = MaxFlightSpeed;
		}
		if (MovementPtr->MaxFlySpeed != MaxFlightSpeed)
		{
			MovementPtr->MaxFlySpeed = MaxFlightSpeed;
		}
	}
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
void AProjectMMWCharacter::CheckDisabledTurning(float DeltaTime)
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

#pragma region getter and setters
FText AProjectMMWCharacter::GetCurrentHealth()
{
	return FText::FromString(FString::SanitizeFloat(CurrentHp, 0));
}
FText AProjectMMWCharacter::GetMaxHealth()
{
	return FText::FromString(FString::SanitizeFloat(MaxHp, 0));
}
FText AProjectMMWCharacter::GetHealthIntText()
{
	int32 HP = FMath::RoundHalfFromZero(HealthPercentage * 100);
	FString HPS = FString::FromInt(HP);
	FString HealthHUD = HPS + FString(TEXT("%"));
	FText HPText = FText::FromString(HealthHUD);
	return HPText;
}
float AProjectMMWCharacter::GetHealthPercentage()
{
	float healthPercentage = CurrentHp / MaxHp;
	return healthPercentage;
}
FText AProjectMMWCharacter::GetCurrentEnergy()
{
	return FText::FromString(FString::SanitizeFloat(CurrentEnergy, 0));
}
FText AProjectMMWCharacter::GetMaxEnergy()
{
	return FText::FromString(FString::SanitizeFloat(MaxEnergy, 0));
}

FText AProjectMMWCharacter::GetEnergyIntText()
{
	int32 Energy = FMath::RoundHalfFromZero(EnergyPercentage * 100);
	FString EnergyS = FString::FromInt(Energy);
	FString EnergyHUD = EnergyS + FString(TEXT("%"));
	FText EnergyText = FText::FromString(EnergyHUD);
	return EnergyText;
}

FText AProjectMMWCharacter::GetWeaponLeft_CurrentClipSize()
{
	if (EquippedWeapon_Left != nullptr)
	{
		AShootingWeapon* shootingWeaponPtr = dynamic_cast<AShootingWeapon*>(EquippedWeapon_Left);
		if (shootingWeaponPtr)
		{
			return FText::AsNumber(shootingWeaponPtr->GetCurrentClipSize());
		}
	}
	return FText::FromString("0");
}

FText AProjectMMWCharacter::GetWeaponLeft_CurrentTotalAmmo()
{
	if (EquippedWeapon_Left != nullptr)
	{
		AShootingWeapon* shootingWeaponPtr = dynamic_cast<AShootingWeapon*>(EquippedWeapon_Left);
		if (shootingWeaponPtr)
		{
			return FText::AsNumber(shootingWeaponPtr->GetCurrentTotalAmmo());
		}
	}
	return FText::FromString("0");
}

FText AProjectMMWCharacter::GetWeaponRight_CurrentClipSize()
{
	if (EquippedWeapon_Right != nullptr)
	{
		AShootingWeapon* shootingWeaponPtr = dynamic_cast<AShootingWeapon*>(EquippedWeapon_Left);
		if (shootingWeaponPtr)
		{
			return FText::AsNumber(shootingWeaponPtr->GetCurrentClipSize());
		}
	}
	return FText::FromString("0");
}

FText AProjectMMWCharacter::GetWeaponRight_CurrentTotalAmmo()
{
	if (EquippedWeapon_Right != nullptr)
	{
		AShootingWeapon* shootingWeaponPtr = dynamic_cast<AShootingWeapon*>(EquippedWeapon_Left);
		if (shootingWeaponPtr)
		{
			return FText::AsNumber(shootingWeaponPtr->GetCurrentTotalAmmo());
		}
	}
	return FText::FromString("0");
}

float AProjectMMWCharacter::GetWeaponLeft_ReloadPercentage()
{
	if (EquippedWeapon_Left != nullptr)
	{
		AShootingWeapon* shootingWeaponPtr = dynamic_cast<AShootingWeapon*>(EquippedWeapon_Left);
		if (shootingWeaponPtr)
		{
			return shootingWeaponPtr->GetReloadPercentage();
		}
	}
	return 0;
}

float AProjectMMWCharacter::GetWeaponRight_ReloadPercentage()
{
	if (EquippedWeapon_Right != nullptr)
	{
		AShootingWeapon* shootingWeaponPtr = dynamic_cast<AShootingWeapon*>(EquippedWeapon_Left);
		if (shootingWeaponPtr)
		{
			return shootingWeaponPtr->GetReloadPercentage();
		}
	}
	return 0;
}


void AProjectMMWCharacter::SetMaxHpStats(int points)
{
	
	switch (points)
	{
	case 1:
		this->MaxHp = 1000;
		StatusMenuInstance->SetAssignedMaxHealthPoints(1);
		break;
	case 2:
		this->MaxHp = 2000;
		StatusMenuInstance->SetAssignedMaxHealthPoints(2);
		break;
	case 3:
		this->MaxHp = 3000;
		StatusMenuInstance->SetAssignedMaxHealthPoints(3);
		break;
	case 4:
		this->MaxHp = 4000;
		StatusMenuInstance->SetAssignedMaxHealthPoints(4);
		break;
	case 5:
		this->MaxHp = 5000;
		StatusMenuInstance->SetAssignedMaxHealthPoints(5);
		break;
	}
}
void AProjectMMWCharacter::SetMaxEnergyStats(int points)
{
	//MaxEnergy = energyPoints;

	switch (points)
	{
	case 1:
		MaxEnergy = 1000;
		StatusMenuInstance->SetAssignedMaxEnergyPoints(1);
		break;
	case 2:
		MaxEnergy = 2000;
		StatusMenuInstance->SetAssignedMaxEnergyPoints(2);
		break;
	case 3:
		MaxEnergy = 3000;
		StatusMenuInstance->SetAssignedMaxEnergyPoints(3);
		break;
	case 4:
		MaxEnergy = 4000;
		StatusMenuInstance->SetAssignedMaxEnergyPoints(4);
		break;
	case 5:
		MaxEnergy = 5000;
		StatusMenuInstance->SetAssignedMaxEnergyPoints(5);
		break;
	}
}
void AProjectMMWCharacter::SetEnergyRegenStats(int points)
{
	switch (points)
	{
	case 1:
		EnergyRegen = 1;
		StatusMenuInstance->SetAssignedEnergyRegenPoints(1);
		break;
	case 2:
		EnergyRegen = 2;
		StatusMenuInstance->SetAssignedEnergyRegenPoints(2);
		break;
	case 3:
		EnergyRegen = 3;
		StatusMenuInstance->SetAssignedEnergyRegenPoints(3);
		break;
	case 4:
		EnergyRegen = 4;
		StatusMenuInstance->SetAssignedEnergyRegenPoints(4);
		break;
	case 5:
		EnergyRegen = 5;
		StatusMenuInstance->SetAssignedEnergyRegenPoints(5);
		break;
	}
}
//void AProjectMMWCharacter::SetMassStats(int points)
//{
//	UCharacterMovementComponent* MovementPtr = Cast<UCharacterMovementComponent>(GetCharacterMovement());
//
//	switch (points)
//	{
//	case 1:
//		MovementPtr->Mass = 100;
//		StatusMenuInstance->SetAssignedAccelerationPoints(1);
//		break;
//	case 2:
//		MovementPtr->Mass = 150;
//		StatusMenuInstance->SetAssignedAccelerationPoints(2);
//		break;
//	case 3:
//		MovementPtr->Mass = 200;
//		StatusMenuInstance->SetAssignedAccelerationPoints(3);
//		break;
//	case 4:
//		MovementPtr->Mass = 250;
//		StatusMenuInstance->SetAssignedAccelerationPoints(4);
//		break;
//	case 5:
//		MovementPtr->Mass = 300;
//		StatusMenuInstance->SetAssignedAccelerationPoints(5);
//		break;
//	}
//}
void AProjectMMWCharacter::SetAccelerationStats(int points)
{
	switch (points)
	{
	case 1:
		AccelerationPower = 1;
		StatusMenuInstance->SetAssignedAccelerationPoints(1);
		break;
	case 2:
		AccelerationPower = 2;
		StatusMenuInstance->SetAssignedAccelerationPoints(2);
		break;
	case 3:
		AccelerationPower = 3;
		StatusMenuInstance->SetAssignedAccelerationPoints(3);
		break;
	case 4:
		AccelerationPower = 4;
		StatusMenuInstance->SetAssignedAccelerationPoints(4);
		break;
	case 5:
		AccelerationPower = 5;
		StatusMenuInstance->SetAssignedAccelerationPoints(5);
		break;
	}
}
void AProjectMMWCharacter::SetMaxGroundSpeedStats(int points)
{
	switch (points)
	{
	case 1:
		MaxGroundSpeed = 400;
		StatusMenuInstance->SetAssignedMaxGroundSpeedPoints(1);
		break;
	case 2:
		MaxGroundSpeed = 600;
		StatusMenuInstance->SetAssignedMaxGroundSpeedPoints(2);
		break;
	case 3:
		MaxGroundSpeed = 800;
		StatusMenuInstance->SetAssignedMaxGroundSpeedPoints(3);
		break;
	case 4:
		MaxGroundSpeed = 1000;
		StatusMenuInstance->SetAssignedMaxGroundSpeedPoints(4);
		break;
	case 5:
		MaxGroundSpeed = 1200;
		StatusMenuInstance->SetAssignedMaxGroundSpeedPoints(5);
		break;
	}
}
void AProjectMMWCharacter::SetMaxFlightSpeedStats(int points)
{
	switch (points)
	{
	case 1:
		MaxFlightSpeed = 400;
		StatusMenuInstance->SetAssignedMaxFlightSpeedPoints(1);
		break;
	case 2:
		MaxFlightSpeed = 600;
		StatusMenuInstance->SetAssignedMaxFlightSpeedPoints(2);
		break;
	case 3:
		MaxFlightSpeed = 800;
		StatusMenuInstance->SetAssignedMaxFlightSpeedPoints(3);
		break;
	case 4:
		MaxFlightSpeed = 1000;
		StatusMenuInstance->SetAssignedMaxFlightSpeedPoints(4);
		break;
	case 5:
		MaxFlightSpeed = 1200;
		StatusMenuInstance->SetAssignedMaxFlightSpeedPoints(5);
		break;
	}
}
void AProjectMMWCharacter::SetMaxFlightPowerStats(int points)
{
	switch (points)
	{
	case 1:
		FlightPower = 0.5f;
		StatusMenuInstance->SetAssignedFlightPowerPoints(1);
		break;
	case 2:
		FlightPower = 1;
		StatusMenuInstance->SetAssignedFlightPowerPoints(2);
		break;
	case 3:
		FlightPower = 1.5f;
		StatusMenuInstance->SetAssignedFlightPowerPoints(3);
		break;
	case 4:
		FlightPower = 2;
		StatusMenuInstance->SetAssignedFlightPowerPoints(4);
		break;
	case 5:
		FlightPower = 2.5f;
		StatusMenuInstance->SetAssignedFlightPowerPoints(5);
		break;
	}
}


void AProjectMMWCharacter::SetMechRotateOnGround(bool enable)
{
	if (enable) bMechRotateOnGround = true;
	else bMechRotateOnGround = false;
}

void AProjectMMWCharacter::SetEnableAimRange(bool enable)
{
	UImage* AimAreaWidgetImage = (UImage*)AimAreaWidget;
	if (enable)
	{
		AimAreaWidgetImage->SetColorAndOpacity(FLinearColor(0.5f, 0.5f, 0.5f, 0.2f));
		bEnableAimRange = true;
	}
	else
	{
		AimAreaWidgetImage->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.0f));
		bEnableAimRange = false;
	}
}

void AProjectMMWCharacter::SetInMenu(bool isInMenu)
{
	if (isInMenu) inMenu = true;
	else inMenu = false;
}

#pragma endregion
#pragma endregion