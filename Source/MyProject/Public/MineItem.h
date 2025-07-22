// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "MineItem.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AMineItem : public ABaseItem
{
	GENERATED_BODY()

public:
	AMineItem();

	USphereComponent* ExplosionCollision;
	
	UPROPERTY(editAnywhere, BlueprintReadWrite, Category = "Item")
	int ExplosionDelay;
	UPROPERTY(editAnywhere, BlueprintReadWrite, Category = "Item")
	int ExplosionDamage;
	UPROPERTY(editAnywhere, BlueprintReadWrite, Category = "Item")
	int ExplosionRadius;

	UPROPERTY(editAnywhere, BlueprintReadWrite, Category = "Effect")
	UParticleSystem* Particle;
	UPROPERTY(editAnywhere, BlueprintReadWrite, Category = "Effect")
	USoundBase* Sound;
	 
	virtual void ActivateItem(AActor* Activator) override;
	void Explode();
	
protected:
	FTimerHandle ExplodeTimer;
	bool bHasExploded;
};
