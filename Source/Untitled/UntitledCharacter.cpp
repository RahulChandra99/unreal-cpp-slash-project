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
#include "MotionWarpingComponent.h"
#include "Components/PlayerActionsComponent.h"
#include "Components/PlayerInteractionComponent.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);




AUntitledCharacter::AUntitledCharacter()
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
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
	InteractionComponent = CreateDefaultSubobject<UPlayerInteractionComponent>(TEXT("InteractionComponent"));


	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

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

		// Vault Mantle Toggle
		EnhancedInputComponent->BindAction(MantleInputAction, ETriggerEvent::Started, this, &AUntitledCharacter::TryVaultMantle);

		// Aiming Toggle
		EnhancedInputComponent->BindAction(AimingInputAction, ETriggerEvent::Triggered, this, &AUntitledCharacter::ToggleAiming);

		// Interact Toggle
		EnhancedInputComponent->BindAction(InteractInputAction, ETriggerEvent::Started, InteractionComponent, &UPlayerInteractionComponent::Interact);
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

void AUntitledCharacter::ToggleCrouching()
{
	if (bIsCrouching)
	{
		FVector Start = GetActorLocation();
		FVector End = Start + FVector(0,0,115.f);

		float SphereRadius = 30.f;

		FHitResult HitResult;

		bool bHit = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		Start,
		End,
		SphereRadius,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		TArray<AActor*>({this}),
		EDrawDebugTrace::None,
		HitResult,
		true
		);

		if (bHit)
		{
			Debug::Print("Cant Crouch, Obstacle Detected");
			return;
		}
	}
	
	bIsCrouching = !bIsCrouching;
	
	CrouchingVisual(bIsCrouching);
	UpdateMovementSpeed();

}

void AUntitledCharacter::ToggleJogging()
{
	bIsJogging = !bIsJogging;

	UpdateMovementSpeed();
	JoggingVisual(bIsJogging);
}

void AUntitledCharacter::UpdateMovementSpeed()
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

void AUntitledCharacter::ToggleAiming()
{
	bIsAiming = !bIsAiming;
	
	AimingVisual(bIsAiming);
}


void AUntitledCharacter::TryVaultMantle()
{
	FHitResult HitResult;
	EVaultType VaultType = DetectVaultType(HitResult);

	if (VaultType == EVaultType::Vault)
	{
		StartVault(HitResult);
	}
	else if (VaultType == EVaultType::Mantle)
	{
		StartMantle(HitResult);
	}
}

EVaultType AUntitledCharacter::DetectVaultType(FHitResult& OutHitResult)
{
	FVector ForwardStart = GetActorLocation() + FVector(0, 0, ForwardHeightOffset);
    FVector ForwardEnd = ForwardStart + GetActorForwardVector() * ForwardDistance;
    
    FHitResult ForwardHit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);
    
    bool bForwardHit = GetWorld()->LineTraceSingleByChannel(ForwardHit, ForwardStart, ForwardEnd, ECC_Visibility, Params);
    DrawDebugLine(GetWorld(), ForwardStart, ForwardEnd, FColor::Green, false, 2.f);

	if (bForwardHit)
	{
		FVector DownTraceStart = ForwardHit.ImpactPoint + FVector(0, 0, DownTraceHeight);
		FVector DownTraceEnd = ForwardHit.ImpactPoint + FVector(0, 0, -50.f);

		FHitResult DownHit;
		bool bDownHit = GetWorld()->LineTraceSingleByChannel(DownHit, DownTraceStart, DownTraceEnd, ECC_Visibility, Params);
		DrawDebugLine(GetWorld(), DownTraceStart, DownTraceEnd, FColor::Blue, false, 2.f);

		if (bDownHit)
		{
			float ObstacleHeight = DownHit.ImpactPoint.Z - GetActorLocation().Z;

			if (ObstacleHeight > MaxMantleHeight)
			{
				// Too high to climb
				return EVaultType::None;
			}

			FVector OverlapLocation = DownHit.ImpactPoint + FVector(0, 0, 50.f);
			FCollisionShape CapsuleShape = FCollisionShape::MakeCapsule(30.f, 90.f);

			bool bOverlapBlocked = GetWorld()->OverlapBlockingTestByChannel(OverlapLocation, FQuat::Identity, ECC_Visibility, CapsuleShape, Params);
			DrawDebugCapsule(GetWorld(), OverlapLocation, 90.f, 30.f, FQuat::Identity, FColor::Red, false, 2.f);

			if (bOverlapBlocked)
			{
				return EVaultType::None;
			}

			if (ObstacleHeight < MaxVaultHeight)
			{
				Debug::Print("Vault");
				return EVaultType::Vault;
			}
			else
			{
				Debug::Print("Mantle");
				return EVaultType::Mantle;
			}
		}
	}

	return EVaultType::None;


}

void AUntitledCharacter::StartVault(const FHitResult& HitResult)
{
	//FVector WarpTarget = HitResult.ImpactPoint + FVector(0, 0, 50); // small upward offset
	//MotionWarpingComponent->AddOrUpdateWarpTarget(FName("VaultTarget"), WarpTarget);
	//PlayAnimMontage(VaultMontage);
}

void AUntitledCharacter::StartMantle(const FHitResult& HitResult)
{
	//FVector WarpTarget = HitResult.ImpactPoint + FVector(0, 0, 100); // higher offset
	//MotionWarpingComponent->AddOrUpdateWarpTarget(FName("MantleTarget"), WarpTarget);
	//PlayAnimMontage(MantleMontage);
}
