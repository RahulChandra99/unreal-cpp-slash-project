// @RC


#include "PlayerInteractionComponent.h"

#include "Engine/OverlapResult.h"
#include "Untitled/Untitled.h"
#include "Untitled/UntitledCharacter.h"
#include "Untitled/Debug/CustomDebug.h"
#include "Untitled/Interactions/Interfaces/InteractInterface.h"


// Sets default values for this component's properties
UPlayerInteractionComponent::UPlayerInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UPlayerInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPlayerInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerInteractionComponent::Interact()
{
	AActor* Owner = GetOwner();
	if (!Owner)
		return;

	FVector Start = Owner->GetActorLocation();
	FCollisionShape Sphere = FCollisionShape::MakeSphere(InteractionRadius);

	TArray<FOverlapResult> Overlaps;
	bool bHit = GetWorld()->OverlapMultiByChannel(
		Overlaps,
		Start,
		FQuat::Identity,
		COLLISION_INTERACTION,
		Sphere
	);
	
	
	for (const FOverlapResult& Result : Overlaps)
	{
		AActor* HitActor = Result.GetActor();
		if (HitActor && HitActor->Implements<UInteractInterface>())
		{
			IInteractInterface* Interactable = Cast<IInteractInterface>(HitActor);
			if (Interactable)
			{
				Interactable->Interact(Cast<AUntitledCharacter>(Owner));
				break;
			}
		}
	}
}

