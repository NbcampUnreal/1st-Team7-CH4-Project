#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "BatItem.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UParticleSystem;
class USoundBase;
class ACharacter;

UCLASS()
class ABatItem : public ABaseItem
{
    GENERATED_BODY()

public:
    ABatItem();

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnPickupOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnSwingHitOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    void DisableSwingHitBox();

public:
    UFUNCTION(BlueprintCallable, Category = "Bat")
    void UseBatItem();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent* PickupCollision;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent* SwingHitBox;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bat")
    float KnockbackStrength;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bat")
    float SwingActiveDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    UParticleSystem* SwingEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    USoundBase* SwingSound;

private:
    FTimerHandle SwingTimerHandle;
    FTimerHandle EffectTimerHandle;
};
