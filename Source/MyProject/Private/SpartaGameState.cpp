// Fill out your copyright notice in the Description page of Project Settings.


#include "SpartaGameState.h"
#include "CoinItem.h"
#include "SpawnVolume.h"
#include "SpartaGameInstance.h"
#include "SpartaPlayerController.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

ASpartaGameState::ASpartaGameState()
{
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	MaxLevels = 3;
	CurrentWave = 0;
	MaxWave = 1;
	WaveDuration = 60.0f;
}

int32 ASpartaGameState::GetScore() const
{
	return Score;
}

void ASpartaGameState::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
		if (SpartaGameInstance)
		{
			SpartaGameInstance->AddToScore(Amount);
		}
	}
}

void ASpartaGameState::BeginPlay()
{
	Super::BeginPlay();

	StartLevel();

	GetWorldTimerManager().SetTimer(
		HudUpdateTimer,
		this,
		&ASpartaGameState::UpdateHUD,
		0.1f,
		true
		);
}

void ASpartaGameState::OnWaveTimedUp()
{
	OnGameOver();
}

void ASpartaGameState::StartWave()
{
	
	if (APlayerController*  PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			APawn* Pawn = Cast<APawn>(SpartaPlayerController->GetPawn());
			if (Pawn)
			{
				AActor* StartPos = UGameplayStatics::GetActorOfClass(GetWorld(),APlayerStart::StaticClass());

				if (StartPos)
				{
					Pawn->SetActorLocation(StartPos->GetActorLocation());
					Pawn->SetActorRotation(StartPos->GetActorRotation());
				}
			}
			SpartaPlayerController->ShowGameHud();
		}
	}
	
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
		if (SpartaGameInstance)
		{
			//CurrentWave = SpartaGameInstance->CurrentWave;
		}
	}
	
	SpawnedCoinCount =0;
	CollectedCoinCount = 0;

	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),ASpawnVolume::StaticClass(),FoundVolumes);

	const int32 ItemToSpawn = 40;

	for (int32 i=0; i<ItemToSpawn; i++)
	{
		if (FoundVolumes.Num()>0)
		{
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);

			if (SpawnVolume)
			{
				AActor* SpawnedActor = SpawnVolume -> SpawnRandomItem();
				if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
				{
					SpawnedCoinCount++;
				}
			}
		}
	}

	UpdateHUD();
	
	GetWorldTimerManager().SetTimer(
		WaveTimer,
		this,
		&ASpartaGameState::OnWaveTimedUp,
		WaveDuration,
		false);

	UE_LOG(LogTemp,Warning,TEXT("Level: %d SpawnCoin: %d"),CurrentWave+1, SpawnedCoinCount);
}
void ASpartaGameState::StartLevel()
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance))
		{
			CurrentLevelIndex = SpartaGameInstance->CurrentLevelIndex; 
		}
	}
	
	StartWave();	
}

void ASpartaGameState::OnGameOver()
{
	if (APlayerController*  PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			SpartaPlayerController->ShowMainMenu(true);
			SpartaPlayerController->SetPause(true);
		}
	}
}

void ASpartaGameState::OnCoinCollected()
{
	CollectedCoinCount++;
	UE_LOG(LogTemp,Warning,TEXT("CollectedCoin: %d / %d "),CollectedCoinCount, SpawnedCoinCount);

	if (CollectedCoinCount >= SpawnedCoinCount && SpawnedCoinCount > 0 )
		EndWave();
}

void ASpartaGameState::UpdateHUD()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = SpartaPlayerController->GetHudWidget())
			{
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("TimeValue"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(WaveTimer);

					TimeText->SetText(FText::FromString(FString::Printf(TEXT("%.1f sec"), RemainingTime)));
				}

				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("ScoreValue"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
						if (SpartaGameInstance)
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score : %d"), SpartaGameInstance->Score)));
						}
					}
				}

				if (UTextBlock* LevelText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("LevelValue"))))
				{
					LevelText->SetText(FText::FromString(FString::Printf(TEXT("Level : %d"), CurrentLevelIndex+1)));
				}

				if (UTextBlock* WaveText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("WaveValue"))))
				{
					WaveText->SetText(FText::FromString(FString::Printf(TEXT("Wave : %d / %d"), CurrentWave, MaxWave)));
				}
			}
		}
	}
}

void ASpartaGameState::EndLevel()
{
	CurrentLevelIndex++;

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
		if (SpartaGameInstance)
		{
			SpartaGameInstance->AddToScore(Score);
			SpartaGameInstance->CurrentLevelIndex = CurrentLevelIndex;
		}
	}

	if (CurrentLevelIndex>=MaxLevels)
	{
		OnGameOver();
		return;
	}

	if (LevelNames.IsValidIndex(CurrentLevelIndex))
	{
		UGameplayStatics::OpenLevel(GetWorld(),LevelNames[CurrentLevelIndex]);
	}
	else
	{
		OnGameOver();
	}
}

void ASpartaGameState::EndWave()
{
	CurrentWave++;
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
		if (SpartaGameInstance)
		{
			SpartaGameInstance->AddToScore(Score);
		}
	}
	NextWave();
}

void ASpartaGameState::NextWave()
{
	if (CurrentWave<MaxWave)
		StartWave();
	else
	{
		EndLevel();
	}
}
