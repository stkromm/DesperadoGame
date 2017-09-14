// All Assets and Code is Property of me.

#include "MyProject.h"
#include "Weapon.h"

AWeapon::AWeapon() {
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}
