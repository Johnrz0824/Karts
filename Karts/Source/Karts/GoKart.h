// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GoKart.generated.h"

USTRUCT()
struct FGoKartMove
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY()
		float Throttle;
	UPROPERTY()
		float SteeringThrow;
	UPROPERTY()
		float DeltaTime;
	UPROPERTY()
		float Time;
};

USTRUCT()
struct FGoKartState
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FTransform Transform;
	UPROPERTY()
	FVector Velocity;
	UPROPERTY()
	FGoKartMove LastMove;
};

UCLASS()
class KARTS_API AGoKart : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGoKart();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	FGoKartMove CreateMove(float DeltaTime);
	UPROPERTY(EditAnywhere)
		float Mass = 1000;
	UPROPERTY(EditAnywhere)
		float MaxDrivingForce = 10000;
	UPROPERTY(EditAnywhere)
		float MinTurningRadius = 10;
	UPROPERTY(EditAnywhere)
		float DragCoefficient = 16;
	UPROPERTY(EditAnywhere)
		float RollingResistanceCoefficient = 0.015;
	UFUNCTION(Server, Reliable, WithValidation) 
		void Server_SendMove(FGoKartMove Move);

	void SimulateMove(const FGoKartMove& Move);
	void MoveForward(float Value);
	void MoveRight(float Value);
	void UpdateLocationFromVelocity(float DeltaTime);
	void ApplyRotation(float DeltaTime,float SteeringThrow);
	void ClearAcknowledgedMoves(float LastMoveTime);
	FVector GetAirResistance();
	FVector GetRollingResistance();

	UPROPERTY(ReplicatedUsing = OnRep_ServerState)
	FTransform ReplicatedTransform;
	UFUNCTION()
	void OnRep_ServerState();

	UPROPERTY(Replicated)
	FGoKartState ServerState;

	FVector Velocity;

	float Throttle;
	float SteeringThrow;

	TArray<FGoKartMove> UnacknowledgedMoves;

};
