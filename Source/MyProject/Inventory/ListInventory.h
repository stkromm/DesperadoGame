// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbstractInventory.h"
#include "ItemAmount.h"
#include "ListInventory.generated.h"

/**
 *
 */
UCLASS()
class DESPERADOGAME_API AListInventory : public AAbstractInventory
{
	GENERATED_BODY()
protected:
	UPROPERTY(Replicated)
		TArray<FItemAmount> Slots;

	TMap<FName, int32> InventoryAmounts;
	int32 GetIndexByIdentifier(FName Identifier);
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, SimpleDisplay, Replicated, Category = "Desperado,Inventory", Meta = (DisplayName = "Capacity", ExposeOnSpawn = true))
		int32 Capacity = 1000;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, SimpleDisplay, Replicated, Category = "Desperado,Inventory", Meta = (DisplayName = "Capacity", ExposeOnSpawn = true))
		int32 UsedCapacity;
	virtual void ServerAddItem_Implementation(int32 Amount, FName Identifier, AItemBehaviour* Behaviour) override;
	virtual void ServerRemoveItem_Implementation(int32 Amount, FName Identifier, bool DestroyBehaviour) override;
	// Pure Methods
	virtual bool CanRemoveItem(int32 Amount, FName Identifier)  override;
	virtual bool CanAddItem(int32 Amount, FName Identifier) override;
	virtual int32 GetNumOfSlots()  override;
	virtual int32 GetAddableAmount(FName Identifier) override;
	virtual int32 GetAmount(FName Identifier) override;
	UFUNCTION(BlueprintCallable, Category = "Inventory", BlueprintPure)
		virtual TArray<FName> GetStoredItemNames();
};
