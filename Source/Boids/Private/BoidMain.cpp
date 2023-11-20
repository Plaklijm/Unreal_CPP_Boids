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
		boidToAdd->SetVelocity(FMath::VRand() * SpeedMultiplier);
		UE_LOG(LogTemp, Warning, TEXT("RandVel: %s"), *boidToAdd->GetVelocity().ToString());
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
	auto v1 = FVector::ZeroVector,
		v2 = FVector::ZeroVector,
		v3 = FVector::ZeroVector,
		v4 = FVector::ZeroVector;

	// Foreach loop in C++
	for (auto b : BoidList)
	{
		// Calculate forces
		v3 = CalcSeperation(b);
		v1 = CalcAlignment(b);
		v2 = CalcCohesion(b);
		v4 = CalcReturnVector(b);
		
		// Set the velocity
		b->SetVelocity(b->GetVelocity() + v1 + v2 + v3 + v4);
		// Clamp velocity
		auto ClampedVel = (b->GetVelocity() / b->GetVelocity().Length()) * MaxSpeed;
		// Calculate new position
		auto NewLocation = b->GetActorLocation() + ClampedVel * DeltaTime * SpeedMultiplier;
		b->SetActorLocation(NewLocation);
	}
}

FVector3d ABoidMain::CalcSeperation(ABoid* currentBoid)
{
	auto c = FVector3d::ZeroVector;
	
	for (const auto b : BoidList)
	{
		if (b != currentBoid)
		{
			if (FVector3d::Dist(b->GetActorLocation(), currentBoid->GetActorLocation()) < ProtectedRange)
			{
				if (EnableDebugView)
				{
					DrawDebugLine(GetWorld(), b->GetActorLocation(), currentBoid->GetActorLocation(), FColor::Red);
				}
				c += currentBoid->GetActorLocation() - b->GetActorLocation();
			}
		}
	}

	// return nothing if there arent neighbours to keep the vel
	return FVector3d::ZeroVector;
}

FVector3d ABoidMain::CalcAlignment(ABoid* currentBoid)
{
	auto PercievedVelocity = FVector3d::ZeroVector;
	int NeighbourCount = 0;
	
	for (auto b : BoidList)
	{
		if (b != currentBoid)
		{
			if (FVector3d::Dist(b->GetActorLocation(), currentBoid->GetActorLocation()) < VisualRange)
			{
				// Calculate PercievedVelocity by adding all the velocities of the boids minus the current one
				PercievedVelocity = PercievedVelocity + b->GetVelocity();
				NeighbourCount += 1;
			}
		}
	}

	if (NeighbourCount > 0)
	{
		PercievedVelocity = PercievedVelocity / NeighbourCount;
		return (PercievedVelocity - currentBoid->GetVelocity()) * MatchFactor;
	}

	// return nothing if there arent neighbours to keep the vel
	return FVector3d::ZeroVector;
}

FVector3d ABoidMain::CalcCohesion(ABoid* currentBoid)
{
	auto PercievedCenter = FVector::ZeroVector;
	int NeighbourCount = 0;

	for (auto b : BoidList)
	{
		if (b != currentBoid)
		{
			if (FVector3d::Dist(b->GetActorLocation(), currentBoid->GetActorLocation()) < VisualRange)
			{
				if (EnableDebugView)
				{
					DrawDebugLine(GetWorld(), b->GetActorLocation(), currentBoid->GetActorLocation(), FColor::Green);
				}
				// Calculate PercievedCenter by adding all the positions of the boids minus the current one
				PercievedCenter = PercievedCenter + b->GetActorLocation();
				NeighbourCount += 1;
			}
		}
	}

	if (NeighbourCount > 0)
	{
		// Devide the percieved center with the neighbourcount to get the average position
		PercievedCenter = PercievedCenter / NeighbourCount;
		
		return (PercievedCenter - currentBoid->GetActorLocation()) * CenteringFactor;
	}

	// return nothing if there arent neighbours to keep the vel
	return FVector3d::ZeroVector;
}

FVector3d ABoidMain::CalcReturnVector(ABoid* currentBoid)
{
	// The min value is origin - extent, max is origin + extent.
	const auto min = this->GetActorLocation() - BoundingBoxExtends;
	const auto max = this->GetActorLocation() + BoundingBoxExtends;
	auto ReturnVec = FVector::ZeroVector;

	// Check to see if the boid has exceeded the bounds of the box
	if (currentBoid->GetActorLocation().X < min.X)
	{
		ReturnVec.X = ReturnSpeed;
	} else if (currentBoid->GetActorLocation().X > max.X)
	{
		ReturnVec.X = -ReturnSpeed;
	}

	if (currentBoid->GetActorLocation().Y < min.Y)
	{
		ReturnVec.Y = ReturnSpeed;
	} else if (currentBoid->GetActorLocation().Y > max.Y)
	{
		ReturnVec.Y = -ReturnSpeed;
	}

	if (currentBoid->GetActorLocation().Z < min.Z)
	{
		ReturnVec.Z = ReturnSpeed;
	} else if (currentBoid->GetActorLocation().Z > max.Z)
	{
		ReturnVec.Z = -ReturnSpeed;
	}

	// Returns Zero if the boid is contained in the box
	return ReturnVec;
}