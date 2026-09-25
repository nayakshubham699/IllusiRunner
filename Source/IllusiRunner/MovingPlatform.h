// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingPlatform.generated.h"

UCLASS()
class ILLUSIRUNNER_API AMovingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovingPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	void MovingPlatform(float DeltaTime);

	void RotatingPlatform(float DeltaTime);

	float GetDistanceMoved();

	UPROPERTY(EditAnywhere, Category = "Movement")
	FVector PlatformVelocity = FVector(0.0, 0.0, 0.0);

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MoveDistance;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	float DistancedMove;

	UPROPERTY(EditAnywhere, Category = "Movement")
	FRotator RotationVelocity;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	FVector StartLocation;
};
