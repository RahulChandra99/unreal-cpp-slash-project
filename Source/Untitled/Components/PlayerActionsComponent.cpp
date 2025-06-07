// @RC


#include "PlayerActionsComponent.h"

#include "Untitled/UntitledCharacter.h"


UPlayerActionsComponent::UPlayerActionsComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

	
}


void UPlayerActionsComponent::BeginPlay()
{
	Super::BeginPlay();
	
}




void UPlayerActionsComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}



