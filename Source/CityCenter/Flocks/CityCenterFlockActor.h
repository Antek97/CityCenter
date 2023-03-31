// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CityCenterFlockActor.generated.h"

class UCapsuleComponent;

UCLASS()
class CITYCENTER_API ACityCenterFlockActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACityCenterFlockActor();
	
	UPROPERTY(EditAnywhere, Category = "Flock")
	USceneComponent* Root;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	UStaticMeshComponent* StaticMesh_01;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	UStaticMesh* StaticMeshBalls;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* Mesh1P;

	UPROPERTY(VisibleAnywhere, Category = "Trigger Capsule")
	UCapsuleComponent* TriggerCapsule;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	void MoveFlockToNewLocation(float DeltaTime);
	void ChangeFlockDispersion();
	void ChangeFlightFlockDirection();

	FVector SetPositionCenterNeighboringBirds(UStaticMeshComponent* StaticMesh);
	FVector SetAwaySpace(UStaticMeshComponent* StaticMesh);
	FVector SetMeshVelocity(UStaticMeshComponent* StaticMesh);
	FVector SetBoundLocation(UStaticMeshComponent* StaticMesh);
	FVector SetLimitVelocity(FVector Velocity);


public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay)
	TArray<UStaticMeshComponent*> BirdsInFlock;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay)
	TArray<FVector> VelocityFlock;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay)
	int32 QuantityBirdsInFlock = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector SafeDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MaxFlockDistance = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MinFlockDistance = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int32 LimitVelocity = 100;

	UFUNCTION(BlueprintCallable)
	void HitFlock(UStaticMeshComponent* Flock);

	UFUNCTION(BlueprintCallable)
	void AddBirdToFlock(UStaticMeshComponent* Flock, FVector Position);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int32 FlockFocus = FlockFocusDefault;

	UPROPERTY(EditAnywhere, Category = Gameplay)
	int32 FlockFocusDefault = 1;

	UPROPERTY(EditAnywhere, Category = Gameplay)
	int32 FlockDispersion = - 3;

	UPROPERTY(EditAnywhere, Category = Gameplay)
	int32 AvoidingObstacles = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int32 EnergyInBird = 50;

};
