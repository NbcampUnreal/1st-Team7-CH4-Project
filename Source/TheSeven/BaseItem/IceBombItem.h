// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IceBombItem.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UParticleSystem;
class USoundBase;

UCLASS()
class THESEVEN_API AIceBombItem : public AActor
{
	GENERATED_BODY()

public:
	AIceBombItem();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPickupOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBombOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void FreezeCharacter(ACharacter* Character);

public:
	UFUNCTION(BlueprintCallable)
	void ThrowIceBomb();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* PickupCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* BombCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ice Bomb")
	float FreezeDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UParticleSystem* IceEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	USoundBase* IceSound;

private:
	bool bIsPickedUp;
	bool bIsThrown;
};
