#include "SlipBox.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

ASlipBox::ASlipBox()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;
	TriggerBox->InitBoxExtent(FVector(300.f, 300.f, 200.f));
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));

	SlideBrakingFrictionFactor = 0.05f;
	SlideGroundFriction = 0.05f;
	SlideBrakingDecelerationWalking = 50.f; 

	DefaultBrakingFrictionFactor = 2.0f;
	DefaultGroundFriction = 8.0f;
	DefaultBrakingDecelerationWalking = 2048.f;

	ImpulseStrength = 200.f;
	VelocityThreshold = 10.f;

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ASlipBox::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ASlipBox::OnOverlapEnd);
}

void ASlipBox::BeginPlay()
{
	Super::BeginPlay();
}

void ASlipBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		ACharacter* OverlappingCharacter = Cast<ACharacter>(OtherActor);
		if (OverlappingCharacter && OverlappingCharacter->GetCharacterMovement())
		{
			OverlappingCharacter->GetCharacterMovement()->BrakingFrictionFactor = SlideBrakingFrictionFactor;
			OverlappingCharacter->GetCharacterMovement()->GroundFriction = SlideGroundFriction;
			OverlappingCharacter->GetCharacterMovement()->BrakingDecelerationWalking = SlideBrakingDecelerationWalking;

			if (OverlappingCharacter->GetVelocity().SizeSquared() < FMath::Square(VelocityThreshold))
			{
				FVector ImpulseDirection = OverlappingCharacter->GetActorForwardVector();
				ImpulseDirection.Normalize();
				OverlappingCharacter->LaunchCharacter(ImpulseDirection * ImpulseStrength, true, true);
			}
		}
	}
}

void ASlipBox::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this)
	{
		ACharacter* OverlappingCharacter = Cast<ACharacter>(OtherActor);
		if (OverlappingCharacter && OverlappingCharacter->GetCharacterMovement())
		{
			OverlappingCharacter->GetCharacterMovement()->BrakingFrictionFactor = DefaultBrakingFrictionFactor;
			OverlappingCharacter->GetCharacterMovement()->GroundFriction = DefaultGroundFriction;
			OverlappingCharacter->GetCharacterMovement()->BrakingDecelerationWalking = DefaultBrakingDecelerationWalking;
		}
	}
}
