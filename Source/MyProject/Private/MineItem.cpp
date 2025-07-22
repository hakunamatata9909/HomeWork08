// Fill out your copyright notice in the Description page of Project Settings.


#include "MineItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

AMineItem::AMineItem()
{
	ExplosionDelay = 5.0f;
	ExplosionDamage = 34.0f;
	ExplosionRadius = 300.0f;
	ItemType = "Mine";

	ExplosionCollision = CreateDefaultSubobject<USphereComponent>("ExplosionCollision");
	ExplosionCollision ->InitSphereRadius(ExplosionRadius);
	ExplosionCollision ->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ExplosionCollision->SetupAttachment(RootComponent);
}

void AMineItem :: ActivateItem(AActor* Activator)
{
	GetWorld()->GetTimerManager().SetTimer(ExplodeTimer, this, &AMineItem::Explode,ExplosionDelay, false);
}

void AMineItem::Explode()
{
	TArray<AActor*> OverlappingActors;
	ExplosionCollision->GetOverlappingActors(OverlappingActors);

	for (auto* a : OverlappingActors)
	{
		if (a && a->ActorHasTag("Player"))
		{
			UGameplayStatics::ApplyDamage(
				a,
				ExplosionDamage,
				nullptr,
				this,
				UDamageType::StaticClass());
		}
	}

	DestroyItem();
}
