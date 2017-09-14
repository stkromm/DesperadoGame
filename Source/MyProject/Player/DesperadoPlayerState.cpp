// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "DesperadoPlayerState.h"
#include "UnrealNetwork.h"

void ADesperadoPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADesperadoPlayerState, Inventory);
}

ADesperadoPlayerState::ADesperadoPlayerState() {

}

void ADesperadoPlayerState::BeginPlay() {
	UWorld* const World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;
		// spawn the projectile at the muzzle
		Inventory = World->SpawnActor<ABagInventory>(SpawnParams);
	}
}