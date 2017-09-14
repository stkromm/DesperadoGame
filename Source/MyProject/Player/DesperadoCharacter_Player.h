// All Assets and Code is Property of me.

#pragma once

#include "Player/DesperadoCharacter.h"
#include "DesperadoCharacter_Player.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class MYPROJECT_API ADesperadoCharacter_Player : public ADesperadoCharacter
{
	GENERATED_UCLASS_BODY()
private:
	/** pawn mesh: 1st person view */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USkeletalMeshComponent* Mesh1P;
protected:
	/** Returns Mesh1P subobject **/
	FORCEINLINE USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	bool bRightMousePressed;
	bool bLeftMousePressed;
	void ToggleRightMouse();
	bool bIsInInventory;
	bool IsFirstPerson;
public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	/**
	* Add camera pitch to first person mesh.
	*/
	void OnCameraUpdate(const FVector& CameraLocation, const FRotator& CameraRotation);
	/** handle mesh visibility and updates */
	void UpdatePawnMeshes();
	void ToggleInventory();
	void MoveForwardSec(float Value);
	void ChangeCameraDistance(float Val);
};
