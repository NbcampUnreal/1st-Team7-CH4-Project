#include "BombItem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

ABombItem::ABombItem()
{
    PrimaryActorTick.bCanEverTick = false;


    CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    CollisionSphere->InitSphereRadius(30.f);
    CollisionSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    RootComponent = CollisionSphere;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    ExplosionRadius = 300.f;
    Damage = 30.f;

    CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ABombItem::OnOverlapBegin);
}

void ABombItem::BeginPlay()
{
    Super::BeginPlay();
}

void ABombItem::OnOverlapBegin(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == this)
    {
        return;
    }

    if (Cast<ACharacter>(OtherActor))
    {
        UE_LOG(LogTemp, Warning, TEXT("BombItem => Overlapped by %s, Exploding..."), *OtherActor->GetName());
        Explode();
    }
}

void ABombItem::Explode()
{
    UE_LOG(LogTemp, Warning, TEXT("BombItem => Explode!"));


    UGameplayStatics::ApplyRadialDamage(
        this,                    
        Damage,                 
        GetActorLocation(),      
        ExplosionRadius,       
        nullptr,                 
        TArray<AActor*>(),       
        this,               
        GetInstigatorController(), 
        true               
    );

    Destroy();
}
