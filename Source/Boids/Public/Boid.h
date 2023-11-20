// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boid.generated.h"

UCLASS()
class BOIDS_API ABoid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoid();
	~ABoid() = default;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	//FVector3d GetPosition() { return GetPosition(); }

	void SetVelocity(const FVector3d& newVelocity) const { Root->ComponentVelocity = newVelocity; }

public:
	UPROPERTY()
	USceneComponent* Root;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;
};
