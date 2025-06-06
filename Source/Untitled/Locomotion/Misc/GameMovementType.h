// @RC

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EGameMovementType : uint8
{
	HITMAN UMETA(DisplayName = "Hitman"),
	RPG UMETA(DisplayName = "RPG")
};