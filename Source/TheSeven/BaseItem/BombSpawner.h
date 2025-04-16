#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BombSpawner.generated.h"

class UBoxComponent;
class ABombItem;

UCLASS()
class THESEVEN_API ABombSpawner : public AActor
{
    GENERATED_BODY()

public:
    ABombSpawner();

protected:
    virtual void BeginPlay() override;

    void SpawnBomb();


    FTimerHandle SpawnTimerHandle;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawner")
    UBoxComponent* SpawnVolume;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
    TSubclassOf<ABombItem> BombItemClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
    float SpawnInterval;
};
