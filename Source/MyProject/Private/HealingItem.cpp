// Fill out your copyright notice in the Description page of Project Settings.


#include "HealingItem.h"
#include "SpartaCharacter.h"

AHealingItem :: AHealingItem()
{
	HealAMount = 20.0f;
	ItemType = "Healing";
}

void AHealingItem :: ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	if (Activator && Activator -> ActorHasTag("Player"))
	{
		if (ASpartaCharacter* playerCharacter = Cast<ASpartaCharacter>(Activator))
		{
			playerCharacter->AddHealth(HealAMount);
		}
		
		DestroyItem();
	}
}