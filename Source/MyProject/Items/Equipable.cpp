// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "UnrealNetwork.h"
#include "DesperadoCharacter.h"
#include "DefaultDesperadoPlayerController.h"
#include "Equipable.h"

void AEquipable::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEquipable, Durability);
	DOREPLIFETIME(AEquipable, Equipt);
}


void  AEquipable::Use_Implementation() {
	Super::Use_Implementation();
	Equip();
}
FText  AEquipable::GetUseHint() {
	FText Hint = Super::GetUseHint();
	return Hint;
}

void AEquipable::Equip() {/*
	if (!OwningPlayer) return;
	if (Equipt) {
		ADesperadoCharacter* Pawn = Cast<ADesperadoCharacter>(OwningPlayer->GetPawn());
		if (Pawn->GetEquipment()->RemoveItem(this)) {
			Equipt = false;
		}
	}
	else {
		ADesperadoCharacter* Pawn = Cast<ADesperadoCharacter>(OwningPlayer->GetOwner()->GetOwner();
		if (Pawn->GetEquipment()->EquipItem(this)) {
			Equipt = true;
		}
	}*/
}

bool AEquipable::IsEquipt() const {
	return Equipt;
}
int32 AEquipable::GetDurability() const {
	return Durability;
}
bool AEquipable::UseDurability_Validate(int32 Damage) {
	return true;
}
void AEquipable::UseDurability_Implementation(int32 Damage) {
	Durability -= (Damage / 1000);
}
