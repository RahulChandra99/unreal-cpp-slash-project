// @RC


#include "DoorBase.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Untitled/UntitledCharacter.h"


// Sets default values
ADoorBase::ADoorBase()
{
	PrimaryActorTick.bCanEverTick = true;

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	RootComponent = DoorMesh;

	LargeInteractionBox = CreateDefaultSubobject<USphereComponent>(TEXT("LargeInteractionBox"));
	LargeInteractionBox->SetupAttachment(RootComponent);
	LargeInteractionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	LargeInteractionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	LargeInteractionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	LargeInteractionBoxWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("BoxInteractionWidget"));
	LargeInteractionBoxWidget->SetupAttachment(RootComponent);
	LargeInteractionBoxWidget->SetWidgetSpace(EWidgetSpace::Screen);
	LargeInteractionBoxWidget->SetVisibility(false);

	CloseInteractionSphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("CloseInteractionSphereCollision"));
	CloseInteractionSphereCollision->SetupAttachment(RootComponent);
	CloseInteractionSphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CloseInteractionSphereCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	CloseInteractionSphereCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	InteractButtonWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractButtonWidget"));
	InteractButtonWidget->SetupAttachment(RootComponent);
	InteractButtonWidget->SetWidgetSpace(EWidgetSpace::Screen);
	InteractButtonWidget->SetVisibility(false);
}

void ADoorBase::BeginPlay()
{
	Super::BeginPlay();

	LargeInteractionBox->OnComponentBeginOverlap.AddDynamic(this,&ADoorBase::OnLargeInteractionBoxOverlap);
	LargeInteractionBox->OnComponentEndOverlap.AddDynamic(this,&ADoorBase::OnLargeInteractionBoxEndOverlap);

	CloseInteractionSphereCollision->OnComponentBeginOverlap.AddDynamic(this,&ADoorBase::OnCloseInteractionSphereOverlap);
	CloseInteractionSphereCollision->OnComponentEndOverlap.AddDynamic(this,&ADoorBase::OnCloseInteractionSphereEndOverlap);
	
}

void ADoorBase::OnLargeInteractionBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AUntitledCharacter* Player = Cast<AUntitledCharacter>(OtherActor))
	{
		LargeInteractionBoxWidget->SetVisibility(true);
	}
}

void ADoorBase::OnLargeInteractionBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AUntitledCharacter* Player = Cast<AUntitledCharacter>(OtherActor))
	{
		LargeInteractionBoxWidget->SetVisibility(false);
	}
	
}

void ADoorBase::OnCloseInteractionSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AUntitledCharacter* Player = Cast<AUntitledCharacter>(OtherActor))
	{
		InteractButtonWidget->SetVisibility(true);
	}
}

void ADoorBase::OnCloseInteractionSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AUntitledCharacter* Player = Cast<AUntitledCharacter>(OtherActor))
	{
		InteractButtonWidget->SetVisibility(false);
	}
}

void ADoorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADoorBase::Interact(class AUntitledCharacter* MainPlayer)
{
	UE_LOG(LogTemp, Warning, TEXT("Base Door Interact"));
}

