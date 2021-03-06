// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "BagInventory.h"
#include "DesperadoPlayerState.generated.h"

/**
 *
 */
UCLASS()
class DESPERADOGAME_API ADesperadoPlayerState : public APlayerState
{

	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Replicated)
	ABagInventory* Inventory = nullptr;
	ADesperadoPlayerState();
	virtual void BeginPlay() override;

};
