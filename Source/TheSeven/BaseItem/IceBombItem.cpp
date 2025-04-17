
// Copyright Epic Games, Inc. All Rights Reserved.
#include "IceBombItem.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"

AIceBombItem::AIceBombItem()
{
	PrimaryActorTick.bCanEverTick = false;

	PickupCollision = CreateDefaultSubobject<USphereComponent>(TEXT("PickupCollision"));
	PickupCollision->InitSphereRadius(50.0f);
	PickupCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	RootComponent = PickupCollision;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	MeshComponent->SetSimulatePhysics(false);

	BombCollision = CreateDefaultSubobject<USphereComponent>(TEXT("BombCollision"));
	BombCollision->InitSphereRadius(50.0f);
	BombCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	BombCollision->SetupAttachment(RootComponent);

	FreezeDuration = 3.0f;
	IceEffect = nullptr;
	IceSound = nullptr;

	bIsPickedUp = false;
	bIsThrown = false;
}

void AIceBombItem::BeginPlay()
{
	Super::BeginPlay();

	PickupCollision->OnComponentBeginOverlap.AddDynamic(this, &AIceBombItem::OnPickupOverlap);
	BombCollision->OnComponentBeginOverlap.AddDynamic(this, &AIceBombItem::OnBombOverlap);
	BombCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AIceBombItem::OnPickupOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this || bIsPickedUp)
		return;

	ACharacter* PlayerChar = Cast<ACharacter>(OtherActor);
	if (!PlayerChar)
		return;

	TArray<AActor*> AttachedActors;
	PlayerChar->GetAttachedActors(AttachedActors);
	for (AActor* Attached : AttachedActors)
	{
		if (Attached && (Attached->IsA(AIceBombItem::StaticClass()) || Attached->IsA(AIceBombItem::StaticClass())))
			return;
	}

	bIsPickedUp = true;
	SetOwner(PlayerChar);

	if (USkeletalMeshComponent* Mesh = PlayerChar->GetMesh())
	{
		FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, true);
		AttachToComponent(Mesh, Rules, TEXT("hand_l"));
	}

	PickupCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BombCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetSimulatePhysics(false);

	if (APlayerController* PC = Cast<APlayerController>(PlayerChar->GetController()))
	{
		EnableInput(PC);
		if (InputComponent)
			InputComponent->BindAction("ThrowIceBomb", IE_Pressed, this, &AIceBombItem::ThrowIceBomb);
	}
}

void AIceBombItem::ThrowIceBomb()
{
	if (!bIsPickedUp || bIsThrown)
		return;

	bIsThrown = true;
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	FVector ThrowDir = FVector::ZeroVector;
	if (ACharacter* OwnerChar = Cast<ACharacter>(GetOwner()))
	{
		ThrowDir = OwnerChar->GetActorForwardVector() + FVector(0.f, 0.f, 0.2f);
		ThrowDir.Normalize();
		BombCollision->IgnoreActorWhenMoving(OwnerChar, true);
	}
	MeshComponent->SetPhysicsLinearVelocity(ThrowDir * 1200.f);

	BombCollision->SetGenerateOverlapEvents(true);
	BombCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	FTimerHandle StopIgnoreHandle;
	GetWorld()->GetTimerManager().SetTimer(StopIgnoreHandle, [this]()
		{
			if (ACharacter* OwnerChar = Cast<ACharacter>(GetOwner()))
				BombCollision->IgnoreActorWhenMoving(OwnerChar, false);
		}, 0.5f, false);
}

void AIceBombItem::OnBombOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this || OtherActor == GetOwner() || !bIsThrown)
		return;

	ACharacter* HitChar = Cast<ACharacter>(OtherActor);
	if (!HitChar)
		return;

	FreezeCharacter(HitChar);

	Destroy();
}

void AIceBombItem::FreezeCharacter(ACharacter* Character)
{
	if (IceEffect)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), IceEffect, Character->GetActorLocation());
	if (IceSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), IceSound, Character->GetActorLocation());

	if (Character->GetCharacterMovement())
		Character->GetCharacterMovement()->DisableMovement();

	Character->CustomTimeDilation = 0.0f;

	FTimerHandle UnfreezeHandle;
	GetWorld()->GetTimerManager().SetTimer(UnfreezeHandle, [Character, this]()
		{
			if (IsValid(Character) && Character->GetCharacterMovement())
			{
				Character->CustomTimeDilation = 1.0f;
				Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
			}
		}, FreezeDuration, false);
}
