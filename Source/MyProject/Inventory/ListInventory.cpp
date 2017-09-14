// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "ListInventory.h"
#include "UnrealNetwork.h"
void AListInventory::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AListInventory, Slots);
	DOREPLIFETIME(AListInventory, Capacity);
	DOREPLIFETIME(AListInventory, UsedCapacity);
}
void AListInventory::ServerRemoveItem_Implementation(int32 Amount, FName Identifier, bool DestroyBehaviour) {
	if (!CanRemoveItem(Amount, Identifier)) return;
	int32 Index = GetIndexByIdentifier(Identifier);
	if (Index == -1) return;
	InventoryAmounts.Add(Identifier, GetAmount(Identifier) - Amount);
	FItemAmount Item = Slots[Index];
	Item.Amount -= Amount;
	if (Item.Amount == 0) {
		Slots.RemoveAt(Index,1,true);
	}
}

void AListInventory::ServerAddItem_Implementation(int32 Amount, FName Identifier, AItemBehaviour* Behaviour) {
	if (!CanAddItem(Amount, Identifier)) return;
	int32 Index = GetIndexByIdentifier(Identifier);
	if (Index == -1) return;
	InventoryAmounts.Add(Identifier, GetAmount(Identifier) + Amount);
	FItemAmount Item = Slots[Index];
	Item.Amount += Amount;
}

bool AListInventory::CanAddItem(int32 Amount, FName Identifier) {
	return Amount <= Capacity - UsedCapacity;
}

bool AListInventory::CanRemoveItem(int32 Amount, FName Identifier)  {
	return GetAmount(Identifier) >= Amount;
}

int32 AListInventory::GetAddableAmount(FName Identifier) {
	return Capacity - UsedCapacity;
}

int32 AListInventory::GetAmount(FName Identifier) {
	switch (Role) {
	case ENetRole::ROLE_Authority:
		return InventoryAmounts.FindRef(Identifier);
	default:
		int32 Index = GetIndexByIdentifier(Identifier);
		if (Index == -1) return 0;
		return Slots[Index].Amount;
	}
}

int32 AListInventory::GetNumOfSlots() {
	return Slots.Num();
}

TArray<FName> AListInventory::GetStoredItemNames() {
	TArray<FName> ReturnArray = TArray<FName>();
	for (const FItemAmount& Item : Slots) {
		ReturnArray.Add(Item.Identifier);
	}
	return ReturnArray;
}

int32 AListInventory::GetIndexByIdentifier(FName Identifier) {
	for (int32 b = 0; b<Slots.Num(); b++) {
		if (Slots[b].Identifier.IsEqual(Identifier)) return b;
	}return -1;
}