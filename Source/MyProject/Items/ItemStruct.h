// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ItemStruct.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct DESPERADOGAME_API FItemStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Range)
		FText Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Range)
		FText Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Range)
		int32 Stacksize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Range)
		UTexture* Icon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Range)
		TSubclassOf<class AItemBehaviour> Class;
	FItemStruct(){}
};
