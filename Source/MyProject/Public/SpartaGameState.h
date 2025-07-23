// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SpartaGameState.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API ASpartaGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	ASpartaGameState();

	UPROPERTY(visibleAnywhere,BlueprintReadOnly, Category = "Coin")
	int32 SpawnedCoinCount;
	UPROPERTY(visibleAnywhere,BlueprintReadOnly, Category = "Coin")
	int32 CollectedCoinCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score");
	int32 Score;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	float WaveDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	int32 MaxLevels;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	TArray<FName> LevelNames;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MaxWave")
	int32 MaxWave;
	
	int32 CurrentLevelIndex;
	FTimerHandle WaveTimer;
	FTimerHandle HudUpdateTimer;
	int32 CurrentWave;

	
	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const;
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);
	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameOver();


	void StartLevel();
	void OnCoinCollected();
	void UpdateHUD();
	void EndLevel();
	virtual void BeginPlay() override;
	void OnWaveTimedUp();
	void StartWave();
	void EndWave();
	void NextWave();
};
