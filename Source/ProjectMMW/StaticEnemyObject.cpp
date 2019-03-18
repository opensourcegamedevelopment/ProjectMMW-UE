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
