// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "SlotInventory.h"
#include "AbstractInventory.h"
#include "ItemStruct.h"
#include "UnrealNetwork.h"


ASlotInventory::ASlotInventory()
{
	PrimaryActorTick.bCanEverTick = false;
	ConstructorHelpers::FObjectFinder<UDataTable> ItemTable_BP(TEXT("DataTable'/Game/Database/C++Referenced/BasicItemDatabase.BasicItemDatabase'"));
	ItemDatabase = ItemTable_BP.Object;
	bReplicates = true;
	for (int32 b = 0; b < NumOfSlots; b++)
	{
		Slots.Add(FItemAmount());
	}
}

void ASlotInventory::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASlotInventory, Slots);
	DOREPLIFETIME(ASlotInventory, NumOfSlots);
}

void ASlotInventory::ServerRemoveItem_Implementation(int32 Amount, FName Identifier, bool DestroyBehaviour)
{
	InventoryAmounts.Add(Identifier, GetAmount(Identifier) - Amount);
	// Update Slots
	int32 RestAmount = Amount;
	int32 b = 0;
	auto FilteredSlots = Slots.FilterByPredicate([](const FItemAmount& It){ return It.Identifier.IsEqual(Identifier) && It.Amount != 0; });
	while (RestAmount > 0 && b < FilteredSlots.Num())
	{
		RestAmount = Slots[b].RemoveAmount(RestAmount);
		b++;
	}
}

void ASlotInventory::ServerAddItem_Implementation(int32 Amount, FName Identifier, AItemBehaviour* Behaviour)
{
	InventoryAmounts.Add(Identifier, Amount + GetAmount(Identifier));
	//
	FItemStruct* RowLookUp = ItemDatabase->FindRow<FItemStruct>(Identifier, TEXT("LookUp Operation"));
	if (!RowLookUp)
	{
		return;
	}
	int32 RestAmount = Amount;
	int32 b = 0;
	auto FilteredSlots = Slots.FilterByPredicate([](const FItemAmount& It){ return (It.Identifier.IsEqual(Identifier)) || It.Amount == 0; });
	while (RestAmount > 0 && b < FilteredSlots.Num())
	{
		RestAmount = Slots[b].AddAmount(RestAmount, RowLookUp->Stacksize);
		if (Slots[b].Behaviour || (RowLookUp->Class == AItemBehaviour::StaticClass()))
		{
			continue;
		}
		if (Behaviour)
		{
			Slots[b].Behaviour = Behaviour;
			Behaviour->SetOwner(this);
		}
		else
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			Slots[b].Behaviour = GetWorld()->SpawnActor<AItemBehaviour>(RowLookUp->Class, SpawnParams);
		}
		Slots[b].Behaviour->Identifier = Slots[b].Identifier;
		b++;
	}
}

bool ASlotInventory::CanAddItem(int32 Amount, FName Identifier)
{
	return Amount <= GetAddableAmount(Identifier);
}

bool ASlotInventory::CanRemoveItem(int32 Amount, FName Identifier)
{
	return !FItemAmount::IsValidAmount(Amount) || !FItemAmount::IsValidIdentifier(Identifier) ? false : (GetAmount(Identifier) >= Amount);
}

int32 ASlotInventory::GetAddableAmount(FName Identifier)
{
	FItemStruct* RowLookUp = ItemDatabase->FindRow<FItemStruct>(Identifier, TEXT("LookUp Operation"));
	if (!RowLookUp)
	{
		return false;
	}
	int32 AddableSlots = Slots.FilterByPredicate([](const FItemAmount& It){ return It.CanAddAmount(Identifier); }).Num();
	return AddableSlots*(RowLookUp->Stacksize) - GetAmount(Identifier);
}

int32 ASlotInventory::GetAmount(FName Identifier) {

	switch (Role) {
	case ENetRole::ROLE_Authority:
		return InventoryAmounts.FindRef(Identifier);
	default:
		int32 ReturnAmount = 0;
		for (const FItemAmount& Item : Slots) {
			if (Item.Identifier.IsEqual(Identifier)) {
				ReturnAmount += Item.Amount;
			}
		}
		return ReturnAmount;
	}

}

int32 ASlotInventory::GetFreeSlot() {
	return Slots.Find(FItemAmount());
}

int32 ASlotInventory::GetNumOfSlots() {
	return Slots.Num();
}

FItemAmount ASlotInventory::GetItem(int32 Slot) {
	return Slots.IsValidIndex(Slot) ? Slots[Slot] : FItemAmount();
}

