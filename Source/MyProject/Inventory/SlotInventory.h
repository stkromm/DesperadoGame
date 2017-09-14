// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbstractInventory.h"
#include "ItemAmount.h"
#include "SlotInventory.generated.h"

/**
 *
 */
UCLASS(BlueprintType, advancedclassdisplay)
class DESPERADOGAME_API ASlotInventory : public AAbstractInventory
{
	GENERATED_BODY()

protected:
	/*
	Replication
	*/


	TMap<FName, int32> InventoryAmounts;
	UDataTable* ItemDatabase;
public:
	/*
	Constructor Variables
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, SimpleDisplay, Replicated, Category = "Desperado,Inventory", Meta = (DisplayName = "Number of Slots", ExposeOnSpawn = true))
		int32 NumOfSlots = 5;
	UPROPERTY(Replicated)
		TArray<FItemAmount> Slots;
	/*
	Constructor
	*/
	ASlotInventory();
	// Server Only Methods
	UFUNCTION(BlueprintCallable, Server, WithValidation, Unreliable, Category = "Desperado, Inventory")
		virtual void SwapSlotsIntern(int32 Slot1, int32 Slot2);
	UFUNCTION(BlueprintCallable, Server, WithValidation, Unreliable, Category = "Desperado, Inventory")
		virtual void SwapSlotsExtern(int32 Slot1, ASlotInventory* ExternInventory, int32 Slot2);
	UFUNCTION(BlueprintCallable, Server, WithValidation, Unreliable, Category = "Desperado, Inventory")
		virtual void SplitAmountFromTo(int32 Slot1, int32 Amount1, ASlotInventory* ExternInventory, int32 Slot2);
	UFUNCTION(BlueprintCallable, Server, WithValidation, Reliable, Category = "Desperado,Inventory")
		virtual void SetIndex(int32 Slot, FItemAmount Item);
	//
	virtual void ServerAddItem_Implementation(int32 Amount, FName Identifier, AItemBehaviour* Behaviour) override;
	virtual void ServerRemoveItem_Implementation(int32 Amount, FName Identifier, bool DestroyBehaviour) override;
	// Pure Methods
	virtual bool CanRemoveItem(int32 Amount, FName Identifier)  override;
	virtual bool CanAddItem(int32 Amount, FName Identifier) override;
	// 
	UFUNCTION(BlueprintCallable, Category = "Desperado,Inventory", BlueprintPure)
		virtual FItemAmount GetItem(int32 Slot);
	UFUNCTION(BlueprintCallable, Category = "Desperado,Inventory", BlueprintPure)
		virtual int32 GetFreeSlot();
	//
	virtual bool IsEmpty();
	virtual int32 GetNumOfSlots()  override;
	virtual int32 GetAddableAmount(FName Identifier) override;
	virtual int32 GetAmount(FName Identifier) override;
};
