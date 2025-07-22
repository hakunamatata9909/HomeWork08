#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSpawnRow.h"
#include "SpawnVolume.generated.h"


class UBoxComponent;

UCLASS()
class MYPROJECT_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolume();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawnVolume")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawnVolume")
	UBoxComponent* SpawnBox;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnVolume")
	UDataTable* ItemDataTable;

	UFUNCTION(BlueprintCallable, Category = "SpawnVolume")
	AActor* SpawnRandomItem();

	FItemSpawnRow* GetRandomItem() const;
	FVector GetRandomPointInVolume() const;
	AActor* SpawnItem(TSubclassOf<AActor> ItemClass);
	
};
