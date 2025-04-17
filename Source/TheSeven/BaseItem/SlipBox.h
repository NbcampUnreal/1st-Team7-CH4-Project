#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SlipBox.generated.h"

UCLASS()
class THESEVEN_API ASlipBox : public AActor
{
	GENERATED_BODY()

public:
	ASlipBox();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SlipBox")
	class UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SlipBox")
	float SlideBrakingFrictionFactor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SlipBox")
	float SlideGroundFriction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SlipBox")
	float SlideBrakingDecelerationWalking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SlipBox")
	float DefaultBrakingFrictionFactor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SlipBox")
	float DefaultGroundFriction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SlipBox")
	float DefaultBrakingDecelerationWalking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SlipBox")
	float ImpulseStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SlipBox")
	float VelocityThreshold;
};
