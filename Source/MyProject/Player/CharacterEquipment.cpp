// All Assets and Code is Property of me.

#include "MyProject.h"
#include "DefaultDesperadoPlayerController.h"
#include "DesperadoCharacter_Player.h"
#include "DesperadoPlayerState.h"
#include "UnrealNetwork.h"
#include "CharacterEquipment.h"


// Sets default values for this component's properties
UCharacterEquipment::UCharacterEquipment()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = false;
	ConstructorHelpers::FObjectFinder<UDataTable> EquipTable_BP(TEXT("DataTable'/Game/Database/C++Referenced/BasicItemDatabase.BasicItemDatabase'"));
	EquipDatabase = EquipTable_BP.Object;
	ConstructorHelpers::FObjectFinder<UDataTable> WeaponTable_BP(TEXT("DataTable'/Game/Database/C++Referenced/BasicItemDatabase.BasicItemDatabase'"));
	WeaponDatabase = WeaponTable_BP.Object;
	for (int b = 0; b < NumOfWeaponSlots; b++) {
		WeaponSlots.Add(nullptr);
	}
	for (int b = 0; b < 7; b++) {
		ArmorSlots.Add(nullptr);
	}
}

void UCharacterEquipment::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCharacterEquipment, WeaponSlots);
	DOREPLIFETIME(UCharacterEquipment, SelectedWeapon);
	DOREPLIFETIME(UCharacterEquipment, bIsTargeting);
	DOREPLIFETIME(UCharacterEquipment, bWantsToFire);
}

#pragma region Targeting
void  UCharacterEquipment::OnStartTargeting()
{
	SetTargeting(true);
}

void  UCharacterEquipment::OnStopTargeting()
{
	SetTargeting(false);
}

void  UCharacterEquipment::SetTargeting(bool bNewTargeting)
{
	bIsTargeting = bNewTargeting;
}

bool UCharacterEquipment::IsTargeting() const {
	return bIsTargeting;
}
#pragma endregion Targeting

void  UCharacterEquipment::OnNextWeapon()
{
	// if(CurrentWeapon->GetCurrentState() != EWeaponState::Equipping)
	//{
	if (SelectedWeapon == NumOfWeaponSlots - 1) {
		EquipWeapon(0);
	}
	else {
		EquipWeapon(SelectedWeapon + 1);
	}
	//}
}

void  UCharacterEquipment::OnPrevWeapon()
{
	// if(CurrentWeapon->GetCurrentState() != EWeaponState::Equipping)
	//{
	if (SelectedWeapon == 0) {
		EquipWeapon(NumOfWeaponSlots - 1);
	}
	else {
		EquipWeapon(SelectedWeapon - 1);
	}
	//}
}

void  UCharacterEquipment::OnReload()
{
	AWeapon* CurrentWeapon = GetWeapon();
	if (CurrentWeapon)
	{
		CurrentWeapon->StartReload();
	}
}

void  UCharacterEquipment::StartWeaponFire()
{
	if (!bWantsToFire)
	{
		bWantsToFire = true;
		AWeapon* CurrentWeapon = GetWeapon();
		if (CurrentWeapon)
		{
			CurrentWeapon->StartFire();
		}
	}
}

void  UCharacterEquipment::StopWeaponFire()
{
	if (bWantsToFire)
	{
		bWantsToFire = false;
		AWeapon* CurrentWeapon = GetWeapon();
		if (CurrentWeapon)
		{
			CurrentWeapon->StopFire();
		}
	}
}

bool  UCharacterEquipment::CanFire() const
{
	return true;// Is not equiping atm, has ammo in the weapon
}

bool  UCharacterEquipment::CanReload() const
{
	return true;// Is not equiping atm, has ammo in the inventory
}

AWeapon* UCharacterEquipment::GetWeapon() const
{
	if (WeaponSlots.IsValidIndex(SelectedWeapon)) {
		return WeaponSlots[SelectedWeapon];
	}
	return nullptr;
}

bool  UCharacterEquipment::IsFiring() const
{
	return bWantsToFire;
};

void UCharacterEquipment::AddWeapon(AWeapon* Weapon) {
	for (AWeapon* EquipedWeapon : WeaponSlots) {
		if (!EquipedWeapon) {
			EquipedWeapon = Weapon;
			return;
		}
	}
}

void UCharacterEquipment::RemoveWeapon(AWeapon* Weapon) {
	if (WeaponSlots.Contains(Weapon)) {
		WeaponSlots.Remove(Weapon);
	}
}

void UCharacterEquipment::EquipWeapon(int32 Slot) {
	if (WeaponSlots.IsValidIndex(Slot)) {
		SelectedWeapon = Slot;
	}
}

bool UCharacterEquipment::EquipItem(AEquipable* Equipable) {
	FEquipableData* RowLookUp = EquipDatabase->FindRow<FEquipableData>(Equipable->Identifier, TEXT("LookUp Operation"));
	if (!RowLookUp)
	{
		return false;
	}
	if (RowLookUp->Slot == EArmorSlots::VE_WEAPON) {
		AWeapon* AsWeapon = Cast<AWeapon>(Equipable);
		if (CanAddWeapon(AsWeapon)) {
			AddWeapon(AsWeapon);
			return true;
		}
		return false;
	}
	else {
		int32 Index = GetArmorSlotFromEnum(RowLookUp->Slot);
		if (ArmorSlots.IsValidIndex(Index) && !ArmorSlots[Index]) {
			ArmorSlots[Index] = Equipable;
			return true;
		}
		return false;
	}
}
bool UCharacterEquipment::CanAddWeapon(AWeapon* Weapon) {
	for (AWeapon* EquipedWeapon : WeaponSlots) {
		if (!EquipedWeapon) {
			return true;
		}
	}return false;
}
bool UCharacterEquipment::RemoveItem(AEquipable* Equipable) { 
	ADesperadoCharacter_Player* Pawn = Cast<ADesperadoCharacter_Player>(GetOwner());
	if (!Pawn) return false;
	ADefaultDesperadoPlayerController* MyPC = Cast<ADefaultDesperadoPlayerController>(Pawn->GetController());
	if (!MyPC) return false;
	ADesperadoPlayerState* PS = Cast<ADesperadoPlayerState>(MyPC->PlayerState);
	if (PS && (PS->Inventory->CanAddItem(1, Equipable->Identifier))) {
		PS->Inventory->ServerAddItem(1, Equipable->Identifier,Equipable);
		return true;
	}return false;
}
AEquipable* UCharacterEquipment::GetEquiptItem(EArmorSlots Slot) {
	if (ArmorSlots.IsValidIndex(GetArmorSlotFromEnum(Slot))) {
		return ArmorSlots[GetArmorSlotFromEnum(Slot)];
	}return nullptr;
}
int32 UCharacterEquipment::GetArmorSlotFromEnum(EArmorSlots Slot) {
	switch (Slot) {
	case EArmorSlots::VE_HAT:
		return 0;
	case EArmorSlots::VE_BANDANA:
		return 1;
	case EArmorSlots::VE_OUTFIT:
		return 2;
	case EArmorSlots::VE_Girdle:
		return 3;
	case EArmorSlots::VE_CLOAK:
		return 4;
	case EArmorSlots::VE_SHOES:
		return 5;
	case EArmorSlots::VE_TRINKET:
		return 6;
	default:
		return -1;
	}
}