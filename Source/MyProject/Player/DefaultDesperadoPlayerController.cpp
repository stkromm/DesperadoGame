// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "DefaultDesperadoPlayerController.h"

void ADefaultDesperadoPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	// voice chat
	InputComponent->BindAction("PushToTalk", IE_Pressed, this, &APlayerController::StartTalking);
	InputComponent->BindAction("PushToTalk", IE_Released, this, &APlayerController::StopTalking);
}

