// @RC


#include "KeyCardDoor.h"


// Sets default values
AKeyCardDoor::AKeyCardDoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AKeyCardDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKeyCardDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AKeyCardDoor::Interact(class AUntitledCharacter* MainPlayer)
{
	UE_LOG(LogTemp, Warning, TEXT("Key Card Door Opened"));

}

