// Fill out your copyright notice in the Description page of Project Settings.


#include "CityCenter/Flocks/CityCenterFlockActor.h"
#include "Math/UnrealMathVectorCommon.h"
#include "CityCenter/CityCenterProjectile.h"
#include "Components/CapsuleComponent.h"
// Sets default values
ACityCenterFlockActor::ACityCenterFlockActor()
{
	Root = CreateDefaultSubobject<USceneComponent>(FName(TEXT("Root")));
	SetRootComponent(Root);

	for (int32 i = 0; i < QuantityBirdsInFlock; ++i)
	{
		UStaticMeshComponent* StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName(FString::Printf(TEXT("bird_%d"), i)));
		BirdsInFlock.Add(StaticMesh);
		VelocityFlock.Add(FVector::ZeroVector);
	}

	StaticMesh_01 = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("MeshTest")));
	StaticMesh_01->SetupAttachment(Root);

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("TriggerCapsule"));
	TriggerCapsule->InitCapsuleSize(200.f, 200.f);
	TriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	TriggerCapsule->SetupAttachment(RootComponent);

	TriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &ACityCenterFlockActor::OnOverlapBegin);
}

// Called when the game starts or when spawned
void ACityCenterFlockActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACityCenterFlockActor::Tick(float DeltaTime)
{
	MoveFlockToNewLocation(DeltaTime);
	Super::Tick(DeltaTime);
}

void ACityCenterFlockActor::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ChangeFlightFlockDirection();
}

void ACityCenterFlockActor::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, TEXT("DZIA£AJ TY KURWA GNOJU JEBANY"));
	bool FallBack = true;
}

void ACityCenterFlockActor::MoveFlockToNewLocation(float DeltaTime)
{
		FVector v1, v2, v3, v4, v5;
		UStaticMeshComponent* Bird;

		for (auto i = 0; i < BirdsInFlock.Num(); i++)
		{
			Bird = BirdsInFlock[i]; 
			v1 = FlockFocus * SetPositionCenterNeighboringBirds(Bird);
			v2 = AvoidingObstacles * SetAwaySpace(Bird);
			v3 = SetMeshVelocity(Bird);
			v4 = SetBoundLocation(Bird);

			FVector LocBird = Bird->GetComponentLocation();
			VelocityFlock[i] = VelocityFlock[i] + v1 + v2 + v3 + v4;

			VelocityFlock[i] = SetLimitVelocity(VelocityFlock[i]);

			LocBird = LocBird + VelocityFlock[i] * DeltaTime;

			Bird->SetWorldLocation(LocBird);
			TriggerCapsule->SetWorldLocation(LocBird);
		}	
}

FVector ACityCenterFlockActor::SetPositionCenterNeighboringBirds(UStaticMeshComponent* ActualyMesh)
{
	FVector NewPositionVector = FVector::ZeroVector;
	int32 NumberOfBirds = BirdsInFlock.Num();

	for (auto i = 0; i < NumberOfBirds; i++)
	{
		if (BirdsInFlock[i] != ActualyMesh)
		{
			NewPositionVector = NewPositionVector + BirdsInFlock[i]->GetComponentLocation();
		}
	}
	NewPositionVector = NewPositionVector / (NumberOfBirds - 1);
	return (NewPositionVector - ActualyMesh->GetComponentLocation()) / 100;
}

FVector ACityCenterFlockActor::SetAwaySpace(UStaticMeshComponent* Bird)
{
	FVector SpaceVector = FVector::ZeroVector;

	for (auto i = 0; i < BirdsInFlock.Num(); i++)
	{
		if (Bird != BirdsInFlock[i])
		{
			FVector LocationBird = BirdsInFlock[i]->GetComponentLocation();
			FVector LocationOur = Bird->GetComponentLocation();

			int32 CurrDiffX = FMath::Abs(LocationBird.X - LocationOur.X);
			int32 CurrDiffY = FMath::Abs(LocationBird.Y - LocationOur.Y);
			int32 CurrDiffZ = FMath::Abs(LocationBird.Z - LocationOur.Z);

			if (VectorAllLesserThan(MakeVectorRegister(CurrDiffX, CurrDiffY, CurrDiffZ, 0.0f), MakeVectorRegister(SafeDistance.X, SafeDistance.Y, SafeDistance.Z, 1.0f)))
			{
				SpaceVector = SpaceVector - (BirdsInFlock[i]->GetComponentLocation() - Bird->GetComponentLocation());
			}
		}
	}
	return SpaceVector;
}

