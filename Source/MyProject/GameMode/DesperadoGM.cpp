#pragma once

#include "MyProject.h"
#include "DesperadoPlayerState.h"
#include "DesperadoCharacter.h"
#include "DesperadoHUD.h"
#include "GameFramework/HUD.h"
#include "DefaultDesperadoPlayerController.h"
#include "DesperadoGameState.h"
#include "DesperadoGM.h"

ADesperadoGM::ADesperadoGM() 
{
    FText PlayerPawnReference = TEXT("Blueprint'/Game/Blueprints/C++Referenced/PlayerPawn.PlayerPawn_C'");
	FText HUDReference = TEXT("Blueprint'/Game/Blueprints/C++Referenced/BP_DesperadoHUD.BP_DesperadoHUD_C'");
	// Set Game Classes
	PlayerStateClass = ADesperadoPlayerState::StaticClass();
	//
	static ConstructorHelpers::FClassFinder<ADesperadoCharacter> Pawn(PlayerPawnReference);
	check(Pawn.Class != NULL && "Blueprint Referenced Pawn is wrong);
	DefaultPawnClass = Pawn.Class;
	//
	PlayerControllerClass = ADefaultDesperadoPlayerController::StaticClass();
	static ConstructorHelpers::FClassFinder<ADesperadoHUD> HUD_Cursor(HUDReference);
	check(HUD_Cursor.Class != NULL && "Blueprint Referenced HUD is wrong");
	HUDClass = HUD_Cursor.Class;
	//
	GameStateClass = ADesperadoGameState::StaticClass();

}

void ADesperadoGM::Killed(AController* Killer, AController* KilledPlayer, APawn* Instigator, UDamageType const* const Type) 
{
	unimplemented();
}