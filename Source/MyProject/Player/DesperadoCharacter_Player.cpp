// All Assets and Code is Property of me.

#include "MyProject.h"

#include "DesperadoHUD.h"
#include "UnrealNetwork.h"
#include "DesperadoGM.h"
#include "DesperadoPlayerState.h"
#include "FirstPersonMovementComponent.h"
#include "CharacterEquipment.h"
#include "DefaultDesperadoPlayerController.h"
#include "DesperadoCharacter_Player.h"

// Sets default values
ADesperadoCharacter_Player::ADesperadoCharacter_Player(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UFirstPersonMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = false;
	Mesh1P = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("PawnMesh1P"));
	Mesh1P->AttachParent = GetCapsuleComponent();
	Mesh1P->bOnlyOwnerSee = true;
	Mesh1P->bOwnerNoSee = false;
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->bReceivesDecals = false;
	Mesh1P->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	Mesh1P->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	Mesh1P->bChartDistanceFactor = false;
	Mesh1P->SetCollisionObjectType(ECC_Pawn);
	Mesh1P->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh1P->SetCollisionResponseToAllChannels(ECR_Ignore);
}
void ADesperadoCharacter_Player::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	check(InputComponent);
	Super::SetupPlayerInputComponent(InputComponent);
	//Movement
	InputComponent->BindAxis("MoveForward", this, &ADesperadoCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ADesperadoCharacter::MoveRight);
	InputComponent->BindAxis("MoveUp", this, &ADesperadoCharacter::MoveUp);
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &ADesperadoCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &ADesperadoCharacter::LookUpAtRate);
	//
	InputComponent->BindAction("Jump", IE_Pressed, this, &ADesperadoCharacter::OnStartJump);
	InputComponent->BindAction("Jump", IE_Released, this, &ADesperadoCharacter::OnStopJump);
	//
	InputComponent->BindAction("Run", IE_Pressed, this, &ADesperadoCharacter::OnStartRunning);
	InputComponent->BindAction("Run", IE_Released, this, &ADesperadoCharacter::OnStopRunning);
	//
	InputComponent->BindAction("Crouch", IE_Pressed, this, &ADesperadoCharacter::StartCrouch);
	InputComponent->BindAction("Crouch", IE_Released, this, &ADesperadoCharacter::StopCrouch);
	// Weapon
	InputComponent->BindAction("Fire", IE_Pressed, this, &ADesperadoCharacter::OnStartFire);
	InputComponent->BindAction("Fire", IE_Released, this, &ADesperadoCharacter::OnStopFire);
	InputComponent->BindAction("Targeting", IE_Pressed, this, &ADesperadoCharacter::OnStartTargeting);
	InputComponent->BindAction("Targeting", IE_Released, this, &ADesperadoCharacter::OnStopTargeting);
	InputComponent->BindAction("NextWeapon", IE_Pressed, this, &ADesperadoCharacter::OnNextWeapon);
	InputComponent->BindAction("PrevWeapon", IE_Pressed, this, &ADesperadoCharacter::OnPrevWeapon);
	InputComponent->BindAction("Reload", IE_Pressed, this, &ADesperadoCharacter::OnReload);
	InputComponent->BindAction("ToggleInventory", IE_Pressed, this, &ADesperadoCharacter_Player::ToggleInventory);
	InputComponent->BindAction("SwitchPerspektive", IE_Pressed, this, &ADesperadoCharacter_Player::UpdatePawnMeshes);
	// Camera
	InputComponent->BindAction("MoveCamera", IE_Pressed, this, &ADesperadoCharacter_Player::ToggleRightMouse);
	InputComponent->BindAction("MoveCamera", IE_Released, this, &ADesperadoCharacter_Player::ToggleRightMouse);
	InputComponent->BindAxis("MouseMove", this, &ADesperadoCharacter_Player::MoveForwardSec);
	InputComponent->BindAxis("Scroll", this, &ADesperadoCharacter_Player::ChangeCameraDistance);
}

