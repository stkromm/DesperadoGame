// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "DesperadoHUD.h"
#include "UnrealNetwork.h"
#include "DesperadoGM.h"
#include "DesperadoPlayerState.h"
#include "FirstPersonMovementComponent.h"
#include "CharacterEquipment.h"
#include "DefaultDesperadoPlayerController.h"
#include "DesperadoCharacter.h"

#pragma region Class

void ADesperadoCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ADesperadoCharacter, bWantsToRun, COND_SkipOwner);
}

// Sets default values
ADesperadoCharacter::ADesperadoCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UFirstPersonMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = false;
	Equipment = CreateDefaultSubobject<UCharacterEquipment>(TEXT("EquipComponent"));
	GetMesh()->bOnlyOwnerSee = false;
	GetMesh()->bOwnerNoSee = true;
	GetMesh()->bReceivesDecals = false;
	GetMesh()->SetCollisionObjectType(ECC_Pawn);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

#pragma endregion Class

#pragma region InputBinding
// Called to bind functionality to input
#pragma region  Movement
void  ADesperadoCharacter::MoveForward(float Val)
{
	if (Controller && Val != 0.f)
	{
		// Limit pitch when walking or falling
		const bool bLimitRotation = (GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling());
		const FRotator Rotation = bLimitRotation ? GetActorRotation() : Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Val);
	}
}
void  ADesperadoCharacter::MoveRight(float Val)
{
		if (Val != 0.f)
		{
			const FQuat Rotation = GetActorQuat();
			const FVector Direction = FQuatRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
			AddMovementInput(Direction, Val);
		}
	}
