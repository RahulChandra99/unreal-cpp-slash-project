// @RC

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainPlayerAnimInstance.generated.h"

class AUntitledCharacter;
/**
 * 
 */
UCLASS()
class UNTITLED_API UMainPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeInitializeAnimation() override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	AUntitledCharacter* MainPlayerRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	bool bIsJogging;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	bool bIsCrouching;
};