#pragma region Perspective
void  ADesperadoCharacter_Player::UpdatePawnMeshes()
{
	// Set pawn visible, if playeri s in third person.
	IsFirstPerson = !IsFirstPerson;
	bRightMousePressed = false;
	ADefaultDesperadoPlayerController* MyPC = Cast<ADefaultDesperadoPlayerController>(Controller);
	MyPC->bShowMouseCursor = !IsFirstPerson && (bIsInInventory || (!bRightMousePressed && !IsFirstPerson));
	bool const bFirstPerson = IsFirstPerson;

	for (UActorComponent* Camera : GetComponentsByTag(UCameraComponent::StaticClass(), "Thirdperson")) {
		Camera->SetActive(!bFirstPerson);
	}
	for (UActorComponent* Camera : GetComponentsByTag(UCameraComponent::StaticClass(), "Firstperson")) {
		Camera->SetActive(bFirstPerson);
	}
	Mesh1P->MeshComponentUpdateFlag = !bFirstPerson ? EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered : EMeshComponentUpdateFlag::AlwaysTickPoseAndRefreshBones;
	Mesh1P->SetOwnerNoSee(!bFirstPerson);
	GetMesh()->MeshComponentUpdateFlag = bFirstPerson ? EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered : EMeshComponentUpdateFlag::AlwaysTickPoseAndRefreshBones;
	GetMesh()->SetOwnerNoSee(bFirstPerson);
}
void  ADesperadoCharacter_Player::OnCameraUpdate(const FVector& CameraLocation, const FRotator& CameraRotation)
{
	USkeletalMeshComponent* DefMesh1P = Cast<USkeletalMeshComponent>(GetClass()->GetDefaultSubobjectByName(TEXT("PawnMesh1P")));
	const FMatrix DefMeshLS = FRotationTranslationMatrix(DefMesh1P->RelativeRotation, DefMesh1P->RelativeLocation);
	const FMatrix LocalToWorld = ActorToWorld().ToMatrixWithScale();

	// Mesh rotating code expect uniform scale in LocalToWorld matrix

	const FRotator RotCameraPitch(CameraRotation.Pitch, 0.0f, 0.0f);
	const FRotator RotCameraYaw(0.0f, CameraRotation.Yaw, 0.0f);

	const FMatrix LeveledCameraLS = FRotationTranslationMatrix(RotCameraYaw, CameraLocation) * LocalToWorld.Inverse();
	const FMatrix PitchedCameraLS = FRotationMatrix(RotCameraPitch) * LeveledCameraLS;
	const FMatrix MeshRelativeToCamera = DefMeshLS * LeveledCameraLS.Inverse();
	const FMatrix PitchedMesh = MeshRelativeToCamera * PitchedCameraLS;

	Mesh1P->SetRelativeLocationAndRotation(PitchedMesh.GetOrigin(), PitchedMesh.Rotator());
}
#pragma endregion Perspective

void ADesperadoCharacter_Player::ChangeCameraDistance(float Val) {
	if (Val == 0) return;
	USpringArmComponent* Arm;

	for (UActorComponent* Camera : GetComponentsByTag(USpringArmComponent::StaticClass(), "Thirdperson")) {
		Arm = Cast<USpringArmComponent>(Camera);
		if (!Arm) continue;
		int32 NewArmLength = Arm->TargetArmLength + (10 * Val);
		if (NewArmLength >= 400) {
			NewArmLength = 400;
		}
		if (NewArmLength <= 0) {
			NewArmLength = 0;
		}
		Arm->TargetArmLength = NewArmLength;
		if (IsFirstPerson) {
			if (NewArmLength > 80) {
				UpdatePawnMeshes();
			}
		}
		else {
			if (NewArmLength < 50) {
				UpdatePawnMeshes();
			}
		}
	}
}
void ADesperadoCharacter_Player::MoveForwardSec(float Val) {
	bLeftMousePressed = Val != 0;
	if (bRightMousePressed) {
		MoveForward(Val);
	}
}
void ADesperadoCharacter_Player::ToggleRightMouse() {
	bRightMousePressed = !bRightMousePressed;
	ADefaultDesperadoPlayerController* MyPC = Cast<ADefaultDesperadoPlayerController>(Controller);
	MyPC->bShowMouseCursor = bIsInInventory || (!bRightMousePressed && !IsFirstPerson);

}void ADesperadoCharacter_Player::ToggleInventory() {
	ADefaultDesperadoPlayerController* MyPC = Cast<ADefaultDesperadoPlayerController>(Controller);
	ADesperadoHUD* MyHUD = MyPC ? Cast<ADesperadoHUD>(MyPC->GetHUD()) : NULL;
	if (MyHUD)
	{
		MyPC->bShowMouseCursor = !bRightMousePressed && !MyPC->bShowMouseCursor;
		bIsInInventory = !bIsInInventory;
		MyHUD->ToggleInventory();
	}
}