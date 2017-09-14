// All Assets and Code is Property of me.

#pragma once

#include "Weapon.h"
#include "Equipable.h"
#include "Components/ActorComponent.h"
#include "CharacterEquipment.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DESPERADOGAME_API UCharacterEquipment : public UActorComponent
{
	GENERATED_BODY()
private:
	UDataTable* EquipDatabase;
	UDataTable* WeaponDatabase;
protected:
	UPROPERTY(BlueprintReadOnly, Replicated, Category = EquiptWeapons)
		TArray<AWeapon*> WeaponSlots;
	UPROPERTY(BlueprintReadOnly, Replicated, Category = EquiptWeapons)
		TArray<AEquipable*> ArmorSlots;
	UPROPERTY(EditDefaultsOnly, Category = EquiptWeapons)
		int32 NumOfWeaponSlots = 3;
	/** current targeting state */
	UPROPERTY(Transient, DuplicateTransient, BlueprintReadOnly,Replicated, Category = SelectedWeapon)
		bool bIsTargeting = false;
	/** current firing state */
	UPROPERTY(Transient, DuplicateTransient, BlueprintReadOnly,Replicated, Category = SelectedWeapon)
		bool bWantsToFire = false;
	/** currently equipped weapon */
	UPROPERTY(BlueprintReadOnly, Transient, Replicated, Category = SelectedWeapon)
		int32 SelectedWeapon = 0;
public:
	// Sets default values for this component's properties
	UCharacterEquipment();
	bool EquipItem(AEquipable* Equipable);
	bool RemoveItem(AEquipable* Equipable);
	AEquipable* GetEquiptItem(EArmorSlots Slot);
	void AddWeapon(AWeapon* Weapon);
	bool CanAddWeapon(AWeapon* Weapon);
	void RemoveWeapon(AWeapon* Weapon);
	void EquipWeapon(int32 Slot);
	void StartWeaponFire();
	void StopWeaponFire();
	void OnStartTargeting();
	void OnStopTargeting();
	void OnNextWeapon();
	void OnPrevWeapon();
	void OnReload();
	void SetTargeting(bool IsTargeting);
	int32 GetArmorSlotFromEnum(EArmorSlots Slot);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = SelectedWeapon)
		bool IsTargeting() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = SelectedWeapon)
		bool CanFire() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = SelectedWeapon)
		bool IsFiring() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = SelectedWeapon)
		bool CanReload() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = SelectedWeapon)
		AWeapon* GetWeapon() const;
};
