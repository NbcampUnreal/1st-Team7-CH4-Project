
// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpeedItem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

ASpeedItem::ASpeedItem()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->InitSphereRadius(50.0f);
	CollisionSphere->SetCollisionProfileName(TEXT("OverlapAll"));
	RootComponent = CollisionSphere;

	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ASpeedItem::HandleOverlapBegin);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	SpeedMultiplier = 2.0f;
	SpeedDuration = 5.0f;

	SpeedEffect = nullptr;
	SpeedSound = nullptr;

	ItemType = EItemType::Speed;
}

void ASpeedItem::HandleOverlapBegin(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if (OtherActor == nullptr)
	{
		return;
	}

	ACharacter* OverlappedCharacter = Cast<ACharacter>(OtherActor);
	if (OverlappedCharacter != nullptr)
	{
		if (UCharacterMovementComponent* MovementComp = OverlappedCharacter->GetCharacterMovement())
		{
			MovementComp->MaxWalkSpeed *= SpeedMultiplier;
		}

		if (SpeedEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				SpeedEffect,
				OverlappedCharacter->GetActorLocation(),
				FRotator::ZeroRotator,
				FVector(1.f),
				true
			);
		}

		if (SpeedSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				GetWorld(),
				SpeedSound,
				OverlappedCharacter->GetActorLocation()
			);
		}

		GetWorldTimerManager().SetTimer(
			SpeedTimerHandle,
			FTimerDelegate::CreateUObject(this, &ASpeedItem::RestoreOriginalSpeed, OverlappedCharacter),
			SpeedDuration,
			false
		);

		SetActorEnableCollision(false);
		SetActorHiddenInGame(true);
	}
}

void ASpeedItem::RestoreOriginalSpeed(ACharacter* OverlappedCharacter)
{
	if (OverlappedCharacter != nullptr)
	{
		if (UCharacterMovementComponent* MovementComp = OverlappedCharacter->GetCharacterMovement())
		{
			MovementComp->MaxWalkSpeed /= SpeedMultiplier;
		}
	}

	Destroy();
}
