// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "DefaultDesperadoPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API ADefaultDesperadoPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual bool IsGameInputAllowed() { return true; }

#pragma region Override
	/** sets up input */
	virtual void SetupInputComponent() override;
#pragma endregion Override
};