void  ADesperadoCharacter::MoveUp(float Val)
{
	if (Val != 0.f)
	{
		// Not when walking or falling.
		if (GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling())
		{
			return;
		}
		AddMovementInput(FVector::UpVector, Val);
	}
}
void  ADesperadoCharacter::TurnAtRate(float Val)
{
		// calculate delta for this frame from the rate information
		AddControllerYawInput(Val * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}
void  ADesperadoCharacter::LookUpAtRate(float Val)
{
		// calculate delta for this frame from the rate information
		AddControllerPitchInput(Val * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
// Jumping
void  ADesperadoCharacter::OnStartJump()
{
	bPressedJump = true;
}
void  ADesperadoCharacter::OnStopJump()
{
	bPressedJump = false;
}
// Running
void  ADesperadoCharacter::OnStartRunning()
{
	SetRunning(true, false);
}
void  ADesperadoCharacter::OnStopRunning()
{
	SetRunning(false, false);
}
void ADesperadoCharacter::StartCrouch() {
		Crouch(true);
}
void ADesperadoCharacter::StopCrouch() {
	UnCrouch(true);
}
void  ADesperadoCharacter::SetRunning(bool bNewRunning, bool bToggle)
{
	bWantsToRun = bNewRunning;

	if (Role < ROLE_Authority)
	{
		ServerSetRunning(bNewRunning, bToggle);
	}
}
bool ADesperadoCharacter::ServerSetRunning_Validate(bool bNewRunning, bool bToggle)
{
	return true;
}
void ADesperadoCharacter::ServerSetRunning_Implementation(bool bNewRunning, bool bToggle)
{
	SetRunning(bNewRunning, bToggle);
}
bool  ADesperadoCharacter::IsRunning() const
{
	if (!GetCharacterMovement())
	{
		return false;
	}
	return bWantsToRun && !GetVelocity().IsZero() && (GetVelocity().GetSafeNormal2D() | GetActorForwardVector()) > -0.1;
}
#pragma endregion Movement
#pragma region Weapon
void   ADesperadoCharacter::OnStartFire()
{
	ADefaultDesperadoPlayerController* MyPC = Cast<ADefaultDesperadoPlayerController>(Controller);
	if (MyPC && MyPC->IsGameInputAllowed())
	{
		Equipment->StartWeaponFire();
	}
}

void   ADesperadoCharacter::OnStopFire()
{
	Equipment->StopWeaponFire();
}

void  ADesperadoCharacter::OnStartTargeting()
{
	Equipment->OnStartTargeting();
}

void ADesperadoCharacter::OnStopTargeting()
{
	Equipment->OnStopTargeting();
}

void ADesperadoCharacter::OnNextWeapon()
{
	Equipment->OnNextWeapon();
}

void  ADesperadoCharacter::OnPrevWeapon()
{
	Equipment->OnPrevWeapon();
}

void  ADesperadoCharacter::OnReload()
{
	Equipment->OnReload();
}
#pragma endregion Weapon
#pragma endregion InputBinding

/** Contains all Methods, for switching third and first person perspective. */

/* Handles Damage Events from all sources.*/
#pragma region Damage
bool  ADesperadoCharacter::IsEnemyFor(AController* TestPC) const
{
	return true;
}

void  ADesperadoCharacter::FellOutOfWorld(const class UDamageType& dmgType)
{
	Die(100000, FDamageEvent(dmgType.GetClass()), NULL, NULL);
}

void  ADesperadoCharacter::Suicide()
{
	KilledBy(this);
}

void  ADesperadoCharacter::KilledBy(APawn* EventInstigator)
{
	if (Role == ROLE_Authority && !bIsDying)
	{
		AController* Killer = NULL;
		if (EventInstigator != NULL)
		{
			Killer = EventInstigator->Controller;
			LastHitBy = NULL;
		}

		Die(100000, FDamageEvent(UDamageType::StaticClass()), Killer, NULL);
	}
}

float  ADesperadoCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	int32 Health = 0;
	if (Health <= 0.f)
	{
		return 0.f;
	}
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (ActualDamage > 0.f)
	{
		Health -= ActualDamage;
		if (Health <= 0)
		{
			Die(ActualDamage, DamageEvent, EventInstigator, DamageCauser);
		}
		else
		{
			PlayHit(ActualDamage, DamageEvent, EventInstigator ? EventInstigator->GetPawn() : NULL, DamageCauser);
		}
		MakeNoise(1.0f, EventInstigator ? EventInstigator->GetPawn() : this);
	}

	return ActualDamage;
}

bool  ADesperadoCharacter::CanDie(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser) const
{
	if (bIsDying										// already dying
		|| IsPendingKill()								// already destroyed
		|| Role != ROLE_Authority						// not authority
		|| GetWorld()->GetAuthGameMode() == NULL
		|| GetWorld()->GetAuthGameMode()->GetMatchState() == MatchState::LeavingMap)	// level transition occurring
	{
		return false;
	}
	return true;
}

bool  ADesperadoCharacter::Die(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser)
{
	if (!CanDie(KillingDamage, DamageEvent, Killer, DamageCauser))
	{
		return false;
	}
	int32 Health = FMath::Min(0.0f, 0.f);
	// if this is an environmental death then refer to the previous killer so that they receive credit (knocked into lava pits, etc)
	UDamageType const* const DamageType = DamageEvent.DamageTypeClass ? DamageEvent.DamageTypeClass->GetDefaultObject<UDamageType>() : GetDefault<UDamageType>();
	Killer = GetDamageInstigator(Killer, *DamageType);
	AController* const KilledPlayer = (Controller != NULL) ? Controller : Cast<AController>(GetOwner());
	GetWorld()->GetAuthGameMode<ADesperadoGM>()->Killed(Killer, KilledPlayer, this, DamageType);
	GetCharacterMovement()->ForceReplicationUpdate();
	OnDeath(KillingDamage, DamageEvent, Killer ? Killer->GetPawn() : NULL, DamageCauser);
	return true;
}

void  ADesperadoCharacter::OnDeath(float KillingDamage, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser)
{
	if (bIsDying)
	{
		return;
	}
	bReplicateMovement = false;
	bTearOff = true;
	bIsDying = true;

	if (Role == ROLE_Authority)
	{
		ReplicateHit(KillingDamage, DamageEvent, PawnInstigator, DamageCauser, true);
	}

	// remove all weapons
	DetachFromControllerPendingDestroy();
	StopAllAnimMontages();
	if (GetMesh())
	{
		static FName CollisionProfileName(TEXT("Ragdoll"));
		GetMesh()->SetCollisionProfileName(CollisionProfileName);
	}
	SetActorEnableCollision(true);

	// Death anim
	float DeathAnimDuration = 0;// PlayAnimMontage(DeathAnim);

	// Ragdoll
	if (DeathAnimDuration > 0.f)
	{
		// Use a local timer handle as we don't need to store it for later but we don't need to look for something to clear
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ADesperadoCharacter::SetRagdollPhysics, FMath::Min(0.1f, DeathAnimDuration), false);
	}
	else
	{
		SetRagdollPhysics();
	}
	// disable collisions on capsule
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
}

void  ADesperadoCharacter::PlayHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser)
{
	if (Role == ROLE_Authority)
	{
		ReplicateHit(DamageTaken, DamageEvent, PawnInstigator, DamageCauser, false);

		/* play the force feedback effect on the client player controller
		APlayerController* PC = Cast<APlayerController>(Controller);
		if (PC && DamageEvent.DamageTypeClass)
		{
		/*UDamageType *DamageType = Cast<UDamageType>(DamageEvent.DamageTypeClass->GetDefaultObject());
		if (DamageType && DamageType->HitForceFeedback)
		{
		PC->ClientPlayForceFeedback(DamageType->HitForceFeedback, false, "Damage");
		}
		}
		}*/

		if (DamageTaken > 0.f)
		{
			ApplyDamageMomentum(DamageTaken, DamageEvent, PawnInstigator, DamageCauser);
		}

		ADefaultDesperadoPlayerController* MyPC = Cast<ADefaultDesperadoPlayerController>(Controller);
		ADesperadoHUD* MyHUD = MyPC ? Cast<ADesperadoHUD>(MyPC->GetHUD()) : NULL;
		if (MyHUD)
		{
			MyHUD->NotifyWeaponHit(DamageTaken, DamageEvent, PawnInstigator);
		}

		if (PawnInstigator && PawnInstigator != this && PawnInstigator->IsLocallyControlled())
		{
			ADefaultDesperadoPlayerController* InstigatorPC = Cast<ADefaultDesperadoPlayerController>(PawnInstigator->Controller);
			ADesperadoHUD* InstigatorHUD = InstigatorPC ? Cast<ADesperadoHUD>(InstigatorPC->GetHUD()) : NULL;
			if (InstigatorHUD)
			{
				InstigatorHUD->NotifyEnemyHit();
			}
		}
	}
}

