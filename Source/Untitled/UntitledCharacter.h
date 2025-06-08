// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/PlayerActionsComponent.h"
#include "GameFramework/Character.h"
#include "Locomotion/Misc/GameMovementType.h"
#include "Logging/LogMacros.h"
#include "UntitledCharacter.generated.h"

class UPlayerInteractionComponent;
class UPlayerActionsComponent;
class UInventoryComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;


DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AUntitledCharacter : public ACharacter
{
private:
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RC|Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RC|Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RC|Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RC|Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RC|Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RC|Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Jog Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RC|Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* JogAction;

	/** Crouch Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RC|Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	/** Switch Game Movement Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RC|Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* SwitchGameMovementAction;
	
	/** Inventory Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RC|Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* InventoryToggleAction;

	/** VaultMantle Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RC|Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* MantleInputAction;
	
	/** Aiming Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RC|Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* AimingInputAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RC|Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractInputAction;

protected:
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay();
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RC|Components")
	class UMotionWarpingComponent* MotionWarpingComponent; 

	void ToggleInventory();

	UPROPERTY()
	APlayerController* PlayerControllerRef;
	
	UPROPERTY(EditAnywhere, Category = "RC|UI")
	TSubclassOf<UUserWidget> InventoryWidgetClass;

	UPROPERTY()
	UUserWidget* InventoryWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RC|Config|Locomotion")
	EGameMovementType CurrentGameType;

	void SetupPlayerDefaults();
	
	void SwitchGameType();
	
	UFUNCTION(BlueprintImplementableEvent)
	void JoggingVisual(bool IsJogging);
	void UpdateMovementSpeed();
	void ToggleJogging();

	UFUNCTION(BlueprintImplementableEvent)
	void CrouchingVisual(bool IsCrouching);
	void ToggleCrouching();

	UFUNCTION(BlueprintImplementableEvent)
	void AimingVisual(bool IsAiming);
	void ToggleAiming();
	
	void TryVaultMantle();
	EVaultType DetectVaultType(FHitResult& OutHitResult);
	void StartVault(const FHitResult& HitResult);
	void StartMantle(const FHitResult& HitResult);

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="RC|Config|Locomotion")
	float WalkSpeed;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="RC|Config|Locomotion")
	float JogSpeed;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="RC|Config|Locomotion")
	float CrouchWalkSpeed;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="RC|Config|Locomotion")
	float CrouchRunSpeed;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category="RC|Config|Locomotion")
	float StandingCapsuleHalfHeight = 88.0f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category="RC|Config|Locomotion")
	float CrouchedCapsuleHalfHeight = 44.0f;
	
	
public:
	AUntitledCharacter();
	
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE UMotionWarpingComponent* GetMotionWarpingComponent() const { return MotionWarpingComponent; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RC|Components")
	UInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RC|Components")
	UPlayerInteractionComponent* InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="RC|Config|ReadOnly")
	bool bIsCrouching;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="RC|Config|ReadOnly")
	bool bIsJogging;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="RC|Config|ReadOnly")
	bool bIsAiming;

	UPROPERTY(EditDefaultsOnly, Category = "Vaulting")
	float VaultTraceDistance = 150.f;

	UPROPERTY(EditDefaultsOnly, Category = "Vaulting")
	float VaultTraceRadius = 30.f;

	float ForwardDistance = 100.f;
	float ForwardHeightOffset = 50.f;

	float DownTraceHeight = 150.f;
	float OverlapCheckHeight = 200.f;

	float MaxVaultHeight = 100.f;
	float MaxMantleHeight = 180.f;


	
};

