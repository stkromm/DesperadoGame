// All Assets and Code is Property of me.

#pragma once

#include "GameFramework/Actor.h"
#include "CharacterStatus.generated.h"

UCLASS()
class DESPERADOGAME_API ACharacterStatus : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACharacterStatus();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
