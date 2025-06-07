// @RC


#include "RCAIController.h"


// Sets default values
ARCAIController::ARCAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARCAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARCAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

