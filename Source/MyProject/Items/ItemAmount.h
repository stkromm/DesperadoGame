// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ItemBehaviour.h"
#include "ItemAmount.generated.h"
/**
 *
 */
USTRUCT(BlueprintType)
struct DESPERADOGAME_API FItemAmount
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Range)
		int32 Amount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Range)
		FName Identifier;

	UPROPERTY(BlueprintReadOnly, Category = Range)
		AItemBehaviour* Behaviour;

	void Reset() {
		Amount = 0;
		Identifier = "";
		Behaviour = nullptr;
	}

	int32 RemoveAmount(int32 RAmount) {
		int32 RestAmount = RAmount - Amount;
		if (RestAmount >= 0) {
			// Delete complete removed stack.
			Reset();
		}
		// The amount of the stack was more than was needed to remove. So the stack is only Decremented.
		else  {
			Amount = RestAmount * (-1);
			RestAmount = 0;
		}
		return RestAmount;
	}

	int32 AddAmount(int32 AAmount, int32 Stacksize) {
		int32 RestAmount = AAmount - (Stacksize - Amount);
		if (RestAmount >= 0) {
			Amount = Stacksize;
		}
		else {
			Amount = Stacksize + RestAmount;
			RestAmount = 0;
		}
		return RestAmount;
	}
	static bool IsValidAmount(int32 Amount) {
		return true;
	}
	static bool IsValidIdentifier(FName Identifier) {
		return true;
	}
	bool CanAddAmount(FName PIdentifier) {
		return Identifier.IsEqual(PIdentifier) || Amount == 0;
	}
	// Constructors
	//
	//
	FItemAmount(int32 a, FName i) {
		Identifier = i;
		Amount = a;
		Behaviour = nullptr;
	}

	FItemAmount() {
		Amount = 0;
		Identifier = "";
		Behaviour = nullptr;
	}
	// Operators
	//
	//
	bool operator==(const FItemAmount& RightItemAmount) const {
		return RightItemAmount.Identifier.IsEqual(Identifier) && RightItemAmount.Amount == Amount;
	}
};
