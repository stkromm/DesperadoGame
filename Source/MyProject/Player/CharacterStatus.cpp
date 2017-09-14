// All Assets and Code is Property of me.

#include "MyProject.h"
#include "CharacterStatus.h"


// Sets default values
ACharacterStatus::ACharacterStatus()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACharacterStatus::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharacterStatus::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

