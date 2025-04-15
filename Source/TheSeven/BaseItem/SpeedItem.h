// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpeedItem.generated.h"

UCLASS()
class THESEVEN_API ASpeedItem : public AActor
{
	GENERATED_BODY()

public:
	ASpeedItem();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* CollisionSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpeedSettings")
	float SpeedMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpeedSettings")
	float SpeedDuration;

	UFUNCTION()
	void HandleOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	void RestoreOriginalSpeed(class ACharacter* OverlappedCharacter);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* MeshComponent;

private:
	FTimerHandle SpeedTimerHandle;
};

