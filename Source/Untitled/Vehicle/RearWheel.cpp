// @RC

#include "RearWheel.h"

URearWheel::URearWheel()
{
	AxleType = EAxleType::Rear;

	// Steering
	bAffectedBySteering = false;
	MaxSteerAngle       = 0.f;
	bAffectedByBrake = true;
	bAffectedByHandbrake = true;
	bAffectedByEngine = true;
	bABSEnabled = false;

	// Suspension
	SuspensionMaxRaise  = 12.f;
	SuspensionMaxDrop   = 18.f;
	SpringRate          = 280.f;
	SpringPreload       = 40.f;
	SuspensionDampingRatio = 0.55f;

	// Brakes
	MaxBrakeTorque      = 1500.f;
	MaxHandBrakeTorque  = 3500.f;
}
