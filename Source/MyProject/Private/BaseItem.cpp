#include "BaseItem.h"
#include "Components/SphereComponent.h"
#include "kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ABaseItem::ABaseItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(Scene);

	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereCollision -> SetCollisionProfileName((TEXT("OverlapAllDynamic")));
	SphereCollision -> SetupAttachment(Scene);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh -> SetupAttachment(SphereCollision);

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this,&ABaseItem::OnItemOverlap);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &ABaseItem::OnItemEndOverlap);
}

void ABaseItem:: OnItemOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		ActivateItem(OtherActor);
	}
}
void ABaseItem:: OnItemEndOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	
}
void ABaseItem:: ActivateItem(AActor* Activator)
{
	UParticleSystemComponent* Pickup = nullptr;
	if (PickupParticle)
	{
		Pickup=UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
			PickupParticle,
			GetActorLocation(),
			GetActorRotation(),
			true
			);
	}
	UE_LOG(LogTemp,Warning,TEXT("Pickup activated"));
	if (PickupSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(),
			PickupSound,
			GetActorLocation(),
			GetActorRotation()
			);
	}

	if (Pickup)
	{
		FTimerHandle TimerHandle;
		TWeakObjectPtr<UParticleSystemComponent> WeakPickup = Pickup;
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			[WeakPickup]()
			{
				if (WeakPickup.IsValid())
				WeakPickup->DestroyComponent();
			},
			2.0f,
			false
			);
	}
}

FName ABaseItem:: GetItemType() const
{
	return ItemType;
}

void ABaseItem::DestroyItem()
{
	Destroy();
}

// Called when the game starts or when spawned
void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

} // Fill out your copyright notice in the Description page of Project Settings.

