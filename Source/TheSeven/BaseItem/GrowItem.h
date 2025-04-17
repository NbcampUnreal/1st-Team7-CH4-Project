// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrowItem.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UParticleSystem;
class USoundBase;

UCLASS()
class THESEVEN_API AGrowItem : public AActor
{
	GENERATED_BODY()
public:
	AGrowItem();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grow Item")
	float GrowthScale = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grow Item")
	float GrowDuration = 5.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grow Item")
	USphereComponent* CollisionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grow Item")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UParticleSystem* GrowEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	USoundBase* GrowSound;
};
