// Copyright Epic Games, Inc. All Rights Reserved.

#include "UntitledCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Blueprint/UserWidget.h"
#include "Components/InventoryComponent.h"
#include "Debug/CustomDebug.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);




AUntitledCharacter::AUntitledCharacter()
	:WalkSpeed(300.f), JogSpeed(400.f)
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 200.0f; 
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 10.f;
	CameraBoom->SocketOffset = FVector(0.f,30.f,70.f);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

}

void AUntitledCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetupPlayerDefaults();

	
	
}

void AUntitledCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUntitledCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AUntitledCharacter::Look);

		// Switch Game Movement Type 
		EnhancedInputComponent->BindAction(SwitchGameMovementAction, ETriggerEvent::Started, this, &AUntitledCharacter::SwitchGameType);
		
		// Inventory Toggle
		EnhancedInputComponent->BindAction(InventoryToggleAction, ETriggerEvent::Started, this, &AUntitledCharacter::ToggleInventory);

		// Jogging Toggle
		EnhancedInputComponent->BindAction(JogAction, ETriggerEvent::Triggered, this, &AUntitledCharacter::ToggleJogging);

		// Crouch Toggle
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AUntitledCharacter::ToggleCrouching);
	}
}

void AUntitledCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AUntitledCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}


void AUntitledCharacter::SetupPlayerDefaults()
{
	//DEBUG
	bIsJogging = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	
	PlayerControllerRef = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!IsValid(PlayerControllerRef)) return;

	//Inventory UI Initialization
	if (InventoryWidgetClass)
	{
		InventoryWidget = CreateWidget(GetWorld(), InventoryWidgetClass);
		InventoryWidget->SetOwningPlayer(PlayerControllerRef);
	}
}

void AUntitledCharacter::SwitchGameType()
{
	switch (CurrentGameType)
	{
	case EGameMovementType::HITMAN:
		Debug::Print("Current Game Movement : Hitman");
		
		CameraBoom->TargetArmLength = 200.0f; 
		CameraBoom->bUsePawnControlRotation = true;
		CameraBoom->bEnableCameraLag = true;
		CameraBoom->CameraLagSpeed = 20.f;
		CameraBoom->SocketOffset = FVector(0.f,20.f,50.f);
		
		bIsJogging = false;
		
		break;
	case EGameMovementType::RPG:
		Debug::Print("Current Game Movement : RPG");
		break;
	default: ;
	}
}

void AUntitledCharacter::ToggleJogging()
{
	if (bIsJogging)
	{
		bIsJogging = false;
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
	else
	{
		bIsJogging = true;
		GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
	}
}

void AUntitledCharacter::ToggleCrouching()
{
}

void AUntitledCharacter::ToggleInventory()
{
	if (!InventoryWidget) return;
	if (InventoryWidget->IsInViewport())
	{
		InventoryWidget->RemoveFromParent();
		PlayerControllerRef->SetShowMouseCursor(false);
		PlayerControllerRef->SetInputMode(FInputModeGameOnly());
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}
	else
	{
		InventoryWidget->AddToViewport();
		PlayerControllerRef->SetShowMouseCursor(true);
		PlayerControllerRef->SetInputMode(FInputModeGameAndUI());
		GetCharacterMovement()->SetMovementMode(MOVE_None);
	}
}
