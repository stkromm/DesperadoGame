// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/CharacterMovementComponent.h"
#include "FirstPersonMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class DESPERADOGAME_API UFirstPersonMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	virtual void InitializeComponent() override;
	virtual float GetMaxSpeed() const override;
};
