

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HeatPillarZone.generated.h"


class UBoxComponent;
class UParticleSystem;

UCLASS()
class THESEVEN_API AHeatPillarZone : public AActor
{
    GENERATED_BODY()

public:
    AHeatPillarZone();

protected:
    virtual void BeginPlay() override;


    void ActivatePillar();

    void DeactivatePillar();


    UPROPERTY(VisibleAnywhere)
    UBoxComponent* DamageZone;

    UPROPERTY(EditAnywhere, Category = "Effects")
    UParticleSystem* PillarEffect;

    FTimerHandle PillarTimerHandle;

public:

    UPROPERTY(EditAnywhere, Category = "Pillar")
    float ActiveDuration = 1.5f;

    UPROPERTY(EditAnywhere, Category = "Pillar")
    float InactiveDuration = 5.0f;

    UPROPERTY(EditAnywhere, Category = "Pillar")
    float DamageAmount = 20.f;

    UPROPERTY(EditAnywhere, Category = "Pillar")
    float ForceStrength = 800.f;
};
