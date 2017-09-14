
#include "MyProject.h"
#include "AbstractInventory.h"

bool AAbstractInventory::ServerAddItem_Validate(int32 Amount, FName Identifier, AItemBehaviour* Behaviour)
{
	return CanAddItem(Amount, Identifier);
}
bool AAbstractInventory::ServerRemoveItem_Validate(int32 Amount, FName Identifier, bool DestroyBehaviour)
{
	return CanRemoveItem(Amount, Identifier);
}

void AAbstractInventory::RemoveItem(int32 Amount, FName Identifier, bool DestroyBehaviour)
{
	if (CanRemoveItem(Amount, Identifier)) {
		ServerRemoveItem(Amount, Identifier, DestroyBehaviour);
	}
}

void AAbstractInventory::AddItem(int32 Amount, FName Identifier, AItemBehaviour* Behaviour)
{
	if (CanAddItem(Amount, Identifier)) {
		ServerAddItem(Amount, Identifier, Behaviour);
	}
}