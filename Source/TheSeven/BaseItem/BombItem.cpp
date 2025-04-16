#include "BombItem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/PlayerController.h"

ABombItem::ABombItem()
{
    PrimaryActorTick.bCanEverTick = false;

    PickupCollision = CreateDefaultSubobject<USphereComponent>(TEXT("PickupCollision"));
    PickupCollision->InitSphereRadius(30.f);
    PickupCollision->SetCollisionProfileName(TEXT("OverlapAll"));
    RootComponent = PickupCollision;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);
    MeshComponent->SetSimulatePhysics(false);

    BombCollision = CreateDefaultSubobject<USphereComponent>(TEXT("BombCollision"));
    BombCollision->InitSphereRadius(30.f);
    BombCollision->SetCollisionProfileName(TEXT("OverlapAll"));
    BombCollision->SetupAttachment(RootComponent);

    ExplosionRadius = 300.f;
    Damage = 30.f;
    bIsPickedUp = false;
    bIsThrown = false;

    ExplosionEffect = nullptr;
    ExplosionSound = nullptr;
}

void ABombItem::BeginPlay()
{
    Super::BeginPlay();

    PickupCollision->OnComponentBeginOverlap.AddDynamic(this, &ABombItem::OnPickupOverlap);
    BombCollision->OnComponentBeginOverlap.AddDynamic(this, &ABombItem::OnBombOverlap);
    BombCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABombItem::OnPickupOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == this || bIsPickedUp)
        return;

    ACharacter* PlayerChar = Cast<ACharacter>(OtherActor);
    if (PlayerChar)
    {
        bIsPickedUp = true;
        SetOwner(PlayerChar);

        if (USkeletalMeshComponent* PlayerMesh = PlayerChar->GetMesh())
        {
            FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, true);
            AttachToComponent(PlayerMesh, Rules, FName("hand_r"));
        }

        PickupCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        PickupCollision->SetGenerateOverlapEvents(false);
        BombCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        MeshComponent->SetSimulatePhysics(false);

        if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
        {
            EnableInput(PC);
            if (InputComponent)
            {
                InputComponent->BindAction("ThrowBomb", IE_Pressed, this, &ABombItem::ThrowBomb);
            }
        }
    }
}

void ABombItem::OnBombOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == this || !bIsThrown)
        return;

    if (Cast<ACharacter>(OtherActor))
    {
        Explode();
    }
}

void ABombItem::ThrowBomb()
{
    if (!bIsPickedUp || bIsThrown)
        return;

    bIsThrown = true;
    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

    if (ACharacter* OwnerChar = Cast<ACharacter>(GetOwner()))
        BombCollision->IgnoreActorWhenMoving(OwnerChar, true);

    MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    MeshComponent->SetSimulatePhysics(true);

    FVector ThrowDir = FVector::ZeroVector;
    if (ACharacter* OwnerChar = Cast<ACharacter>(GetOwner()))
    {
        ThrowDir = OwnerChar->GetActorForwardVector();
        ThrowDir += FVector(0.f, 0.f, 0.2f);
    }
    ThrowDir.Normalize();

    MeshComponent->SetPhysicsLinearVelocity(ThrowDir * 1500.f);

    FTimerHandle EnableCollisionHandle;
    GetWorld()->GetTimerManager().SetTimer(EnableCollisionHandle, [this]()
        {
            BombCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        }, 0.1f, false);

    FTimerHandle StopIgnoreHandle;
    GetWorld()->GetTimerManager().SetTimer(StopIgnoreHandle, [this]()
        {
            if (ACharacter* OwnerChar = Cast<ACharacter>(GetOwner()))
                BombCollision->IgnoreActorWhenMoving(OwnerChar, false);
        }, 0.3f, false);
}

void ABombItem::Explode()
{
    if (ExplosionEffect)
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());

    if (ExplosionSound)
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());

    ApplyDamage();
    Destroy();
}

void ABombItem::ApplyDamage()
{
    TArray<AActor*> OverlappingActors;
    BombCollision->GetOverlappingActors(OverlappingActors, ACharacter::StaticClass());
    for (AActor* Actor : OverlappingActors)
    {
        ACharacter* Char = Cast<ACharacter>(Actor);
        if (Char && FVector::Dist(GetActorLocation(), Char->GetActorLocation()) <= ExplosionRadius)
        {
            UGameplayStatics::ApplyDamage(Char, Damage, GetInstigatorController(), this, nullptr);
        }
    }
}
