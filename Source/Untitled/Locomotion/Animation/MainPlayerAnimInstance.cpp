// @RC


#include "MainPlayerAnimInstance.h"

#include "Untitled/UntitledCharacter.h"

void UMainPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	MainPlayerRef = Cast<AUntitledCharacter>(TryGetPawnOwner());
	
	bIsCrouching = MainPlayerRef->bIsCrouching;
	bIsJogging = MainPlayerRef->bIsJogging;
}

void UMainPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!MainPlayerRef) return;

	
}


