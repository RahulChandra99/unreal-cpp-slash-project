// @RC


#include "MainPlayerAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Untitled/UntitledCharacter.h"

void UMainPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	MainPlayerRef = Cast<AUntitledCharacter>(TryGetPawnOwner());
	
	
}

void UMainPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (MainPlayerRef == nullptr)
	{
		MainPlayerRef = Cast<AUntitledCharacter>(TryGetPawnOwner());
	};

	if (MainPlayerRef == nullptr) return;

	FVector Velocity = MainPlayerRef->GetVelocity();
	Velocity.Z = 0;
	Speed = Velocity.Size();

	bIsAccelerating = MainPlayerRef->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0 ? true : false;
	
	bIsCrouching = MainPlayerRef->bIsCrouching;
	bIsJogging = MainPlayerRef->bIsJogging;
	
	
}


