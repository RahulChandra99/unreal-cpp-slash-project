// @RC

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerActionsComponent.generated.h"


class AUntitledCharacter;

UENUM(BlueprintType)
enum class EVaultType : uint8
{
	None,
	Vault,
	Mantle
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UNTITLED_API UPlayerActionsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UPlayerActionsComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
								   FActorComponentTickFunction* ThisTickFunction) override;


protected:
	virtual void BeginPlay() override;
	
	

	
	
	
	
};
