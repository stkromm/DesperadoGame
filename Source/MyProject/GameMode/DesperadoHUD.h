#pragma once

#include "GameFramework/HUD.h"
#include "DesperadoHUD.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class DESPERADOGAME_API ADesperadoHUD : public AHUD
{
	GENERATED_BODY()

public:
	// Menus
	//
	//
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Menus")
		void ToggleInventory();
	// Notifications
	//
	//
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Notifications")
	void NotifyEnemyHit();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Notifications")
	void NotifyWeaponHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator);
};
