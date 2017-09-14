#pragma once

#include "Object.h"
#include "ItemBehaviour.h"
#include "AbstractInventory.generated.h"

UCLASS(Blueprintable, abstract)
class DESPERADOGAME_API AAbstractInventory : public AActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void AddItem(int32 Amount, FName Identifier, AItemBehaviour* Behaviour);
	UFUNCTION(Server, WithValidation, Reliable)
		void ServerAddItem(int32 Amount, FName Identifier, AItemBehaviour* Behaviour);
	virtual void ServerAddItem_Implementation(int32 Amount, FName Identifier, AItemBehaviour* Behaviour)
	{
		check(0 && "you must override this");
	}
	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void RemoveItem(int32 Amount, FName Identifier, bool DestroyBehaviour);
	UFUNCTION(Server, WithValidation, Reliable)
		virtual void ServerRemoveItem(int32 Amount, FName Identifier, bool DestroyBehaviour);
	virtual void ServerRemoveItem_Implementation(int32 Amount, FName Identifier, bool DestroyBehaviour)
	{
		check(0 && "you must override this");
	}
	UFUNCTION(BlueprintCallable, Category = "Inventory", BlueprintPure)
		virtual bool CanAddItem(int32 Amount, FName Identifier)
	{
		check(0 && "you must override this");
		return 0;
	}
	UFUNCTION(BlueprintCallable, Category = "Inventory", BlueprintPure)
		virtual bool CanRemoveItem(int32 Amount, FName Identifier)
	{
		check(0 && "you must override this");
		return false;
	}

	UFUNCTION(BlueprintCallable, Category = "Inventory", BlueprintPure)
		virtual int32 GetAmount(FName Identifier)
	{
		check(0 && "you must override this");
		return 0;
	}

	UFUNCTION(BlueprintCallable, Category = "Inventory", BlueprintPure)
		virtual int32 GetNumOfSlots()
	{
		return 0;
	}

	UFUNCTION(BlueprintCallable, Category = "Inventory", BlueprintPure)
		virtual int32 GetAddableAmount(FName Identifier)	
	{
		check(0 && "you must override this");
		return 0;
	}
};
