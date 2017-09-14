// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ItemBehaviour.generated.h"

UCLASS()
class DESPERADOGAME_API AItemBehaviour : public AActor
{
	GENERATED_BODY()
public:
	// Constructors
	//
	//
	AItemBehaviour();
	// Properties
	//
	//
	FName Identifier;
	// Getter
	//
	//
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory, Behaviour")
		virtual FText GetUseHint()
	{
		check(0 && "you must override this");
		return FText::FromString("");
	}
	// Server Methods
	//
	//
	UFUNCTION(BlueprintCallable, Server, WithValidation, Reliable, Category = "Inventory, Behaviour")
		void Use();
	virtual void Use_Implementation(){
		check(0 && "you must override this");
	}
};
