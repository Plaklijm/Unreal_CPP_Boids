// Fill out your copyright notice in the Description page of Project Settings.

#include "Boids/Public/BoidMain.h"
#include "Boids/Public/Boid.h"

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
		ABoid* boidToAdd = GetWorld()->SpawnActor<ABoid>();
		BoidList.Add(boidToAdd);
	}
	
	// Init Positions()
	
}

// Called every frame
void ABoidMain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// update boid positions

	// FVector3d Stores in doubles, so it is higher in accuracy (more memory but worth it for Position Data)
	FVector3d v1,v2,v3;

	// Foreach loop in C++
	for (auto b : BoidList)
	{
		v1 = CalcCohesion(b);
		v2 = CalcSeperation(b);
		v3 = CalcAlignment(b);

		b->SetVelocity(v1 + v2 + v3);
	}
}

void ABoidMain::UpdateBoidPosition()
{
	FVector3f v1,v2,v3;
	ABoid b;
}

FVector3d ABoidMain::CalcSeperation(ABoid* currentBoid)
{
	FVector3d c = FVector3d::ZeroVector;
	
	for (auto b : BoidList)
	{
		if (b != currentBoid)
		{
			// Using DistSquared for a bit of performance optimization (we dont need it to be squared to know the dist)
			//if (FVector3d::DistSquared(b->GetPosition(), currentBoid->GetPosition()) < 250)
			{
				//c -= b->GetPosition() - currentBoid->GetPosition();
			}
		}
	}

	return c;
}

FVector3d ABoidMain::CalcAlignment(ABoid* currentBoid)
{
	FVector3d PercievedVelocity;

	for (auto b : BoidList)
	{
		if (b != currentBoid)
		{
			PercievedVelocity += b->GetVelocity();
		}
	}

	PercievedVelocity /= BoidList.Num() - 1;

	return (PercievedVelocity - currentBoid->GetVelocity()) / 8;
}

FVector3d ABoidMain::CalcCohesion(ABoid* currentBoid)
{
	FVector3d PercievedCenter;

	for (auto b : BoidList)
	{
		if (b != currentBoid)
		{
			// Calculate PercievedCenter by adding all the positions of the boids minus the current one
			//PercievedCenter += currentBoid->GetPosition();
		}
	}

	PercievedCenter = PercievedCenter / BoidList.Num() - 1;

	// Move
	//return (PercievedCenter - currentBoid->GetPosition()) / 100;
	return FVector3d::ZeroVector;
}

