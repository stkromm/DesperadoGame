// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "DesperadoCharacter.h"
#include "FirstPersonMovementComponent.h"

void UFirstPersonMovementComponent::InitializeComponent(){
	Super::InitializeComponent();
	bCanWalkOffLedgesWhenCrouching = false;
	bCrouchMaintainsBaseLocation = false;
	NavAgentProps.bCanCrouch = true;
	MaxWalkSpeedCrouched = MaxWalkSpeed / 5;
}

float UFirstPersonMovementComponent::GetMaxSpeed() const
{
	float MaxSpeed = Super::GetMaxSpeed();

	ADesperadoCharacter* const  ShooterCharacterOwner = Cast<ADesperadoCharacter>(PawnOwner);
	if (ShooterCharacterOwner)
	{
		if (ShooterCharacterOwner->GetEquipment()->IsTargeting())
		{
			MaxSpeed *= ShooterCharacterOwner->GetTargetingSpeedModifier();
		}
		if (ShooterCharacterOwner->IsRunning())
		{
			MaxSpeed *= ShooterCharacterOwner->GetRunningSpeedModifier();
		}
	}

	return MaxSpeed;
}