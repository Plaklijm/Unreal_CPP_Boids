// Fill out your copyright notice in the Description page of Project Settings.

#include "Boids/Public/BoidMain.h"
#include "Boids/Public/Boid.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABoidMain::ABoidMain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABoidMain::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < NumOfBoids; ++i)
	{
		// Spawn a new Boid inside a boundingbox of the BoidObject type (this is the blueprint object set in the inspector)
		ABoid* boidToAdd = GetWorld()->SpawnActor<ABoid>(BoidObject);
		boidToAdd->SetActorLocation(UKismetMathLibrary::RandomPointInBoundingBox(GetActorLocation(), BoundingBoxExtends));
		boidToAdd->SetVelocity(FMath::VRand() * 10);
		BoidList.Add(boidToAdd);
	}
}

// Called every frame
void ABoidMain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Draw the BoundingBox
	DrawDebugBox(GetWorld(), GetActorLocation(), BoundingBoxExtends, FColor::Red);

	// update boid positions
	// FVector3d Stores in doubles, so it is higher in accuracy (more memory but worth it for Position Data)
	FVector3d v1 = FVector::ZeroVector, v2 = FVector::ZeroVector, v3 = FVector::ZeroVector, v4 = FVector::ZeroVector;

	// Foreach loop in C++
	for (auto b : BoidList)
	{
		v1 = CalcAlignment(b);
		v2 = CalcCohesion(b);
		v3 = CalcSeperation(b);
		v4 = CalcReturnVector(b);


		b->SetVelocity(b->GetVelocity() + v1 + v2 + v3);
		FVector3d ClampedVel = (b->GetVelocity() / b->GetVelocity().Length()) * MaxSpeed;
		FVector3d Location = b->GetActorLocation() + ClampedVel * DeltaTime * SpeedMultiplier;
		b->SetActorLocation(Location);
	}
}

FVector3d ABoidMain::CalcSeperation(ABoid* currentBoid)
{
	FVector3d c = FVector3d::ZeroVector;
	
	for (const auto b : BoidList)
	{
		if (b != currentBoid)
		{
			if (FVector3d::Dist(b->GetActorLocation(), currentBoid->GetActorLocation()) < ProtectedRange)
			{
				DrawDebugLine(GetWorld(), b->GetActorLocation(), currentBoid->GetActorLocation(), FColor::Green);
				c += currentBoid->GetActorLocation() - b->GetActorLocation();
			}
		}
	}

	return c * AvoidFactor;
}

FVector3d ABoidMain::CalcAlignment(ABoid* currentBoid)
{
	FVector3d PercievedVelocity = FVector::ZeroVector;
	int NeighbourCount = 0;
	
	for (auto b : BoidList)
	{
		if (b != currentBoid)
		{
			if (FVector3d::Dist(b->GetActorLocation(), currentBoid->GetActorLocation()) < VisualRange)
			{
				PercievedVelocity = PercievedVelocity + b->GetVelocity();
				NeighbourCount += 1;
			}
		}
	}

	if (NeighbourCount > 0)
	{
		PercievedVelocity = PercievedVelocity / NeighbourCount;
	}

	return (PercievedVelocity - currentBoid->GetVelocity()) * MatchFactor;
}

FVector3d ABoidMain::CalcCohesion(ABoid* currentBoid)
{
	FVector3d PercievedCenter = FVector::ZeroVector;
	int NeighbourCount = 0;

	for (auto b : BoidList)
	{
		if (b != currentBoid)
		{
			if (FVector3d::Dist(b->GetActorLocation(), currentBoid->GetActorLocation()) < VisualRange)
			{
				// Calculate PercievedCenter by adding all the positions of the boids minus the current one
				PercievedCenter = PercievedCenter + b->GetActorLocation();
				NeighbourCount += 1;
			}
		}
	}

	if (NeighbourCount > 0)
	{
		PercievedCenter = PercievedCenter/NeighbourCount;
	}

	PercievedCenter = PercievedCenter / BoidList.Num() - 1;

	// Move
	return (PercievedCenter - currentBoid->GetActorLocation()) * CenteringFactor;
}

FVector3d ABoidMain::CalcReturnVector(ABoid* currentBoid)
{
	// The min value is origin - extent, max is origin + extent.
	FVector3f min = GetActorLocation() - BoundingBoxExtends;
	UKismetMathLibrary::MakeBox(, GetActorLocation() + BoundingBoxExtends);
	FVector3d ReturnVec;

	if (currentBoid->GetActorLocation().X )
	{
		
	}
		
	return ReturnVec;
}

