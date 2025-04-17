#include "BatItem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Particles/ParticleSystemComponent.h"

ABatItem::ABatItem()
{
    PrimaryActorTick.bCanEverTick = false;

    PickupCollision = CreateDefaultSubobject<USphereComponent>(TEXT("PickupCollision"));
    RootComponent = PickupCollision;
    PickupCollision->InitSphereRadius(100.f);
    PickupCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

    SwingHitBox = CreateDefaultSubobject<USphereComponent>(TEXT("SwingHitBox"));
    SwingHitBox->SetupAttachment(RootComponent);
    SwingHitBox->InitSphereRadius(150.f);
    SwingHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    SwingHitBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);

    KnockbackStrength = 1000.f;
    SwingActiveDuration = 0.2f;

    SwingEffect = nullptr;
    SwingSound = nullptr;
}

void ABatItem::BeginPlay()
{
    Super::BeginPlay();

    PickupCollision->OnComponentBeginOverlap.AddDynamic(this, &ABatItem::OnPickupOverlap);
    SwingHitBox->OnComponentBeginOverlap.AddDynamic(this, &ABatItem::OnSwingHitOverlap);
}

void ABatItem::OnPickupOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == this)
    {
        return;
    }

    ACharacter* PlayerChar = Cast<ACharacter>(OtherActor);
    if (PlayerChar)
    {
        UE_LOG(LogTemp, Warning, TEXT("Bat item picked up by %s"), *PlayerChar->GetName());

        SetOwner(PlayerChar);

        if (USkeletalMeshComponent* PlayerMesh = PlayerChar->GetMesh())
        {
            FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, true);
            AttachToComponent(PlayerMesh, Rules, FName("hand_r"));
        }

        PickupCollision->SetGenerateOverlapEvents(false);
        PickupCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

        MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        MeshComponent->SetSimulatePhysics(false);
    }
}

void ABatItem::OnSwingHitOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == this)
    {
        return;
    }

    if (OtherActor == GetOwner())
    {
        UE_LOG(LogTemp, Warning, TEXT("SwingHit => Overlap with owner => ignore"));
        return;
    }

    ACharacter* HitEnemy = Cast<ACharacter>(OtherActor);
    if (HitEnemy && HitEnemy->GetCharacterMovement())
    {
        UE_LOG(LogTemp, Warning, TEXT("HitEnemy => %s"), *HitEnemy->GetName());

        FVector LaunchDir = (HitEnemy->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        LaunchDir.Z += 1.0f;
        LaunchDir.Normalize();

        float LaunchPower = 5000.f;
        HitEnemy->LaunchCharacter(LaunchDir * LaunchPower, true, true);
    }
}

void ABatItem::UseBatItem()
{
    UE_LOG(LogTemp, Warning, TEXT("UseBatItem => Swing started"));

    if (SwingEffect)
    {
        UParticleSystemComponent* PSC = UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            SwingEffect,
            GetActorLocation(),
            FRotator::ZeroRotator,
            FVector(1.f), 
            true 
        );

        if (PSC)
        {
            PSC->bAutoDestroy = true;

            GetWorld()->GetTimerManager().SetTimer(
                EffectTimerHandle,
                [PSC]()
                {
                    if (PSC->IsActive())
                    {
                        PSC->DeactivateSystem();
                    }
                },
                0.1f,
                false
            );
        }
    }

    if (SwingSound)
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), SwingSound, GetActorLocation());
    }


    SwingHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

    GetWorld()->GetTimerManager().SetTimer(
        SwingTimerHandle,
        this,
        &ABatItem::DisableSwingHitBox,
        SwingActiveDuration,
        false
    );
}

void ABatItem::DisableSwingHitBox()
{
    SwingHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    UE_LOG(LogTemp, Warning, TEXT("Swing ended => Destroy bat"));

    Destroy();
}
