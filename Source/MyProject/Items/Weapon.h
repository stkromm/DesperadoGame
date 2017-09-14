// All Assets and Code is Property of me.

#pragma once

#include "Equipable.h"
#include "Weapon.generated.h"

namespace EWeaponState
{
	enum Type
	{
		Idle,
		Firing,
		Reloading,
		Equipping,
		Modifying,
		Repairing
	};
}

USTRUCT(BlueprintType)
struct DESPERADOGAME_API FWeaponData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	/** inifite ammo for reloads */
	UPROPERTY(EditDefaultsOnly, Category = Ammo)
		bool bInfiniteAmmo;

	/** infinite ammo in clip, no reload required */
	UPROPERTY(EditDefaultsOnly, Category = Ammo)
		bool bInfiniteClip;

	/** max ammo */
	UPROPERTY(EditDefaultsOnly, Category = Ammo)
		int32 MaxAmmo;

	/** clip size */
	UPROPERTY(EditDefaultsOnly, Category = Ammo)
		int32 AmmoPerClip;

	/** initial clips */
	UPROPERTY(EditDefaultsOnly, Category = Ammo)
		int32 InitialClips;

	/** time between two consecutive shots */
	UPROPERTY(EditDefaultsOnly, Category = WeaponStat)
		float TimeBetweenShots;

	/** failsafe reload duration if weapon doesn't have any animation for it */
	UPROPERTY(EditDefaultsOnly, Category = WeaponStat)
		float NoAnimReloadDuration;
	/** time between two consecutive shots */
	UPROPERTY(EditDefaultsOnly, Category = WeaponStat)
		float Precision;
	/** time between two consecutive shots */
	UPROPERTY(EditDefaultsOnly, Category = WeaponStat)
		float Damage;
	/** animation played on pawn (1st person view) */
	UPROPERTY(EditDefaultsOnly, Category = Animation)
		UAnimMontage* Pawn1P;

	/** animation played on pawn (3rd person view) */
	UPROPERTY(EditDefaultsOnly, Category = Animation)
		UAnimMontage* Pawn3P;

	/** weapon mesh: 1st person view */
	UPROPERTY(EditDefaultsOnly, Category = Mesh)
		USkeletalMeshComponent* Mesh1P;

	/** weapon mesh: 3rd person view */
	UPROPERTY(EditDefaultsOnly, Category = Mesh)
		USkeletalMeshComponent* Mesh3P;
	/** defaults */
	FWeaponData()
	{
		bInfiniteAmmo = false;
		bInfiniteClip = false;
		MaxAmmo = 100;
		AmmoPerClip = 20;
		InitialClips = 4;
		TimeBetweenShots = 0.2f;
		NoAnimReloadDuration = 1.0f;
		Precision = 0.9f;
		Damage = 10;
	}
};

UCLASS()
class DESPERADOGAME_API AWeapon : public AEquipable
{
	GENERATED_BODY()

public:
	// Constructors
	//
	//
	AWeapon();
	// Buisness  
	//
	//
	virtual void StartReload() {}
	virtual void StartFire() {}
	virtual void StopFire() {}
	virtual void StartTargetting() {}
	virtual void StopTargetting() {}
	virtual void StopReload() {}
	/** attaches weapon mesh to pawn's mesh */
	virtual void AttachMeshToPawn(){}
	/** detaches weapon mesh from pawn */
	virtual void DetachMeshFromPawn(){}
	// Getter
	//
	//
	virtual int32 GetMunitionCap() { return 0; }
	virtual int32 GetMunitionLoaded() { return 0; }
	/** Get the aim of the weapon, allowing for adjustments to be made by the weapon */
	virtual FVector GetAdjustedAim() const{ return FVector(0, 0, 0); }
	/** Get the aim of the camera */
	virtual FVector GetCameraAim() const{ return FVector(0, 0, 0); }
	/** get the muzzle location of the weapon */
	virtual FVector GetMuzzleLocation() const{ return FVector(0, 0, 0); }
	/** get direction of weapon's muzzle */
	virtual FVector GetMuzzleDirection() const{ return FVector(0, 0, 0); }
};
