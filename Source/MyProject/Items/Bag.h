// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlotInventory.h"
#include "Bag.generated.h"

USTRUCT(BlueprintType)
struct DESPERADOGAME_API FBagData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = Size)
		int32 BagSize;
	UPROPERTY(EditDefaultsOnly, Category = Restrictions)
	TSubclassOf<class AItemBehaviour> ValidItemClass;
};

UCLASS()
class MYPROJECT_API ABag : public AItemBehaviour
{
	GENERATED_BODY()
private:
	UPROPERTY()
		bool IsEquipt;
protected:
	UPROPERTY()
	FBagData Data;
	UPROPERTY()
	ASlotInventory* Inventory;
public:
	// Constructors
	//
	//
	ABag();
	// Getter
	//
	//
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "General")
	ASlotInventory* GetInventory() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Bag")
	bool IsEquipt() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Bag")
	int32 GetBagSize() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Bag")
	TSubclassOf<class AItemBehaviour> GetFilter() const;

	// Overrides
	//
	//
	virtual void Use_Implementation() override;
	virtual FText GetUseHint() override;
	void BeginPlay() override;
};
