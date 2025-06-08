// @RC

#pragma once

#include "CoreMinimal.h"
#include "Untitled/Interactions/DoorBase.h"
#include "KeyDoor.generated.h"

UCLASS()
class UNTITLED_API AKeyDoor : public ADoorBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AKeyDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact(class AUntitledCharacter* MainPlayer) override;
};
