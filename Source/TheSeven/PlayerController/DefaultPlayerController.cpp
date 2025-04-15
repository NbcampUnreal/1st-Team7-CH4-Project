// 


#include "DefaultPlayerController.h"
#include "EnhancedInputSubsystems.h"

ADefaultPlayerController::ADefaultPlayerController()
{
}

void ADefaultPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetupInputMappingContext();	
}

void ADefaultPlayerController::SetupInputMappingContext()
{
	checkf(!InputMappingContext, TEXT("InputMappingContext가 설정되지 않았습니다."));
	
	if(true == IsLocalController())
	{
		if(ULocalPlayer* LocalPlayer = GetLocalPlayer())
		{
			if(UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}