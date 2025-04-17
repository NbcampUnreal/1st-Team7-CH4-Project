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

    // ���� ��, ���� Overlap�Ǹ� �����ϴ� �Լ�
    UFUNCTION()
    void OnBombOverlap(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );

    // ���� ó�� (������ ���� �� Destroy)
    void Explode();

    // ���� ���� �� ĳ���͵鿡�� ������ ����
    void ApplyDamage();

public:
    // ��ź ������ (A Ű �Է¿� ���� ȣ��)
    UFUNCTION(BlueprintCallable, Category = "Bomb")
    void ThrowBomb();

public:
    // �ݱ� �� �浹�� ���Ǵ� Collision Sphere
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bomb")
    USphereComponent* PickupCollision;

    // ��ź�� �޽� (�ð��� ���)
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
