// @RC


#include "NormalDoor.h"

#include "Untitled/Debug/CustomDebug.h"


// Sets default values
ANormalDoor::ANormalDoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bIsDoorOpening = false;
}

// Called when the game starts or when spawned
void ANormalDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANormalDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANormalDoor::Interact(class AUntitledCharacter* MainPlayer)
{

	// Notify Blueprint
	OnDoorInteract(bIsDoorOpening);
}


