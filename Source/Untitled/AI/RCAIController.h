// @RC

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RCAIController.generated.h"

UCLASS()
class UNTITLED_API ARCAIController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARCAIController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
