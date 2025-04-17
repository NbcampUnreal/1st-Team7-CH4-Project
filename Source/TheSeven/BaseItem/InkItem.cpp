// Copyright Epic Games, Inc. All Rights Reserved.
#include "InkItem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

AInkItem::AInkItem()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    CollisionSphere->InitSphereRadius(50.f);
    CollisionSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    RootComponent = CollisionSphere;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);

    InkEffect = nullptr;
    InkSound = nullptr;

    ItemType = EItemType::Ink;
}

void AInkItem::BeginPlay()
{
    Super::BeginPlay();
    CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AInkItem::OnOverlapBegin);
}

void AInkItem::OnOverlapBegin(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    ACharacter* Character = Cast<ACharacter>(OtherActor);
    if (!Character)
        return;

    if (InkEffect)
    {
        UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            InkEffect,
            Character->GetActorLocation(),
            FRotator::ZeroRotator,
            FVector(1.f),
            true
        );
    }

    if (InkSound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            GetWorld(),
            InkSound,
            Character->GetActorLocation()
        );
    }

    ACharacter* PlayerPawn = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (Character == PlayerPawn && InkWidgetClass)
    {
        APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        if (PC)
        {
            UUserWidget* InkWidget = CreateWidget<UUserWidget>(PC, InkWidgetClass);
            if (InkWidget)
            {
                InkWidget->AddToViewport();
                FTimerHandle Handle;
                FTimerDelegate Delegate;
                Delegate.BindLambda([InkWidget]() { InkWidget->RemoveFromParent(); });
                GetWorld()->GetTimerManager().SetTimer(Handle, Delegate, InkEffectDuration, false);
            }
        }
    }

    Destroy();
}
