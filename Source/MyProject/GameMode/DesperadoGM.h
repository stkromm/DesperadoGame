#pragma once

#include "GameFramework/GameMode.h"
#include "DesperadoGM.generated.h"

UCLASS()
class DESPERADOGAME_API ADesperadoGM : public AGameMode
{
	GENERATED_BODY()
public:
	// Constructors
	//
	//
	ADesperadoGM();
	// Combat
	//
	//
	void Killed(AController* Killer, AController* KilledPlayer, APawn* Instigator, UDamageType const* const Type);
};
