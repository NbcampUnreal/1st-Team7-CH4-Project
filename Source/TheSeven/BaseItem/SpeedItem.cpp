
// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpeedItem.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

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
		UCharacterMovementComponent* MovementComp = OverlappedCharacter->GetCharacterMovement();
		if (MovementComp != nullptr)
		{
			MovementComp->MaxWalkSpeed *= SpeedMultiplier;
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
		UCharacterMovementComponent* MovementComp = OverlappedCharacter->GetCharacterMovement();
		if (MovementComp != nullptr)
		{
			MovementComp->MaxWalkSpeed /= SpeedMultiplier;
		}
	}

	Destroy();
}
