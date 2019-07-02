// Fill out your copyright notice in the Description page of Project Settings.

#include "StaticEnemyObject.h"
#include "Engine/StaticMesh.h"						//Added by Tarmous/ Taslink

// Sets default values
AStaticEnemyObject::AStaticEnemyObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	currentHealth = maxHealth;
	state = State::isIdle;
	respawnTimer = 0;					// we count up to the respawnCooldown
	spawnLocation = FVector(0, 0, 0);
}

// Called when the game starts or when spawned
void AStaticEnemyObject::BeginPlay()
{
	Super::BeginPlay();

	spawnLocation = GetActorLocation();
	GetComponents<UStaticMeshComponent>(components);

	GetComponents<UWidgetComponent>(statusBarComponents);
	statusBarComponent = statusBarComponents[0]; //Should only have 1
	if (GEngine) 
	{
		CameraLocation = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager->GetCameraLocation();
		statusBarlocation = statusBarComponent->RelativeLocation;
		statusBarComponent->SetRelativeRotation(FRotationMatrix::MakeFromXZ(CameraLocation - statusBarlocation, CameraLocation - statusBarlocation).Rotator());
	}
}

// Called every frame
void AStaticEnemyObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// this could be considered a behavior tree
	switch (state)
	{
		case AStaticEnemyObject::isIdle:
									// this is purely for testing.
			//DamageObject(1);		//enable only to test the object taking damage
									//(takes a few seconds to die)
			break;

		case AStaticEnemyObject::isDead:
			respawnTimer += DeltaTime;
			if (respawnTimer >= respawnCooldown) RespawnObject(spawnLocation);
			break;

		default:
			break;
	}

	if (GEngine)
	{
		CameraLocation = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager->GetCameraLocation();
		statusBarlocation = statusBarComponent->RelativeLocation;
		statusBarComponent->SetRelativeRotation(FRotationMatrix::MakeFromXZ(CameraLocation - statusBarlocation, CameraLocation - statusBarlocation).Rotator());
	}
}

void AStaticEnemyObject::DamageObject(int damage)
{
	currentHealth -= damage;
	if (currentHealth <= 0)
	{
		currentHealth = 0;
		state = State::isDead;
		// maybe do some cool animations here and disable collider

		for (int16 i = 0; i<components.Num(); i++)
		{
			UStaticMeshComponent* StaticMeshComponent = components[i];
			//UStaticMesh* StaticMesh = StaticMeshComponent->GetStaticMesh();	// use if you want to get the actual static mesh
			StaticMeshComponent->SetVisibility(false, false);					// Do we wanna propagate visibility to children?
		}
	}

}

void AStaticEnemyObject::DamageObject(int damage, AActor* actor)
{
	DamageObject(damage);

	TArray<UActorComponent*> MArray;
	actor->GetComponents<UActorComponent>(MArray);

	for (UActorComponent* comp : MArray)
	{
		UE_LOG(LogTemp, Warning, TEXT("component->GetName(): %s"), *comp->GetName());

		if (comp->GetName().Compare("StatusBar") == 0)
		{
			UWidgetComponent* WidgetComponent = (UWidgetComponent*) comp; //Cast Actor Component to widget component
			UUserWidget* UserWidget = WidgetComponent->GetUserWidgetObject(); //Retrieve user widget 
			
			UStaticEnemyHUDWidget* EnemyHUDWidget = (UStaticEnemyHUDWidget*) UserWidget; //Cast user widget to HUDWidget Widget (HP bar of the blueprint)

			UWidgetTree* tree = EnemyHUDWidget->WidgetTree; //Retrieve widgetTree in the HUD
			UWidget* HealthBar = tree->FindWidget("HealthBar"); //get health bar
			UProgressBar* HealthProgressBar = (UProgressBar*)HealthBar; // get progress bar of the health bar
			HealthProgressBar->Percent = GetHealthPercentage(); //set progress
			EnemyHUDWidget->SetHealthPercentage(GetHealthPercentage());
		}
	}
}

void AStaticEnemyObject::RespawnObject(FVector location)
{
	respawnTimer = 0;	// reset timer back to 0

	state = State::isIdle;
	currentHealth = maxHealth;
	SetActorLocation(location);

	// re-display | re-animate dead mesh component ?!
	for (int16 i = 0; i<components.Num(); i++)
	{
		UStaticMeshComponent* StaticMeshComponent = components[i];
		//UStaticMesh* StaticMesh = StaticMeshComponent->GetStaticMesh();	//use if you want to get the actual static mesh
		StaticMeshComponent->SetVisibility(true, false);					// Do we wanna propagate visibility to children?
	}

}

float AStaticEnemyObject::GetHealthPercentage()
{
	float result = (float)currentHealth / (float)maxHealth;
	return result;
}

