
#include "HeatPillarZone.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"

AHeatPillarZone::AHeatPillarZone()
{
    PrimaryActorTick.bCanEverTick = false;

    DamageZone = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageZone"));
    RootComponent = DamageZone;
    DamageZone->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    DamageZone->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void AHeatPillarZone::BeginPlay()
{
    Super::BeginPlay();
    ActivatePillar();
}

void AHeatPillarZone::ActivatePillar()
{
    if (PillarEffect)
    {
        UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            PillarEffect,
            GetActorLocation(),
            GetActorRotation(),
            true
        );
    }

    DamageZone->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

    TArray<AActor*> Overlaps;
    DamageZone->GetOverlappingActors(Overlaps, ACharacter::StaticClass());
    for (AActor* A : Overlaps)
    {
        ACharacter* Ch = Cast<ACharacter>(A);
        if (!Ch) continue;

        UGameplayStatics::ApplyDamage(Ch, DamageAmount, nullptr, this, nullptr);


        FVector Dir = (Ch->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        Ch->LaunchCharacter(Dir * ForceStrength + FVector(0, 0, ForceStrength * 0.2f), true, true);
    }

    GetWorld()->GetTimerManager().SetTimer(
        PillarTimerHandle,
        this,
        &AHeatPillarZone::DeactivatePillar,
        ActiveDuration,
        false
    );
}

void AHeatPillarZone::DeactivatePillar()
{
    DamageZone->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    GetWorld()->GetTimerManager().SetTimer(
        PillarTimerHandle,
        this,
        &AHeatPillarZone::ActivatePillar,
        InactiveDuration,
        false
    );
}
