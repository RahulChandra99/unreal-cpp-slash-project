﻿// @RC


#include "NormalDoor.h"


// Sets default values
ANormalDoor::ANormalDoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
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
	UE_LOG(LogTemp, Warning, TEXT("Normal Door Opened"));

}

