

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseItem.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Bomb	UMETA(DisplayName = "Bomb"),
	IceBomb UMETA(DisplayName = "IceBomb"),
	Bat		UMETA(DisplayName = "Bat"),
	Grow	UMETA(DisplayName = "Grow"),
	Ink		UMETA(DisplayName = "Ink"),
	Speed	UMETA(DisplayName = "Speed")
};

UCLASS()
class THESEVEN_API ABaseItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType ItemType;
};
