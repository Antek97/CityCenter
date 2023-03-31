// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CityCenterHUD.generated.h"

class UCityCenterMainHudWidget;
class UCityCenterWidgetComponent;

DECLARE_EVENT(ACityCenterHUD, FHudIsActive);

UCLASS()
class ACityCenterHUD : public AHUD
{
	GENERATED_BODY()

public:
	ACityCenterHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:

	bool bNeedsHudInit = true;
public:

	FHudIsActive OnUpdateCrosshair;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Main Hud")
	TSubclassOf<UUserWidget> WidgetClass;

	UPROPERTY(VisibleInstanceOnly, Category = "Main Hud")
	UCityCenterMainHudWidget* MainHudWidget;

	void InitializeHud();
	void InitializeComponent(UCityCenterWidgetComponent* PlayerHudComponent);
};

