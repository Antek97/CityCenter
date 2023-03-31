// Copyright Epic Games, Inc. All Rights Reserved.

#include "CityCenterProjectile.h"
#include "CityCenter/Flocks/CityCenterFlockActor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Curves/CurveFloat.h"

#include "Kismet/GameplayStatics.h"

ACityCenterProjectile::ACityCenterProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ACityCenterProjectile::OnHit);	// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	TriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("TriggerCapsule"));
	TriggerCapsule->InitCapsuleSize(55.f, 55.f);
	TriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	TriggerCapsule->SetupAttachment(RootComponent);

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	
	TriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &ACityCenterProjectile::OnOverlapBegin);
	// Die after 3 seconds by default
	InitialLifeSpan = 0.0f;
}

void ACityCenterProjectile::Tick(float DeltaTime)
{
	UpdateEnergy(DeltaTime);
	Super::Tick(DeltaTime);

#define DWA

#ifdef JEDEN
	if (ProjectileMovement && SpeedOverTime)
	{
		// UCurveFloat
		const float Factor = SpeedOverTime->GetFloatValue(GetGameTimeSinceCreation());
		FVector Force = GetActorForwardVector();
		ProjectileMovement->AddForce(Force * Factor);
	}
#elif defined(DWA)
	if (ProjectileMovement)
	{
		//runtime
		FVector Acceleration = ProjectileMovement->ComputeAcceleration(ProjectileMovement->Velocity, DeltaTime);
		const float CurrSpeed = ProjectileMovement->Velocity.Size();

		if(Acceleration == FVector::ZeroVector || FVector::DotProduct(ProjectileMovement->Velocity.GetSafeNormal(), Acceleration.GetSafeNormal()) > 0.5f)
		{
			const float VelocityFactor = (ProjectileVelocity / 100) - 0.5f;
			Acceleration = GetActorForwardVector() * CurrSpeed * VelocityFactor;
		}
		ProjectileMovement->AddForce(Acceleration);
	}
#endif
}

void ACityCenterProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void ACityCenterProjectile::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
		if (TargetFlock != nullptr)
		{
			//TargetBird = Cast<UStaticMeshComponent>(OtherComp);
			TargetFlock->HitFlock(TargetBird);
			ProjectileVelocity += 50;
		}
}

void ACityCenterProjectile::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void ACityCenterProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//TargetFlock = nullptr;
	//TargetFlock = Cast<ACityCenterFlockActor>(OtherActor);

	//if (TargetFlock != nullptr)
	//{
	//	TargetBird = Cast<UStaticMeshComponent>(OtherComp);
	//	TargetFlock->HitFlock(TargetBird);
	//}

	//// Only add impulse and destroy projectile if we hit a physics
	//if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && TargetFlock != nullptr/* && OtherComp->IsSimulatingPhysics()*/)
	//{
	//	OtherComp->AddImpulseAtLocation(GetVelocity() * 500.0f, GetActorLocation());
	//	
	//	ProjectileVelocity += 50;
	//}
}

void ACityCenterProjectile::UpdateEnergy(float DeltaTime)
{
	if (ProjectileVelocity <= 0)
	{
		ChangeToTarget();
	}
	else if(ProjectileVelocity > MaxEnergy)
	{
		ProjectileVelocity = MaxEnergy;
	}
	else
	{
		if (ProjectileVelocity == MaxEnergy)
		{
			ProjectileVelocity -= 1;
			bFlagCount = true;
		}
		else 
		{
			if (bFlagCount)
			{
				ProjectileVelocity -= 1;
				bFlagCount = true;
			}
			else
			{
				ProjectileVelocity += 1;
			}
		}
	}
}

void ACityCenterProjectile::ChangeToTarget()
{
	if (ProjectileVelocity <= 0)
	{
		TargetFlock = Cast<ACityCenterFlockActor>(TargetFlock);

		if (TargetFlock != nullptr)
		{
			TargetBird = Cast<UStaticMeshComponent>(TargetBird);
			TargetFlock->AddBirdToFlock(TargetBird, this->GetActorLocation());
		}

		Destroy();
	}
}

void ACityCenterProjectile::SetTrackToTarget()
{
	if (ProjectileMovement != nullptr)
	{
		ProjectileMovement->HomingTargetComponent = TargetBird;
		ProjectileMovement->bIsHomingProjectile = true;
	}
}

