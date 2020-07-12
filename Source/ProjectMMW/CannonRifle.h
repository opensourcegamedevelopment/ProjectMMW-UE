// @CopyRight 2018 Open Source Game Dev

#pragma once

#include "CoreMinimal.h"
#include "ShootingWeapon.h"
#include "UObject/ConstructorHelpers.h"
#include "CannonRifle.generated.h"

UCLASS()
class PROJECTMMW_API ACannonRifle : public AShootingWeapon
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACannonRifle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	

};
