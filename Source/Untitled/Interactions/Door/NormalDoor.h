// @RC

#pragma once

#include "CoreMinimal.h"
#include "Untitled/Interactions/DoorBase.h"
#include "NormalDoor.generated.h"

UCLASS()
class UNTITLED_API ANormalDoor : public ADoorBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ANormalDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact(class AUntitledCharacter* MainPlayer) override;
};
