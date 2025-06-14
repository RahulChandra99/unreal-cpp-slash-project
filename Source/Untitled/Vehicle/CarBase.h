// @RC

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "WheeledVehiclePawn.h"
#include "CarBase.generated.h"

class UInputAction;
class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;
/**
 * 
 */
UCLASS()
class UNTITLED_API ACarBase : public AWheeledVehiclePawn
{
	GENERATED_BODY()

public:

	ACarBase();

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "Steering")
	UCurveFloat* SteeringCurve;


protected:

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void MoveForwardBackward(float Value);

	void MoveRL(float Value);
	void BrakePressed();
	void BrakeReleased();
	void LookUpDown(float Value);
	void LookRL(float Value);




private:
	
	
	
};
