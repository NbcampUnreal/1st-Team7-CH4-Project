// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IceBombItem.generated.h"

UCLASS()
class THESEVEN_API AIceBombItem : public AActor
{
	GENERATED_BODY()

public:
	AIceBombItem();

protected:
	virtual void BeginPlay() override;


	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ice Bomb")
	float FreezeDuration = 3.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USphereComponent* CollisionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* MeshComponent;
};
