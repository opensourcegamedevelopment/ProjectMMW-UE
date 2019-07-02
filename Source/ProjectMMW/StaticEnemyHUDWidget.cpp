// @CopyRight 2018 Open Source Game Dev


#include "StaticEnemyHUDWidget.h"

float UStaticEnemyHUDWidget::GetHealthPercentage()
{
	return currentHealthPercentage;
}

void UStaticEnemyHUDWidget::SetHealthPercentage(float percentage)
{
	currentHealthPercentage = percentage;
}