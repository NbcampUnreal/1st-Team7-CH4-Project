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
    MeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

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

    ItemType = EItemType::Bomb;
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
    if (OtherActor == GetOwner() || !OtherActor || OtherActor == this || bIsPickedUp)
        return;

    ACharacter* PlayerChar = Cast<ACharacter>(OtherActor);
    if (!PlayerChar)
        return;

    
    TArray<AActor*> AttachedActors;
    PlayerChar->GetAttachedActors(AttachedActors);
    for (AActor* Attached : AttachedActors)
    {
        if (Attached && Attached->IsA(ABombItem::StaticClass()))
            return;
    }

    bIsPickedUp = true;
    SetOwner(PlayerChar);

    if (USkeletalMeshComponent* PlayerMesh = PlayerChar->GetMesh())
    {
        FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, true);
        AttachToComponent(PlayerMesh, Rules, FName("hand_l"));
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
            InputComponent->BindAction("ThrowBomb", IE_Pressed, this, &ABombItem::ThrowBomb);
    }
}

void ABombItem::OnBombOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor == GetOwner() || !OtherActor || OtherActor == this || !bIsThrown)
        return;

    if (Cast<ACharacter>(OtherActor))
        Explode();
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
    MeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

    FVector ThrowDir = FVector::ZeroVector;
    if (ACharacter* OwnerChar = Cast<ACharacter>(GetOwner()))
    {
        ThrowDir = OwnerChar->GetActorForwardVector() + FVector(0.f, 0.f, 0.2f);
        ThrowDir.Normalize();
    }
    MeshComponent->SetPhysicsLinearVelocity(ThrowDir * 1500.f);

    BombCollision->SetGenerateOverlapEvents(true);
    BombCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

    FTimerHandle StopIgnoreHandle;
    GetWorld()->GetTimerManager().SetTimer(
        StopIgnoreHandle,
        [this]()
        {
            if (ACharacter* OwnerChar = Cast<ACharacter>(GetOwner()))
                BombCollision->IgnoreActorWhenMoving(OwnerChar, false);
        },
        0.5f,
        false
    );
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
    TArray<AActor*> Overlapping;
    BombCollision->GetOverlappingActors(Overlapping, ACharacter::StaticClass());

    for (AActor* Actor : Overlapping)
    {
        ACharacter* Char = Cast<ACharacter>(Actor);
        if (!Char || Char == GetOwner())
            continue;

        if (FVector::Dist(GetActorLocation(), Char->GetActorLocation()) <= ExplosionRadius)
            UGameplayStatics::ApplyDamage(Char, Damage, GetInstigatorController(), this, nullptr);
    }
}