void ASlotInventory::SwapSlotsIntern_Implementation(int32 Slot1, int32 Slot2) {
	if (!Slots.IsValidIndex(Slot1) || !Slots.IsValidIndex(Slot2)) return;
	FItemAmount Temp = GetItem(Slot1);
	FItemAmount Temp2 = GetItem(Slot2);
	if (!Temp.Identifier.IsEqual(Temp2.Identifier)) {
		Slots.Swap(Slot1, Slot2);
	}
	else {
		FItemStruct* RowLookUp = ItemDatabase->FindRow<FItemStruct>(Temp.Identifier, TEXT("LookUp Operation"));
		if (RowLookUp)
		{
			Temp.Amount = Temp2.AddAmount(Temp.Amount, RowLookUp->Stacksize);
			if (Temp.Amount == 0) Temp = FItemAmount();
			SetIndex(Slot1, Temp2);
			SetIndex(Slot2, Temp);
		}
	}
}
bool ASlotInventory::SwapSlotsIntern_Validate(int32 Slot1, int32 Slot2) {
	return true;
}
void ASlotInventory::SetIndex_Implementation(int32 Slot, FItemAmount Item) {
	if (!Slots.IsValidIndex(Slot)) return;
	FItemAmount ItemOnIndex = Slots[Slot];
	InventoryAmounts.Add(ItemOnIndex.Identifier, GetAmount(ItemOnIndex.Identifier) - ItemOnIndex.Amount);
	InventoryAmounts.Add(Item.Identifier, GetAmount(Item.Identifier) + Item.Amount);
	if (Item.Amount != 0) 
	{
		Slots[Slot] = Item;
	}
	else 
	{
		Slots[Slot] = FItemAmount();
	}
}

bool ASlotInventory::SetIndex_Validate(int32 Slot, FItemAmount Item) 
{
	return true;
}
bool ASlotInventory::SwapSlotsExtern_Validate(int32 Slot1, ASlotInventory* ExternInventory, int32 Slot2) 
{ 
	return true; 
}

void ASlotInventory::SwapSlotsExtern_Implementation(int32 Slot1, ASlotInventory* ExternInventory, int32 Slot2) 
{
	if (!ExternInventory || !(ExternInventory->GetOwner() == GetOwner()) || !Slots.IsValidIndex(Slot1) || !ExternInventory->Slots.IsValidIndex(Slot2)) return;
	FItemAmount Temp = GetItem(Slot1);
	FItemAmount Temp2 = ExternInventory->GetItem(Slot2);
	if (!Temp.Identifier.IsEqual(Temp2.Identifier)) 
	{
		SetIndex(Slot1, Temp2);
		ExternInventory->SetIndex(Slot2, Temp);
	}
	else 
	{
		FItemStruct* RowLookUp = ItemDatabase->FindRow<FItemStruct>(Temp.Identifier, TEXT("LookUp Operation"));
		if (!RowLookUp)
		{
		}
			Temp.Amount = Temp2.AddAmount(Temp.Amount, RowLookUp->Stacksize);
			if (Temp.Amount == 0) Temp = FItemAmount();
			SetIndex(Slot1, Temp2);
			ExternInventory->SetIndex(Slot2, Temp);
	}
}

void ASlotInventory::SplitAmountFromTo_Implementation(int32 Slot1, int32 Amount1, ASlotInventory* ExternInventory, int32 Slot2) 
{
	if (!ExternInventory || !(ExternInventory->GetOwner() == GetOwner()) || !(ExternInventory->Slots.IsValidIndex(Slot2)) || !ExternInventory->GetItem(Slot2).Amount == 0)
	{
		return;
	}
	FItemAmount Temp = GetItem(Slot1);
	if (Amount1 < 1 || Amount1 > Temp.Amount)
	{
		return;
	}

	FItemAmount Temp2 = FItemAmount();
	Temp2.Identifier = Temp.Identifier;
	Temp2.Amount = Amount1;
	Temp.RemoveAmount(Amount1);
	ExternInventory->SetIndex(Slot2, Temp2);
	SetIndex(Slot1, Temp);
}

bool ASlotInventory::SplitAmountFromTo_Validate(int32 Slot1, int32 Amount1, ASlotInventory* ExternInventory, int32 Slot2) {
	return true;
}

bool ASlotInventory::IsEmpty() {
	for (FItemAmount Item : Slots)
	{
		if (Item.Amount != 0)
		{
			return false;
		}
	}
	return true;
}