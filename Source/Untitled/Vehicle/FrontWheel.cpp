// @RC


#include "FrontWheel.h"

UFrontWheel::UFrontWheel()
{
	AxleType = EAxleType::Front;
	
	// Steering
	bAffectedBySteering = true;
	MaxSteerAngle       = 45.f;  
	bAffectedByBrake = true;
    bAffectedByHandbrake = false;
    bAffectedByEngine = false; // or true if AWD/FWD
	bABSEnabled = true;

	// Suspension
	SuspensionMaxRaise  = 10.f;  // centimetres
	SuspensionMaxDrop   = 15.f;  // centimetres
	SpringRate          = 300.f; // N/cm
	SpringPreload       = 50.f;  // N
	SuspensionDampingRatio = 0.65f;

	// Brakes
	MaxBrakeTorque      = 2500.f;
	MaxHandBrakeTorque  = 0.f;   // no hand-brake torque on front axle
}
