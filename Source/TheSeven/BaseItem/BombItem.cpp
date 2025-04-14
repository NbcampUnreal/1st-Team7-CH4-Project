// Copyright Epic Games, Inc. All Rights Reserved.

#include "BombItem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

ABombItem::ABombItem()
{
	PrimaryActorTick.bCanEverTick = false;


	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->InitSphereRadius(30.0f);
	CollisionSphere->SetCollisionProfileName(TEXT("OverlapAll"));
	RootComponent = CollisionSphere;


	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);


	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ABombItem::OnOverlapBegin);

	ExplosionRadius = 300.f;
	Damage = 30.f;
}

void ABombItem::BeginPlay()
{
	Super::BeginPlay();
}

void ABombItem::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{

	if (!OtherActor)
	{
		return;
	}


	ACharacter* OverlappedCharacter = Cast<ACharacter>(OtherActor);
	if (OverlappedCharacter)
	{
		Explode();
	}
}

void ABombItem::Explode()
{
	UE_LOG(LogTemp, Warning, TEXT("Bomb exploded via overlap."));


	ApplyDamage();

	Destroy();
}

void ABombItem::ApplyDamage()
{
	TArray<AActor*> OverlappingActors;

	CollisionSphere->GetOverlappingActors(OverlappingActors, ACharacter::StaticClass());

	for (AActor* Actor : OverlappingActors)
	{
		ACharacter* Char = Cast<ACharacter>(Actor);
		if (Char)
		{

			float Distance = FVector::Dist(GetActorLocation(), Char->GetActorLocation());
			if (Distance <= ExplosionRadius)
			{
				UE_LOG(LogTemp, Warning, TEXT("Applying %.1f damage to %s"), Damage, *Char->GetName());
				UGameplayStatics::ApplyDamage(
					Char,
					Damage,
					GetInstigatorController(),
					this,
					nullptr
				);
			}
		}
	}
}
