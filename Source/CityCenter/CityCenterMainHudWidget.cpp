// Fill out your copyright notice in the Description page of Project Settings.


#include "CityCenterMainHudWidget.h"
#include "CityCenterWidgetComponent.h"


void UCityCenterMainHudWidget::InitPlayerComponent(UCityCenterWidgetComponent* PlayerHudComponent)
{
	check(PlayerHudComponent);
	CachedPlayerComponent = PlayerHudComponent;
	CachedPlayerComponent->OnUpdateCrosshair.AddUObject(this, &UCityCenterMainHudWidget::UpdateCrosshair);
}

void UCityCenterMainHudWidget::BeginDestroy()
{
	if (CachedPlayerComponent)
	{
		CachedPlayerComponent->OnUpdateCrosshair.RemoveAll(this);
	}

	Super::BeginDestroy();
}