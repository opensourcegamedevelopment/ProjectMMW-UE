// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Damageable.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "Engine/Engine.h"
#include "StaticEnemyHUDWidget.h"
#include "StaticEnemyObject.generated.h"

//, public IDamageable
UCLASS()
class PROJECTMMW_API AStaticEnemyObject : public AActor, public IDamageable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AStaticEnemyObject();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void DamageObject(int damage);	// deal damage to object
	void DamageObject(int damage, AActor* actor);	// deal damage to object
	UFUNCTION(BlueprintPure, Category = Stats)
	float GetHealthPercentage();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stats)
		int currentHealth;

	void RespawnObject(FVector location);			// respawn object at location x,y,z in world map

private:
	int maxHealth = 1000;
	
	float respawnTimer = 0;							// timer used for respawning
	const float respawnCooldown = 10;				// object respawns every X seconds using the above timer
	enum State
	{
		isIdle,
		isDead
	};											// More potential states in future, like isTargeting, isFiring, isSearching
	State state = State::isIdle;				// to hold the enum value
	FVector spawnLocation;						// the location is saved during the start,
													// one the object enters in "isDead" state,
													// after 10 seconds it resets to that position
													// PS: No actual Destruction of object is happening

	TArray<UStaticMeshComponent*> components;	// this is used to edit the UStaticMeshComponents under root (ex: toggle vissibility)
	TArray<UWidgetComponent*> statusBarComponents;    // this is used to edit the UWidgetComponent under root
	UWidgetComponent* statusBarComponent;
	FVector CameraLocation;
	FVector statusBarlocation;
};
