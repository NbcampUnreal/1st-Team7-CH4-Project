// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InkItem.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UUserWidget;
class UParticleSystem;
class USoundBase;

UCLASS()
class THESEVEN_API AInkItem : public AActor
{
    GENERATED_BODY()

public:
    AInkItem();

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
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ink")
    float InkEffectDuration = 3.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ink")
    TSubclassOf<UUserWidget> InkWidgetClass;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ink")
    USphereComponent* CollisionSphere;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ink")
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    UParticleSystem* InkEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    USoundBase* InkSound;
};
