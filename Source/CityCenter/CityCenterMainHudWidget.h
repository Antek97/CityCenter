// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CityCenterMainHudWidget.generated.h"

class UCityCenterWidgetComponent;

UCLASS()
class CITYCENTER_API UCityCenterMainHudWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UCityCenterWidgetComponent* CachedPlayerComponent = nullptr;

	void InitPlayerComponent(UCityCenterWidgetComponent* PlayerHudComponent);

	UFUNCTION(BlueprintImplementableEvent, Category = Crosshair)
	void UpdateCrosshair(FName Crosshair);

	virtual void BeginDestroy();
};
