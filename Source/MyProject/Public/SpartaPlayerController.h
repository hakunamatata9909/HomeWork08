// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "SpartaPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

/**
 * 
 */
UCLASS()
class MYPROJECT_API ASpartaPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ASpartaPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> HUDWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD")
	UUserWidget* HUDWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD")
	UUserWidget* MainMenuWidgetInstance;
	
	UFUNCTION(BlueprintPure,Category = "HUD")
	UUserWidget* GetHudWidget() const;

	UFUNCTION(BlueprintCallable,Category = "HUD")
	void ShowGameHud();
	UFUNCTION(BlueprintCallable,Category = "Menu")
	void ShowMainMenu(bool bIsRestart);
	UFUNCTION(BlueprintCallable,Category = "Menu")
	void StartGame();
protected:
	virtual void BeginPlay() override;
};