void  ADesperadoCharacter::ReplicateHit(float Damage, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser, bool bKilled)
{
	/*const float TimeoutTime = GetWorld()->GetTimeSeconds() + 0.5f;

	FDamageEvent const& LastDamageEvent = LastTakeHitInfo.GetDamageEvent();
	if ((PawnInstigator == LastTakeHitInfo.PawnInstigator.Get()) && (LastDamageEvent.DamageTypeClass == LastTakeHitInfo.DamageTypeClass) && (LastTakeHitTimeTimeout == TimeoutTime))
	{
	// same frame damage
	if (bKilled && LastTakeHitInfo.bKilled)
	{
	// Redundant death take hit, just ignore it
	return;
	}

	// otherwise, accumulate damage done this frame
	Damage += LastTakeHitInfo.ActualDamage;
	}

	LastTakeHitInfo.ActualDamage = Damage;
	LastTakeHitInfo.PawnInstigator = Cast<AShooterCharacter>(PawnInstigator);
	LastTakeHitInfo.DamageCauser = DamageCauser;
	LastTakeHitInfo.SetDamageEvent(DamageEvent);
	LastTakeHitInfo.bKilled = bKilled;
	LastTakeHitInfo.EnsureReplication();

	LastTakeHitTimeTimeout = TimeoutTime;*/
}
//Pawn::PlayDying sets this lifespan, but when that function is called on client, dead pawn's role is still SimulatedProxy despite bTearOff being true. 
void ADesperadoCharacter::TornOff()
{
	SetLifeSpan(25.f);
}
#pragma endregion Damage

/* Handle animtionstate of the pawn.*/
#pragma region Animation
float  ADesperadoCharacter::PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName)
{
	USkeletalMeshComponent* UseMesh = GetMesh();
	if (AnimMontage && UseMesh && UseMesh->AnimScriptInstance)
	{
		return UseMesh->AnimScriptInstance->Montage_Play(AnimMontage, InPlayRate);
	}
	return 0.0f;
}
void  ADesperadoCharacter::StopAnimMontage(class UAnimMontage* AnimMontage)
{
	USkeletalMeshComponent* UseMesh = GetMesh();
	if (AnimMontage && UseMesh && UseMesh->AnimScriptInstance &&
		UseMesh->AnimScriptInstance->Montage_IsPlaying(AnimMontage))
	{
		UseMesh->AnimScriptInstance->Montage_Stop(AnimMontage->BlendOutTime);
	}
}
void  ADesperadoCharacter::StopAllAnimMontages()
{
	USkeletalMeshComponent* UseMesh = GetMesh();
	if (UseMesh && UseMesh->AnimScriptInstance)
	{
		UseMesh->AnimScriptInstance->Montage_Stop(0.0f);
	}
}
#pragma endregion Animation

FName  ADesperadoCharacter::GetWeaponAttachPoint() const
{
	return WeaponAttachPoint;
}

float  ADesperadoCharacter::GetTargetingSpeedModifier() const
{
	return TargetingSpeedModifier;
}

float  ADesperadoCharacter::GetRunningSpeedModifier() const
{
	return RunningSpeedModifier;
}

bool  ADesperadoCharacter::IsAlive() const
{
	return true;
}

FRotator  ADesperadoCharacter::GetAimOffsets() const
{
	const FVector AimDirWS = GetBaseAimRotation().Vector();
	const FVector AimDirLS = ActorToWorld().InverseTransformVectorNoScale(AimDirWS);
	const FRotator AimRotLS = AimDirLS.Rotation();
	return AimRotLS;
}

void  ADesperadoCharacter::SetRagdollPhysics()
{
	bool bInRagdoll = false;

	if (IsPendingKill())
	{
		bInRagdoll = false;
	}
	else if (!GetMesh() || !GetMesh()->GetPhysicsAsset())
	{
		bInRagdoll = false;
	}
	else
	{
		// initialize physics/etc
		GetMesh()->SetAllBodiesSimulatePhysics(true);
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->WakeAllRigidBodies();
		GetMesh()->bBlendPhysics = true;

		bInRagdoll = true;
	}

	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->SetComponentTickEnabled(false);

	if (!bInRagdoll)
	{
		// hide and set short lifespan
		TurnOff();
		SetActorHiddenInGame(true);
		SetLifeSpan(1.0f);
	}
	else
	{
		SetLifeSpan(10.0f);
	}
}

UCharacterEquipment*  const ADesperadoCharacter::GetEquipment() {
	return Equipment;
}