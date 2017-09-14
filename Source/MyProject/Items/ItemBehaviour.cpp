// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "DefaultDesperadoPlayerController.h"
#include "ItemBehaviour.h"


// Sets default values
AItemBehaviour::AItemBehaviour()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

bool AItemBehaviour::Use_Validate() { return true; }