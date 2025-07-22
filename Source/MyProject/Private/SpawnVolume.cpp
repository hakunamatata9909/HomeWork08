// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"

#include "MineItem.h"
#include "Components/BoxComponent.h"

// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Scene = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(Scene);
	
	SpawnBox = CreateDefaultSubobject<UBoxComponent>("SpawnBox");
	SpawnBox->SetupAttachment(Scene);

	ItemDataTable = nullptr;
}

FVector ASpawnVolume::GetRandomPointInVolume() const
{
	FVector BoxExtent= SpawnBox->GetScaledBoxExtent();
	FVector BoxOrigin = SpawnBox->GetComponentLocation();
	
	return BoxOrigin + FVector(
		FMath::FRandRange(-BoxExtent.X,BoxExtent.X),
		FMath::FRandRange(-BoxExtent.Y,BoxExtent.Y),
		FMath::FRandRange(-BoxExtent.Z,BoxExtent.Z)
		);
}

AActor* ASpawnVolume::SpawnItem(TSubclassOf<AActor> ItemClass)
{
	if (!ItemClass) return nullptr;

	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(
		ItemClass,
		GetRandomPointInVolume(),
		FRotator::ZeroRotator);

	return SpawnedActor;	
}

AActor* ASpawnVolume::SpawnRandomItem()
{
	if (FItemSpawnRow* SelectedRow = GetRandomItem())
	{
		if (UClass* ActualClass = SelectedRow -> ItemClass.Get())
		{
			return SpawnItem(ActualClass);
		}
	}

	return nullptr;
}

FItemSpawnRow* ASpawnVolume::GetRandomItem() const
{
	if (!ItemDataTable) return nullptr;

	TArray<FItemSpawnRow*> AllRows;
	static const FString ContextString(TEXT("ItemSpawnContext"));
	ItemDataTable->GetAllRows(ContextString,AllRows);

	if (AllRows.IsEmpty())
		return nullptr;

	float TotalPercent=0.0f;

	for (const FItemSpawnRow* Row : AllRows)
	{
		if (Row)
		{
			TotalPercent+=Row->SpawnChance;
		}
	}

	const float RandValue = FMath::RandRange(0.0f,TotalPercent);
	float AccumulatePercent = 0.0f;
	
	for (FItemSpawnRow* Row : AllRows)
	{
		AccumulatePercent+=Row->SpawnChance;
		if (RandValue < AccumulatePercent)
		{
			return Row;
		}
	}

	return nullptr;
}






// Called when the game starts or when spawneds

