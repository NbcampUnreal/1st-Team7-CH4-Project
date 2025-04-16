#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BombItem.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class ACharacter;

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

public:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bomb")
    USphereComponent* CollisionSphere;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bomb")
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bomb")
    float ExplosionRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bomb")
    float Damage;
};
