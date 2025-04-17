
// Copyright Epic Games, Inc. All Rights Reserved.

#include "IceBombItem.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AIceBombItem::AIceBombItem()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->InitSphereRadius(50.0f);
	CollisionSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RootComponent = CollisionSphere;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	IceEffect = nullptr;
	IceSound = nullptr;
}

void AIceBombItem::BeginPlay()
{
	Super::BeginPlay();
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AIceBombItem::OnOverlapBegin);
}

void AIceBombItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* Character = Cast<ACharacter>(OtherActor);
	if (!Character)
	{
		return;
	}

	if (IceEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), IceEffect, Character->GetActorLocation(), FRotator::ZeroRotator, FVector(1.f), true);
	}
	if (IceSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), IceSound, Character->GetActorLocation());
	}

	if (Character->GetCharacterMovement())
	{
		Character->GetCharacterMovement()->DisableMovement();
		UE_LOG(LogTemp, Warning, TEXT("%s movement disabled."), *Character->GetName());
	}

	Character->CustomTimeDilation = 0.0f;
	UE_LOG(LogTemp, Warning, TEXT("%s is fully frozen."), *Character->GetName());

	FTimerHandle UnfreezeTimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([=]()
		{
			if (IsValid(Character) && Character->GetCharacterMovement())
			{
				Character->CustomTimeDilation = 1.0f;
				Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
				UE_LOG(LogTemp, Warning, TEXT("%s is unfrozen."), *Character->GetName());
			}
		});
	GetWorld()->GetTimerManager().SetTimer(UnfreezeTimerHandle, TimerDelegate, FreezeDuration, false);

	Destroy();
}
