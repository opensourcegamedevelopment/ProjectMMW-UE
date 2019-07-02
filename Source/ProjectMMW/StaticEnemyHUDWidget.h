// @CopyRight 2018 Open Source Game Dev

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "ProgressBar.h"
#include "StaticEnemyHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMMW_API UStaticEnemyHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = Stats)
		float GetHealthPercentage();

	void SetHealthPercentage(float percentage);

protected:

private:
	float currentHealthPercentage;
};
