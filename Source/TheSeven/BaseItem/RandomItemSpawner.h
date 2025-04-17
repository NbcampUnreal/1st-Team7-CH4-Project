

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RandomItemSpawner.generated.h"

UCLASS()
class THESEVEN_API ARandomItemSpawner : public AActor
{
	GENERATED_BODY()

public:
	ARandomItemSpawner();

protected:
	virtual void BeginPlay() override;


	UFUNCTION()
	void SpawnRandomItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Spawner")
	TArray<TSubclassOf<AActor>> ItemClasses;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Spawner")
	float SpawnInterval = 5.0f;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Spawner")
	class UBoxComponent* SpawnVolume;

	FTimerHandle SpawnTimerHandle;
};
