#include "MyProject.h"
#include "UnrealNetwork.h"
#include "Bag.h"
#include "BagInventory.h"


#pragma region Class Methods


void ABagInventory::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABagInventory, Bags);
}

ABagInventory::ABagInventory()
{
	bReplicates = true;
}
void ABagInventory::BeginPlay()
{
	if (Role < ROLE_Authority)
	{
		return;
	}
	check(!GetWorld() && "GetWorld() failed while doin Constructor of BagInventory");
	// Initialize Bag Array
	Bags.Reserve(NumBagSlots);
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;
	ABag* Bag1 = GetWorld()->SpawnActor<ABag>(SpawnParams);
	Bags.Add(Bag1);
	ABag* Bag2 = GetWorld()->SpawnActor<ABag>(SpawnParams);
	Bags.Add(Bag2);
	Bags.Add(nullptr);
	Bags.Add(nullptr);
	Bags.Add(nullptr);
}
#pragma endregion Class Methods

void ABagInventory::ServerAddItem_Implementation(int32 Amount, FName Identifier, AItemBehaviour* Behaviour)
{
	if (Role < ROLE_Authority)
	{
		return;
	}
	int32 RestAmount = Amount;
	for (ABag* const Bag : Bags)
	{
		if (!Bag)
		{
			continue;
		}
		int32 BagCap = Bag->Inventory->GetAddableAmount(Identifier);
		if (BagCap == 0)
		{
			continue;
		}
		if (RestAmount > BagCap)
		{
			RestAmount -= BagCap;
			Bag->Inventory->ServerAddItem(BagCap, Identifier, Behaviour);
		}
		else
		{
			Bag->Inventory->ServerAddItem(RestAmount, Identifier, Behaviour);
			RestAmount = 0;
		}
		if (RestAmount <= 0)
		{
			return;
		}
	}
}
void ABagInventory::ServerRemoveItem_Implementation(int32 Amount, FName Identifier, bool DestroyBehaviour)
{
	if (Role < ROLE_Authority)
	{
		return;
	}
	int32 RestAmount = Amount;
	for (ABag* const Bag : Bags)
	{
		if (!Bag)
		{
			continue;
		}
		int32 BagCap = Bag->Inventory->GetAmount(Identifier);
		RestAmount -= BagCap;
		if (RestAmount >= 0)
		{
			Bag->Inventory->RemoveItem(BagCap, Identifier, DestroyBehaviour);
		}
		else
		{
			Bag->Inventory->RemoveItem(BagCap + RestAmount, Identifier, DestroyBehaviour);
		}
		if (RestAmount <= 0)
		{
			return;
		}
	}
}
bool ABagInventory::CanRemoveItem(int32 Amount, FName Identifier)
{
	return Amount <= GetAmount(Identifier);
}
bool ABagInventory::CanAddItem(int32 Amount, FName Identifier)
{
	return Amount <= GetAddableAmount(Identifier);
}
int32 ABagInventory::GetNumOfSlots()
{
	return Bags.Num();
}
int32 ABagInventory::GetAddableAmount(FName Identifier)
{
	int32 Amount = 0;
	for (ABag* const Bag : Bags)
	{
		if (Bag)
		{
			Amount += Bag->Inventory->GetAddableAmount(Identifier);
		}
	}
	return Amount;
}
int32 ABagInventory::GetAmount(FName Identifier)
{
	int32 Amount = 0;
	for (ABag* const Bag : Bags)
	{
		if (Bag)
		{
			Amount += Bag->Inventory->GetAmount(Identifier);
		}
	}
	return Amount;
}

#pragma region Bag Operations
void ABagInventory::AddBag_Implementation(ABag* Bag)
{
	if (Role < ROLE_Authority)
	{
		return;
	}
	if (CanAddBag(Bag))
	{
		ABag* IBag = Bags.FindByPredicate([](const Bag& B){return B; });
		check(Bag && "CanAddBag failed!");
		IBag = Bag;
	}
}
void ABagInventory::RemoveBag_Implementation(ABag* Bag)
{
	if (Role < ROLE_Authority)
	{
		return;
	}
	if (CanRemoveBag(Bag))
	{
		Bags.Remove(Bag);
		AddAmount(1, Bag->Identifier, Bag);
	}
}
void ABagInventory::SwitchBag_Implementation(int32 Slot1, int32 Slot2)
{
	if (Role < ROLE_Authority)
	{
		return;
	}
	if (Bags.IsValidIndex(Slot1) && Bags.IsValidIndex(Slot2))
	{
		Bags.Swap(Slot1, Slot2);
	}
}
bool ABagInventory::CanRemoveBag(ABag* Bag)
{
	return Bag && Bags.Contains(Bag) ?
		// Check, if there is a Bag in the Bags Array, besides the Parameter Bag, that has a free Slot 
		Bags.FindByPredicate([](const Bag& B){return (B != Bag) && (B->Inventory->GetFreeSlot() != -1); })
		:
		false;
}
bool ABagInventory::CanAddBag(ABag* Bag)
{
	return Bag && !Bags.Contains(Bag) ?
		// Check, if there is a free spot in the Bags Array (free spot == nullptr)
		!(Bags.FindByPredicate([](const Bag& B){return !B; }))
		:
		false;
}
ABag* ABagInventory::GetBag(int32 Slot)
{
	return Bags.IsValidIndex(Slot) ? Bags[Slot] : nullptr;
}
#pragma endregion Bag Operations
#pragma region Validation
bool ABagInventory::SwitchBag_Validate(int32 Slot1, int32 Slot2)
{
	return true;
}
bool ABagInventory::AddBag_Validate(ABag* Bag)
{
	return true;
}
bool ABagInventory::RemoveBag_Validate(ABag* Bag)
{
	return true;
}
#pragma endregion Validation