// Fill out your copyright notice in the Description page of Project Settings.


#include "CityCenterWidgetComponent.h"
#include "CityCenterCharacter.h"

// Sets default values for this component's properties
UCityCenterWidgetComponent::UCityCenterWidgetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UCityCenterWidgetComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCityCenterWidgetComponent::SetCrosshair()
{
	PlayerCharacter = CastChecked<ACityCenterCharacter>(GetOwner());
	check(PlayerCharacter);

	if (!PlayerCharacter)
	{
		PlayerCharacter = CastChecked<ACityCenterCharacter>(GetOwner());
		check(PlayerCharacter);
	}
		for (auto ArrayIndex = 0; ArrayIndex < CrosshairArray.Num() ; ArrayIndex++)
		{
			if (CrosshairArray[ArrayIndex] == CrosshairIndex)
			{
				OnUpdateCrosshair.RemoveAll(this);

				if (ArrayIndex == CrosshairArray.Num() - 1)
				{
					CrosshairIndex = CrosshairArray[0];
				}
				else
				{
					CrosshairIndex = CrosshairArray[ArrayIndex + 1];
				}
				
				OnUpdateCrosshair.Broadcast(CrosshairIndex);

				break;
			}
		}

}
