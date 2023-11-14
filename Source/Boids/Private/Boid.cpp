// Fill out your copyright notice in the Description page of Project Settings.


#include "Boids/Public/Boid.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABoid::ABoid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Root
	// (initialized here because it is a pure C++ project, nothing will be done in Blueprints)
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Root->SetMobility(EComponentMobility::Movable);
	// Set as the RootComponent, this will be the main Transform of the Boid
	RootComponent = Root;
	

	// Create a MeshComponent and attach it to the RootComponent
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoidMesh"));
	Mesh->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void ABoid::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABoid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

