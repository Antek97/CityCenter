// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CityCenterProjectile.generated.h"

class ACityCenterFlockActor;
class UProjectileMovementComponent;
class USphereComponent;
class UCapsuleComponent;
class UCurveFloat;

UCLASS(config=Game)
class ACityCenterProjectile : public AActor
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, Category = "Trigger Capsule")
	class UCapsuleComponent* TriggerCapsule;

	UPROPERTY(EditAnywhere)
	UCurveFloat* SpeedOverTime = nullptr;

public:
	ACityCenterProjectile();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay();

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	ACityCenterFlockActor* TargetFlock = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UStaticMeshComponent* TargetBird = nullptr;

	bool bIsOverlapBegin = false;

	FVector VelocityProjectile = FVector::ZeroVector;
	TArray<ACityCenterFlockActor*> FoundActors;

	void UpdateEnergy(float DeltaTime);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float MaxEnergy = 150;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float ProjectileVelocity = MaxEnergy / 2;

	UFUNCTION(BlueprintCallable)
	void ChangeToTarget();

	bool bFlagCount = false;

	UFUNCTION(BlueprintCallable)
	void SetFolow();
	
};

