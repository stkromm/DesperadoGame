#include "MyProject.h"
#include "Bag.h"
#include "DesperadoPlayerState.h"
#include "BagInventory.h"
#include "SlotInventory.h"
#include "UnrealNetwork.h"

ABag::ABag() {
	Super();
	ConstructorHelpers::FObjectFinder<UDataTable> BagTable_BP(TEXT("DataTable'/Game/Database/C++Referenced/BasicItemDatabase.BasicItemDatabase'"));
	if (!BagTable_BP.Object) {
		GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, "Bag Data Table Path is not correct. Bag.cpp l.12");
		Data = FBagData();
		return;
	}
	FBagData* RowLookUp = BagTable_BP.Object->FindRow<FBagData>(Identifier, TEXT("LookUp Operation"));
	if (!RowLookUp)
	{
		Data = *RowLookUp;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, Identifier.ToString() + " is missing in Bag Database.");
		FBagData Empty = FBagData();
		Data = FBagData();
	}
}

void ABag::BeginPlay() {
	UWorld* const World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = Instigator;
		Inventory = World->SpawnActor<ASlotInventory>(SpawnParams);
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, "Error Spawning SlotInventory in Bag. Destroying Bag.");
		Destroy();
	}
}

void ABag::Use_Implementation()
{
	ABagInventory* Inventory = Cast<ABagInventory>(GetOwner()->GetOwner());
	if (!Inventory)return;
	//
	if (!IsEquipt)
	{
		if (Inventory->CanAddBag(this))
		{
			Inventory->AddBag(this);
		}
	}
	else
	{
		if (Inventory->CanRemoveBag(this))
		{
			Inventory->RemoveBag(this);
		}
	}
}

FText ABag::GetUseHint()
{
	return FText::FromString("Right-Click: \nEquip this Bag");
}

ASlotInventory* ABag::GetInventory() const
{
	return Inventory;
}
bool ABag::IsEquipt() const
{
	return IsEquipt;
}
int32 ABag::GetBagSize() const
{
	return Data->BagSize;
}
TSubclassOf<class AItemBehaviour> ABag::GetFilter() const
{
	return Data->ValidItemClass;
}