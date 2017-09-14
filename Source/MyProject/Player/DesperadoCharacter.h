// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Weapon.h"
#include "CharacterEquipment.h"
#include "DesperadoCharacter.generated.h"

UCLASS(Blueprintable)
class MYPROJECT_API ADesperadoCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()

#pragma region Properties
private:
	UPROPERTY()
		UCharacterEquipment* Equipment;
protected:
	
	/** modifier for max movement speed */
	UPROPERTY(EditDefaultsOnly, Category = Behaviour)
		float TargetingSpeedModifier = 0.5f;
	/** modifier for max movement speed */
	UPROPERTY(EditDefaultsOnly, Category = Behaviour)
		float RunningSpeedModifier = 3.f;
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(EditDefaultsOnly, Category = Behaviour)
	float BaseTurnRate = 45.f;
	/** Base lookup rate, in deg/sec. Other scaling may affect final lookup rate. */
	UPROPERTY(EditDefaultsOnly, Category = Behaviour)
	float BaseLookUpRate = 45.f;
	/** socket or bone name for attaching weapon mesh */
	UPROPERTY(EditDefaultsOnly, Category = AttachPoints)
		FName WeaponAttachPoint;
	/** current running state */
	UPROPERTY(Transient, Replicated)
	bool bWantsToRun = false;
	/** Identifies if pawn is in its dying state */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health)
		uint32 bIsDying : 1;
	/** Time at which point the last take hit info for the actor times out and won't be replicated; Used to stop join-in-progress effects all over the screen */
	float LastTakeHitTimeTimeout;
	/** notification when killed, for both the server and client. */
	virtual void OnDeath(float KillingDamage, struct FDamageEvent const& DamageEvent, class APawn* InstigatingPawn, class AActor* DamageCauser);
	/** play effects on hit */
	virtual void PlayHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser);
	/** sets up the replication for taking a hit */
	void ReplicateHit(float Damage, struct FDamageEvent const& DamageEvent, class APawn* InstigatingPawn, class AActor* DamageCauser, bool bKilled);
	/** update targeting state */
	UFUNCTION(reliable, server, WithValidation)
		void ServerSetRunning(bool bNewRunning, bool bToggle);
	/** Responsible for cleaning up bodies on clients. */
	virtual void TornOff();
	/** switch to ragdoll */
	void SetRagdollPhysics();
public:
	UFUNCTION(BlueprintCallable, Category = "Game|Equipment")
		UCharacterEquipment*  const GetEquipment();
	/** get aim offsets */
	UFUNCTION(BlueprintCallable, Category = "Game|Weapon")
		FRotator GetAimOffsets() const;
	/** get weapon attach point */
	FName GetWeaponAttachPoint() const;
	/** get weapon taget modifier speed	*/
	UFUNCTION(BlueprintCallable, Category = "Game|Weapon")
		float GetTargetingSpeedModifier() const;
	/** get the modifier value for running speed */
	UFUNCTION(BlueprintCallable, Category = Pawn)
		float GetRunningSpeedModifier() const;
	/** get running state */
	UFUNCTION(BlueprintCallable, Category = Pawn)
	bool IsRunning() const;

#pragma region InputHandlers
	void SetRunning(bool bNewRunning, bool bToggle);
	void MoveForward(float Val);
	void MoveRight(float Val);
	void MoveUp(float Val);
	void TurnAtRate(float Val);
	void LookUpAtRate(float Val);
	void OnStartFire();
	void OnStopFire();
	void OnStartTargeting();
	void OnStopTargeting();
	void OnNextWeapon();
	void OnPrevWeapon();
	void OnReload();
	void OnStartJump();
	void OnStopJump();
	void OnStartRunning();
	void OnStopRunning();
	void StartCrouch();
	void StopCrouch();
#pragma endregion InputHandlers
#pragma region Combat
	/** check if pawn is still alive */
	bool IsAlive() const;
	virtual bool IsEnemyFor(AController* TestPC) const;
	/** Pawn suicide */
	UFUNCTION(Exec,BlueprintCallable, Category = Damage)
	virtual void Suicide();
	/** Returns True if the pawn can die in the current state */
	virtual bool CanDie(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser) const;
	/**
	* Kills pawn.  Server/authority only.
	* @param KillingDamage - Damage amount of the killing blow
	* @param DamageEvent - Damage event of the killing blow
	* @param Killer - Who killed this pawn
	* @param DamageCauser - the Actor that directly caused the damage (i.e. the Projectile that exploded, the Weapon that fired, etc)
	* @returns true if allowed
	*/
	virtual bool Die(float KillingDamage, struct FDamageEvent const& DamageEvent, class AController* Killer, class AActor* DamageCauser);
	/** Kill this pawn */
	virtual void KilledBy(class APawn* EventInstigator);
	// Die when we fall out of the world.
	virtual void FellOutOfWorld(const class UDamageType& dmgType) override;
	/** Take damage, handle death */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;
#pragma endregion Combat
#pragma region Animation
	/** stop playing all montages */
	void StopAllAnimMontages();
	/** play anim montage */
	virtual float PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate = 1.f, FName StartSectionName = NAME_None) override;
	/** stop playing montage */
	virtual void StopAnimMontage(class UAnimMontage* AnimMontage) override;
#pragma endregion Animation
};



