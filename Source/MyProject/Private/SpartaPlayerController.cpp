// Fill out your copyright notice in the Description page of Project Settings.


#include "SpartaPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "SpartaGameInstance.h"
#include "SpartaGameState.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

ASpartaPlayerController::ASpartaPlayerController()
	:InputMappingContext(nullptr),
	MoveAction(nullptr),
	JumpAction(nullptr),
	LookAction(nullptr),
	SprintAction(nullptr),
	HUDWidgetClass(nullptr),
	MainMenuWidgetClass(nullptr),
	MainMenuWidgetInstance(nullptr)
{
}

void ASpartaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* System =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				System->AddMappingContext(InputMappingContext, 0);
			}
		}
	}

	FString CurrentMapName = GetWorld()->GetMapName();
	if (CurrentMapName.Contains("MenuLevel"))
	{
		ShowMainMenu(false);
	}
}

UUserWidget* ASpartaPlayerController::GetHudWidget() const
{
	return HUDWidgetInstance;
}

void ASpartaPlayerController::ShowMainMenu(bool bIsRestart)
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromViewport();
		HUDWidgetInstance = nullptr;
	}

	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromViewport();
		MainMenuWidgetInstance = nullptr;
	}

	if (MainMenuWidgetClass)
	{
		MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidgetInstance)
		{
			MainMenuWidgetInstance->AddToViewport();
			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
			UE_LOG(LogTemp,Warning,TEXT("BEFORE BUTTONTEXT CASTING"));
		}

		if (UTextBlock* ButtonText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("ButtonText"))))
		{
			UE_LOG(LogTemp,Warning,TEXT("AFTER BUTTONTEXT CASTING"));

			if (bIsRestart)
			{
				ButtonText->SetText(FText::FromString(TEXT("Restart")));
			}
			else
			{
				ButtonText->SetText(FText::FromString(TEXT("start")));
			}

			if (bIsRestart)
			{
				UFunction* PlayAnimFunc = MainMenuWidgetInstance->FindFunction(FName("PlayGameOverAnim"));
				if (PlayAnimFunc)
				{
					MainMenuWidgetInstance->ProcessEvent(PlayAnimFunc, nullptr);
				}

				UTextBlock* TotalScroeTextBlock = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("TotalScoreText")));
				if (TotalScroeTextBlock)
				{
					USpartaGameInstance* GameInstance = Cast<USpartaGameInstance>(UGameplayStatics::GetGameInstance(this));
					int32 TotalScore = GameInstance->Score;
					TotalScroeTextBlock->SetText(FText::FromString(FString::Printf(TEXT("TotalScore : %d"),TotalScore)));
				}
			}
		}
		
	}
		
}

void ASpartaPlayerController::ShowGameHud()
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromViewport(); 
		HUDWidgetInstance = nullptr;
	}

	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromViewport();
		MainMenuWidgetInstance = nullptr;
	}

	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();
			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());
		}
	}

	ASpartaGameState* SpartaGameState = GetWorld() ? GetWorld()->GetGameState<ASpartaGameState>():nullptr;

	if (SpartaGameState)
	{
		SpartaGameState->UpdateHUD();
	}
}

void ASpartaPlayerController::StartGame()
{
	if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GetWorld()->GetGameInstance()))
	{
		SpartaGameInstance->CurrentLevelIndex=0;
		SpartaGameInstance->Score=0;
	}

	UGameplayStatics::OpenLevel(GetWorld(),"BasicLevel");
	SetPause(false);
}