FVector ACityCenterFlockActor::SetMeshVelocity(UStaticMeshComponent* Bird)
{
	FVector PerceivedVelocity = FVector::ZeroVector;
	FVector Velocity = FVector::ZeroVector;

	int32 NumberOfBirds = BirdsInFlock.Num();

	for (auto i = 0; i < NumberOfBirds; i++)
	{

		if (Bird != BirdsInFlock[i])
		{
			PerceivedVelocity = PerceivedVelocity + VelocityFlock[i];
		}
		else
		{
			Velocity = VelocityFlock[i];
		}
	}
	PerceivedVelocity = PerceivedVelocity / (NumberOfBirds - 1);
	return (PerceivedVelocity - Velocity) / 10;
}

FVector ACityCenterFlockActor::SetBoundLocation(UStaticMeshComponent* StaticMesh)
{
	int32 BackTheWay = 20;

	FVector BoundLocationVector = FVector::ZeroVector;

	FVector MaxVector = (MaxFlockDistance + this->GetActorLocation());
	FVector MinVector = (MinFlockDistance + this->GetActorLocation());

	FVector BirdPosition = StaticMesh->GetComponentLocation();

	if (BirdPosition.X < MinVector.X)
	{
		BoundLocationVector.X += BackTheWay;
	}
	else if (BirdPosition.X > MaxVector.X)
	{
		BoundLocationVector.X += -BackTheWay;
	}

	if (BirdPosition.Y < MinVector.Y)
	{
		BoundLocationVector.Y += BackTheWay;
	}
	else if (BirdPosition.Y > MaxVector.Y)
	{
		BoundLocationVector.Y += -BackTheWay;
	}

	if (BirdPosition.Z < MinVector.Z)
	{
		BoundLocationVector.Z += BackTheWay;
	}
	else if (BirdPosition.Z > MaxVector.Z)
	{
		BoundLocationVector.Z += -BackTheWay;
	}
	return BoundLocationVector;
}

FVector ACityCenterFlockActor::SetLimitVelocity(FVector Velocity)
{
	FVector NewVelocity = FVector::ZeroVector;

	int32 CurrDiffX = Velocity.X;
	int32 CurrDiffY = Velocity.Y;
	int32 CurrDiffZ = Velocity.Z;

	int32 Loc = FMath::Sqrt(CurrDiffX * CurrDiffX + CurrDiffY * CurrDiffY + CurrDiffZ * CurrDiffZ);

	if (Velocity.Size() > LimitVelocity)
	{
		NewVelocity = Velocity.GetSafeNormal() * LimitVelocity;
	}
	else
	{
		NewVelocity = Velocity;
	}

	return NewVelocity;
}

void ACityCenterFlockActor::HitFlock(UStaticMeshComponent* Flock)
{
	int32 NumberOfBirds = BirdsInFlock.Num();

	for (auto i = 0; i < NumberOfBirds; i++)
	{
		if (BirdsInFlock[i] == Flock)
		{
			BirdsInFlock[i]->SetVisibility(false);

			FlockFocus = FlockDispersion;
			ChangeFlightFlockDirection();

			FTimerHandle TimerHandle;
			GetWorldTimerManager().ClearTimer(TimerHandle);
			GetWorldTimerManager().SetTimer(TimerHandle, this, &ACityCenterFlockActor::ChangeFlockDispersion, 1.f, true, 3.f);
		}
	}
}

void ACityCenterFlockActor::AddBirdToFlock(UStaticMeshComponent* Flock, FVector Position)
{
	int32 NumberOfBirds = BirdsInFlock.Num();

	for (auto i = 0; i < NumberOfBirds; i++)
	{
		if (BirdsInFlock[i] == Flock)
		{
			BirdsInFlock[i]->SetWorldLocation(Position);
			BirdsInFlock[i]->SetVisibility(true);

		}
	}
}

void ACityCenterFlockActor::ChangeFlockDispersion()
{
		FlockFocus = FlockFocusDefault;
}

void ACityCenterFlockActor::ChangeFlightFlockDirection()
{
	int32 FlyForward = 1;
	int32 CurrentDirection = AvoidingObstacles;

	if (CurrentDirection == FlyForward)
	{
		CurrentDirection = -FlyForward;
	}
	else if (CurrentDirection == -FlyForward)
	{
		CurrentDirection = FlyForward;
	}
}
