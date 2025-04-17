

#include "RandomItemSpawner.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "TimerManager.h"

ARandomItemSpawner::ARandomItemSpawner()
{
	PrimaryActorTick.bCanEverTick = false;


	SpawnVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnVolume"));

	SpawnVolume->SetBoxExtent(FVector(500.0f, 500.0f, 200.0f));
	RootComponent = SpawnVolume;
}

void ARandomItemSpawner::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ARandomItemSpawner::SpawnRandomItem, SpawnInterval, true);
}

void ARandomItemSpawner::SpawnRandomItem()
{
	if (ItemClasses.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No ItemClasses assigned in SpawnVolume!"));
		return;
	}

	int32 RandomIndex = FMath::RandRange(0, ItemClasses.Num() - 1);
	TSubclassOf<AActor> SelectedItemClass = ItemClasses[RandomIndex];

	if (SelectedItemClass)
	{
		FVector BoxCenter = SpawnVolume->GetComponentLocation();
		FVector BoxExtent = SpawnVolume->GetScaledBoxExtent();
		FVector RandomLocation = UKismetMathLibrary::RandomPointInBoundingBox(BoxCenter, BoxExtent);

		FRotator SpawnRotation = FRotator::ZeroRotator;

		FActorSpawnParameters SpawnParams;
		AActor* SpawnedItem = GetWorld()->SpawnActor<AActor>(SelectedItemClass, RandomLocation, SpawnRotation, SpawnParams);
		if (SpawnedItem)
		{
			UE_LOG(LogTemp, Warning, TEXT("Spawned item %s at %s"), *SpawnedItem->GetName(), *RandomLocation.ToString());
		}
	}
}
