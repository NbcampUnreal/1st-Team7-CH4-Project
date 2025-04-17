#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BombItem.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class ACharacter;
class UParticleSystem;
class USoundBase;

UCLASS()
class THESEVEN_API ABombItem : public AActor
{
    GENERATED_BODY()

public:
    ABombItem();

protected:
    virtual void BeginPlay() override;
    UFUNCTION()
    void OnPickupOverlap(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );

    // 던진 후, 적과 Overlap되면 폭발하는 함수
    UFUNCTION()
    void OnBombOverlap(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );

    // 폭발 처리 (데미지 적용 후 Destroy)
    void Explode();

    // 폭발 범위 내 캐릭터들에게 데미지 적용
    void ApplyDamage();

public:
    // 폭탄 던지기 (A 키 입력에 의해 호출)
    UFUNCTION(BlueprintCallable, Category = "Bomb")
    void ThrowBomb();

public:
    // 줍기 및 충돌에 사용되는 Collision Sphere
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bomb")
    USphereComponent* PickupCollision;

    // 폭탄의 메시 (시각적 요소)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bomb")
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bomb")
    USphereComponent* BombCollision;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bomb")
    float ExplosionRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bomb")
    float Damage;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bomb")
    bool bIsPickedUp;

    bool bIsThrown;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    UParticleSystem* ExplosionEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    USoundBase* ExplosionSound;
};
