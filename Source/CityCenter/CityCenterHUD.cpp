// Copyright Epic Games, Inc. All Rights Reserved.

#include "CityCenterHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"

#include "CityCenterWidgetComponent.h"
#include "CityCenterMainHudWidget.h"

ACityCenterHUD::ACityCenterHUD()
{
}

void ACityCenterHUD::DrawHUD()
{
	if (bNeedsHudInit)
	{
		bNeedsHudInit = false;

		InitializeHud();
	}

	Super::DrawHUD();
}

void ACityCenterHUD::InitializeHud()
{
	if (IsValid(WidgetClass))
	{
		if (MainHudWidget == NULL)
		{
			MainHudWidget = Cast <UCityCenterMainHudWidget>(CreateWidget(GetWorld(), WidgetClass));
		}

		if (MainHudWidget != nullptr)
		{
			MainHudWidget->AddToViewport();

			OnUpdateCrosshair.Broadcast();
		}
	}
}

void ACityCenterHUD::InitializeComponent(UCityCenterWidgetComponent* PlayerHudComponent)
{
	MainHudWidget->InitPlayerComponent(PlayerHudComponent);
}
