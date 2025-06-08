// @RC

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractInterface.h"
#include "DoorBase.generated.h"

class UWidgetComponent;

UCLASS()
class UNTITLED_API ADoorBase : public AActor, public IInteractInterface
{
	GENERATED_BODY()

public:
	ADoorBase();
	virtual void Tick(float DeltaTime) override;

	virtual void Interact(class AUntitledCharacter* MainPlayer) override;

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* DoorMesh;

	UPROPERTY(EditAnywhere)
	class USphereComponent* LargeInteractionBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetComponent* LargeInteractionBoxWidget;

	UPROPERTY(EditAnywhere)
	class USphereComponent* CloseInteractionSphereCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetComponent* InteractButtonWidget;
	
private:

	UFUNCTION()
	void OnLargeInteractionBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnLargeInteractionBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnCloseInteractionSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCloseInteractionSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
};
