// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Locomotion/Misc/GameMovementType.h"
#include "Logging/LogMacros.h"
#include "UntitledCharacter.generated.h"

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

protected:
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay();
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

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

	
	void ToggleJogging();
	void ToggleCrouching();

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="RC|Config|Locomotion")
	float WalkSpeed;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="RC|Config|Locomotion")
	float JogSpeed;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="RC|Config|Locomotion")
	float CrouchSpeed;
	
	
public:
	AUntitledCharacter();
	
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RC|UI")
	UInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="RC|Config|ReadOnly")
	bool bIsCrouching;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="RC|Config|ReadOnly")
	bool bIsJogging;

	
};

