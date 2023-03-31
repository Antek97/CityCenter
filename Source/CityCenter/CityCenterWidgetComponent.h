// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "Components/ActorComponent.h"
#include "CityCenterWidgetComponent.generated.h"

class ACityCenterCharacter;

DECLARE_EVENT_OneParam(UCityCenterWidgetComponent, FCrosshairUpdate, FName);

USTRUCT(BlueprintType)
struct FCrosshairProperties : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FName CrosshairTag;
	
	UPROPERTY(BlueprintReadWrite)
	int32 AnimationKey;

	UPROPERTY(BlueprintReadWrite)
	FColor ColorCrosshair;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CITYCENTER_API UCityCenterWidgetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCityCenterWidgetComponent();

	FCrosshairUpdate OnUpdateCrosshair;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	ACityCenterCharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere, Category = "Main")
	FName CrosshairActualy = FName(TEXT("ThisIsMyTestFName"));

	UPROPERTY(EditAnywhere, Category = "Main")
	TArray<FName> CrosshairArray;

	FName CrosshairIndex = FName("Default");

	void SetCrosshair();
};

