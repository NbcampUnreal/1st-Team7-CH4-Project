// Copyright Epic Games, Inc. All Rights Reserved.

#include "InkItem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"

AInkItem::AInkItem()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->InitSphereRadius(50.0f); 
	CollisionSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RootComponent = CollisionSphere;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
}

void AInkItem::BeginPlay()
{
	Super::BeginPlay();


	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AInkItem::OnOverlapBegin);
}

void AInkItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	ACharacter* Character = Cast<ACharacter>(OtherActor);
	if (!Character)
	{
		return;
	}

	APlayerController* PC = Cast<APlayerController>(Character->GetController());
	if (PC && InkWidgetClass)
	{
\
		UUserWidget* InkWidget = CreateWidget<UUserWidget>(PC, InkWidgetClass);
		if (InkWidget)
		{
			InkWidget->AddToViewport();
		}

\
		FTimerHandle TimerHandle;
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindLambda([=]()
			{
				if (InkWidget)
				{
					InkWidget->RemoveFromParent();
				}
			});
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, InkEffectDuration, false);
	}

	UE_LOG(LogTemp, Warning, TEXT("Ink effect activated on %s for %f seconds."), *Character->GetName(), InkEffectDuration);


	Destroy();
}