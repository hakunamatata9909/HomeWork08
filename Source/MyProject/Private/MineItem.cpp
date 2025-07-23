// Fill out your copyright notice in the Description page of Project Settings.


#include "MineItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AMineItem::AMineItem()
{
	ExplosionDelay = 5.0f;
	ExplosionDamage = 34.0f;
	ExplosionRadius = 300.0f;
	ItemType = "Mine";
	bHasExploded = false;

	ExplosionCollision = CreateDefaultSubobject<USphereComponent>("ExplosionCollision");
	ExplosionCollision ->InitSphereRadius(ExplosionRadius);
	ExplosionCollision ->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ExplosionCollision->SetupAttachment(RootComponent);
}

void AMineItem :: ActivateItem(AActor* Activator)
{
	if (bHasExploded) return;
	Super::ActivateItem(Activator);
	GetWorld()->GetTimerManager().SetTimer(ExplodeTimer, this, &AMineItem::Explode,ExplosionDelay, false);
	bHasExploded = true;
}

void AMineItem::Explode()
{
	UParticleSystemComponent* ParticleInstance = nullptr;	
	if (Particle)
	{
		ParticleInstance = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			Particle,
			GetActorLocation(),
			GetActorRotation(),
			false
			);
	}

	if (Sound)
	{
		UGameplayStatics::SpawnSoundAtLocation(
			GetWorld(),
			Sound,
			GetActorLocation(),
			GetActorRotation()
			);
	}
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

	if (ParticleInstance)
	{
		FTimerHandle TimerHandle;
		TWeakObjectPtr<UParticleSystemComponent> WeakParticle = ParticleInstance;
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			[WeakParticle]()
			{
				if (WeakParticle.IsValid())
				WeakParticle->DestroyComponent();
			},
			1.0f,
			false
			);
	}
}
