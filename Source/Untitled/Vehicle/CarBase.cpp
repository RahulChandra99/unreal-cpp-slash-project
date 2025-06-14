// @RC

#include "CarBase.h"

#include "Camera/CameraComponent.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputCoreTypes.h"

ACarBase::ACarBase()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 600.f;
	SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 140.f));
	SpringArm->SetRelativeRotation(FRotator(-10.f, 0.f, 0.f));
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	if (auto* Movement = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent()))
	{
		Movement->EngineSetup.MaxRPM            = 7000.f;
		Movement->EngineSetup.MaxTorque         = 500.f;
		Movement->EngineSetup.EngineIdleRPM     = 1200.f;
		Movement->EngineSetup.EngineBrakeEffect = 0.1f;
		Movement->EngineSetup.EngineRevDownRate = 700.f;
		Movement->EngineSetup.EngineRevUpMOI    = 4.f;

		Movement->TransmissionSetup.bUseAutomaticGears = true;
		Movement->DifferentialSetup.DifferentialType = EVehicleDifferential::RearWheelDrive;
		Movement->TransmissionSetup.bUseAutomaticGears = true;
		Movement->SetUseAutomaticGears(true);


		Movement->SetHandbrakeInput(false);
	}
}

void ACarBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (auto* Movement = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent()))
	{
		const float SpeedKmh = Movement->GetForwardSpeed() * 0.036f;
		GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Green,
			FString::Printf(TEXT("Throttle: %.2f  Speed: %.2f km/h  RPM: %.0f  Gear: %d"),
				Movement->GetThrottleInput(),
				SpeedKmh,
				Movement->GetEngineRotationSpeed(),
				Movement->GetCurrentGear()));
	}
}

void ACarBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ACarBase::MoveForwardBackward);
	PlayerInputComponent->BindAxis(TEXT("MoveRL"), this, &ACarBase::MoveRL);
	PlayerInputComponent->BindAxis(TEXT("LookUpDown"), this, &ACarBase::LookUpDown);
	PlayerInputComponent->BindAxis(TEXT("LookRL"), this, &ACarBase::LookRL);

	PlayerInputComponent->BindAction(TEXT("Brakes"), IE_Pressed, this, &ACarBase::BrakePressed);
	PlayerInputComponent->BindAction(TEXT("Brakes"), IE_Released, this, &ACarBase::BrakeReleased);
}

void ACarBase::MoveForwardBackward(float Value)
{
	if (auto* Movement = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent()))
	{
		// Make sure Chaos auto-reverse is enabled
		Movement->bReverseAsBrake = true;

		// Forward
		if (Value >= 0.f)
		{
			Movement->SetThrottleInput(Value);
			Movement->SetBrakeInput(0.f);
		}
		// Reverse (via braking)
		else
		{
			Movement->SetThrottleInput(0.f);
			Movement->SetBrakeInput(FMath::Abs(Value));
		}
	}
}




void ACarBase::MoveRL(float Value)
{
	if (auto* Movement = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent()))
	{
		float Steering = FMath::Clamp(Value, -1.f, 1.f);

		if (SteeringCurve)
		{
			float Speed = FMath::Abs(Movement->GetForwardSpeed());
			float CurveVal = SteeringCurve->GetFloatValue(Speed);
			Steering *= CurveVal;
		}

		Movement->SetSteeringInput(Steering);
	}
}

void ACarBase::BrakePressed()
{
	if (auto* Movement = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent()))
	{
		Movement->SetHandbrakeInput(true);
	}
}

void ACarBase::BrakeReleased()
{
	if (auto* Movement = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent()))
	{
		Movement->SetHandbrakeInput(false);
	}
}

void ACarBase::LookUpDown(float Value)
{
	AddControllerPitchInput(Value);
}

void ACarBase::LookRL(float Value)
{
	AddControllerYawInput(Value);
}
