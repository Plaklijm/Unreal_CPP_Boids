// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoidMain.generated.h"

class ABoid;

UCLASS()
class BOIDS_API ABoidMain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoidMain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	int NumOfBoids;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABoid> BoidObject;

	UPROPERTY(EditAnywhere)
	FVector3d BoundingBoxExtends;

	UPROPERTY(EditAnywhere)
	float SpeedMultiplier;

	UPROPERTY(EditAnywhere)
	float ProtectedRange;

	UPROPERTY(EditAnywhere)
	float VisualRange;

	UPROPERTY(EditAnywhere)
	float AvoidFactor;

	UPROPERTY(EditAnywhere)
	float MatchFactor;

	UPROPERTY(EditAnywhere)
	float CenteringFactor;

	UPROPERTY(EditAnywhere)
	float MaxSpeed;
	
	UPROPERTY(VisibleAnywhere)
	TArray<ABoid*> BoidList;
	
private:
	UFUNCTION()
	FVector3d CalcSeperation(ABoid* currentBoid);

	UFUNCTION()
	FVector3d CalcAlignment(ABoid* currentBoid);

	UFUNCTION()
	FVector3d CalcCohesion(ABoid* currentBoid);

	UFUNCTION()
	FVector3d CalcReturnVector(ABoid* currentBoid);
};
