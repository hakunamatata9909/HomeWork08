// Fill out your copyright notice in the Description page of Project Settings.


#include "SpartaGameInstance.h"

USpartaGameInstance::USpartaGameInstance()
{
	Score=0;
	CurrentLevelIndex=0;
}

void USpartaGameInstance::AddToScore(int32 Amount)
{
	Score+=Amount;
	UE_LOG(LogTemp,Warning,TEXT("USpartaGameInstance::Score(%d)"),Score);
}