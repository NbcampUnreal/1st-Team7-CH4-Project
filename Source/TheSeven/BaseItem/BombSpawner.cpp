#include "BombSpawner.h"
#include "Components/BoxComponent.h"
#include "BombItem.h" 
#include "Engine/World.h"
#include "TimerManager.h"

ABombSpawner::ABombSpawner()
{
    PrimaryActorTick.bCanEverTick = false;

    SpawnVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnVolume"));
    RootComponent = SpawnVolume;

    SpawnInterval = 3.0f;
}

void ABombSpawner::BeginPlay()
{
    Super::BeginPlay();

    GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ABombSpawner::SpawnBomb, SpawnInterval, true);
}

void ABombSpawner::SpawnBomb()
{
    if (!BombItemClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("BombSpawner: BombItemClass is not set!"));
        return;
    }

    FVector Origin = SpawnVolume->Bounds.Origin;
    FVector Extent = SpawnVolume->Bounds.BoxExtent;

    FVector SpawnLocation = Origin + FVector(
        FMath::FRandRange(-Extent.X, Extent.X),
        FMath::FRandRange(-Extent.Y, Extent.Y),
        FMath::FRandRange(-Extent.Z, Extent.Z)
    );
    SpawnLocation.Z = Origin.Z;

    FRotator SpawnRotation = FRotator::ZeroRotator;

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;

    ABombItem* SpawnedBomb = GetWorld()->SpawnActor<ABombItem>(BombItemClass, SpawnLocation, SpawnRotation, SpawnParams);
    if (SpawnedBomb)
    {
        UE_LOG(LogTemp, Warning, TEXT("BombSpawner: Spawned Bomb at %s"), *SpawnLocation.ToString());
        
        SpawnedBomb->MeshComponent->SetSimulatePhysics(true);
        SpawnedBomb->MeshComponent->SetEnableGravity(true);
    }
}
