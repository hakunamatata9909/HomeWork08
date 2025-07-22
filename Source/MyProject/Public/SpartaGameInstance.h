// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SpartaGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API USpartaGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	USpartaGameInstance();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpartaGameInstance")
	int32 Score;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpartaGameInstance")
	int32 CurrentLevelIndex;

	UFUNCTION(BlueprintCallable, Category = "SpartaGameInstance")
	void AddToScore(int32 Amount);
};
