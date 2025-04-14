// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BombItem.generated.h"

UCLASS()
class THESEVEN_API ABombItem : public AActor
{
	GENERATED_BODY()

public:
	ABombItem();

protected:
	virtual void BeginPlay() override;


	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);


	void Explode();


	void ApplyDamage();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bomb")
	float ExplosionRadius = 300.f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bomb")
	float Damage = 30.f;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bomb")
	class USphereComponent* CollisionSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bomb")
	class UStaticMeshComponent* MeshComponent;
};
