// @CopyRight 2018 Open Source Game Dev


#include "StaticEnemyHUDWidget.h"

UStaticEnemyHUDWidget::UStaticEnemyHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SetHealthPercentage(1);
}

float UStaticEnemyHUDWidget::GetHealthPercentage()
{
	return currentHealthPercentage;
}

void UStaticEnemyHUDWidget::SetHealthPercentage(float percentage)
{
	currentHealthPercentage = percentage;
}