// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

// Sets default values
AMovingPlatform::AMovingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	
	StartLocation = GetActorLocation();

	FString Name = GetName();

	UE_LOG(LogTemp, Display, TEXT("My Actor Namae = %s"), *Name);
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FVector CurrentLocation = GetActorLocation();

	CurrentLocation += PlatformVelocity * DeltaTime;

	// Actor의 위치를 Set
	SetActorLocation(CurrentLocation);

	float DistanceMoved = FVector::Dist(StartLocation,CurrentLocation);

	if(DistanceMoved > MoveDistance)
	{
		FVector MoveDir = PlatformVelocity.GetSafeNormal();
		StartLocation += MoveDir * MoveDistance;
		SetActorLocation(StartLocation);
		PlatformVelocity = -PlatformVelocity;
	}

}

