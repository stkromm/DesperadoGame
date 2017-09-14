// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbstractInventory.h"
#include "Bag.h"
#include "BagInventory.generated.h"

/**
 *
 */
UCLASS()
class DESPERADOGAME_API ABagInventory : public AAbstractInventory
{
	GENERATED_BODY()
private:
	const int32 NumBagSlots = 5;
	TArray<ABag*> Bags;
public:
	// Constructor
	//
	//
	ABagInventory();
	void BeginPlay() override;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Bags")
	ABag* GetBag(int32 Slot);
	UFUNCTION(Server, WithValidation, Reliable, Category = "Bags")
		virtual void AddBag(ABag* Bag);
	UFUNCTION(Server, WithValidation, Reliable, Category = "Bags")
		virtual void RemoveBag(ABag* Bag);
	UFUNCTION(BlueprintCallable,Server, WithValidation, Reliable, Category = "Bags")
		virtual void SwitchBag(int32 Slot1, int32 Slot2);
	virtual bool CanRemoveBag(ABag* Bag);
	virtual bool CanAddBag(ABag* Bag);
	virtual void ServerAddItem_Implementation(int32 Amount, FName Identifier, AItemBehaviour* Behaviour) override;
	virtual void ServerRemoveItem_Implementation(int32 Amount, FName Identifier, bool DestroyBehaviour) override;
	virtual bool CanRemoveItem(int32 Amount, FName Identifier)  override;
	virtual bool CanAddItem(int32 Amount, FName Identifier) override;
	virtual int32 GetNumOfSlots()  override;
	virtual int32 GetAddableAmount(FName Identifier) override;
	virtual int32 GetAmount(FName Identifier) override;
};
