// Copyright Epic Games, Inc. All Rights Reserved.

#include "GrowItem.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AGrowItem::AGrowItem()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->InitSphereRadius(50.0f);
	CollisionSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RootComponent = CollisionSphere;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	GrowEffect = nullptr;
	GrowSound = nullptr;

	ItemType = EItemType::Grow;
}

void AGrowItem::BeginPlay()
{
	Super::BeginPlay();
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AGrowItem::OnOverlapBegin);
}

void AGrowItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* Character = Cast<ACharacter>(OtherActor);
	if (Character == nullptr)
	{
		return;
	}

	const FVector OriginalScale = Character->GetActorScale3D();


	if (GrowEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), GrowEffect, Character->GetActorLocation(), FRotator::ZeroRotator, FVector(1.f), true);
	}
	if (GrowSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), GrowSound, Character->GetActorLocation());
	}

	Character->SetActorScale3D(OriginalScale * GrowthScale);
	UE_LOG(LogTemp, Warning, TEXT("%s has grown!"), *Character->GetName());

	FTimerHandle ResetTimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([=]()
		{
			if (IsValid(Character))
			{
				Character->SetActorScale3D(OriginalScale);
				UE_LOG(LogTemp, Warning, TEXT("%s has returned to normal size."), *Character->GetName());
			}
		});

	GetWorldTimerManager().SetTimer(ResetTimerHandle, TimerDelegate, GrowDuration, false);

	Destroy();
}
