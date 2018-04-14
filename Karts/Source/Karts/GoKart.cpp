// Fill out your copyright notice in the Description page of Project Settings.

#include "GoKart.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"


// Sets default values
AGoKart::AGoKart()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGoKart::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGoKart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector Force = GetActorForwardVector()* MaxDrivingForce * Throttle;
	Force += GetAirResistance();
	Force += GetRollingResistance();
	FVector Acceleration = Force / Mass;

	float AcceleratuibDueToGravity = -GetWorld()->GetGravityZ() / 100;

	Velocity += Acceleration * DeltaTime;

	ApplyRotation(DeltaTime);
	UpdateLocationFromVelocity(DeltaTime);
}

FVector AGoKart::GetAirResistance()
{
	return - Velocity.GetSafeNormal() * Velocity.SizeSquared() * DragCoefficient;
}

FVector AGoKart::GetRollingResistance()
{
	float AcceleratuibDueToGravity = -GetWorld()->GetGravityZ() / 100;
	float NormalForce = Mass * AcceleratuibDueToGravity;
	return - Velocity.GetSafeNormal() * RollingResistanceCoefficient * NormalForce;
}

void AGoKart::ApplyRotation(float DeltaTime)
{
	float DeltaLocation = FVector::DotProduct(GetActorForwardVector(),Velocity) * DeltaTime;
	float RotationAngle = DeltaLocation / MinTurningRadius * SteeringThrow;
	FQuat RotationDelta(GetActorUpVector(), RotationAngle);
	Velocity = RotationDelta.RotateVector(Velocity);
	AddActorWorldRotation(RotationDelta, true);
}

void AGoKart::UpdateLocationFromVelocity(float DeltaTime)
{
	FVector translation = Velocity * DeltaTime * 100;
	FHitResult hit;
	AddActorWorldOffset(translation, true, &hit);
	if (hit.IsValidBlockingHit())
	{
		Velocity = FVector::ZeroVector;
	}
}

// Called to bind functionality to input
void AGoKart::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AGoKart::Server_MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGoKart::Server_MoveRight);
}

void AGoKart::Server_MoveForward_Implementation(float Value)
{
	Throttle = Value;
}

bool AGoKart::Server_MoveForward_Validate(float Value)
{
	return FMath::Abs(Value)<=1;
}

void AGoKart::Server_MoveRight_Implementation(float Value)
{
	SteeringThrow = Value;
}

bool AGoKart::Server_MoveRight_Validate(float Value)
{
	return FMath::Abs(Value) <= 1;
}
