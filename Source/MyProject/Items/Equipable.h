// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItemBehaviour.h"
#include "Equipable.generated.h"

UENUM(BlueprintType)
enum class EArmorSlots : uint8
{
	VE_HAT UMETA(DisplayName = "Hat"),
	VE_BANDANA UMETA(DisplayName = "Bandana"),
	VE_OUTFIT UMETA(DisplayName = "Outfit"),
	VE_CLOAK UMETA(DisplayName = "Cloak"),
	VE_SHOES UMETA(DisplayName = "Shoe"),
	VE_Girdle UMETA(DisplayName = "Girdle"),
	VE_TRINKET UMETA(DisplayName = "Trinket"),
	VE_WEAPON UMETA(DisplayName = "Weapon")
};

USTRUCT(BlueprintType)
struct DESPERADOGAME_API FStatusBoni : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = Stats)
		int32 Coolness;
	UPROPERTY(EditDefaultsOnly, Category = Stats)
		int32 Speech;
	UPROPERTY(EditDefaultsOnly, Category = Stats)
		int32 Expertise;
	UPROPERTY(EditDefaultsOnly, Category = Stats)
		int32 Protection;
	UPROPERTY(EditDefaultsOnly, Category = Stats)
		int32 Warm;
	UPROPERTY(EditDefaultsOnly, Category = Stats)
		int32 Flexibility;
	UPROPERTY(EditDefaultsOnly, Category = Stats)
		int32 NumWeaponSlots;
};

USTRUCT(BlueprintType)
struct DESPERADOGAME_API FEquipableData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = General)
		float MaxDurability;
	UPROPERTY(EditDefaultsOnly, Category = Stats)
		FStatusBoni Boni;
	UPROPERTY(EditAnywhere, Category = Mesh)
		TAssetPtr<USkeletalMesh> Mesh;
	UPROPERTY(EditDefaultsOnly, Category = General)
		EArmorSlots Slot;
};

UCLASS()
class DESPERADOGAME_API AEquipable : public AItemBehaviour
{
	GENERATED_BODY()
private:
	UPROPERTY(Replicated)
		bool Equipt;
protected:
	virtual void Equip();
	UPROPERTY(Replicated)
		float Durability;
public:
	FEquipableData Data;
	// Getter
	//
	//
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "State")
		bool IsEquipt() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Durability")
		int32 GetDurability() const;
	// Buisness
	//
	//
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Durability")
		void UseDurability(int32 Damage);
	/** attaches weapon mesh to pawn's mesh */
	UFUNCTION()
	virtual void AttachMeshToPawn(){}
	/** detaches weapon mesh from pawn */
	UFUNCTION()
	virtual void DetachMeshFromPawn(){}
	// Item Behaviour Overrides
	//
	//
	void Use_Implementation() override;
	virtual FText GetUseHint() override;
};
