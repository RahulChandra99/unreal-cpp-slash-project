// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryGridWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Untitled/UntitledCharacter.h"
#include "Untitled/Components/InventoryComponent.h"

void UInventoryGridWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AUntitledCharacter* CharacterRef = Cast<AUntitledCharacter>(GetOwningPlayerPawn());

	if (!CharacterRef) return;

	UInventoryComponent* InventoryComponent = CharacterRef->InventoryComponent;

	Columns = InventoryComponent->Columns;
	Rows = InventoryComponent->Rows;
	TileSize = InventoryComponent->TileSize;

	float NewWidth = Columns * TileSize;
	float NewHeight = Rows * TileSize;

	LineStructData = new FLines();
	StartX = {};
	StartY = {};
	EndX = {};
	EndY = {};

	UCanvasPanelSlot* BorderAsCanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridBorder);
	BorderAsCanvasSlot->SetSize(FVector2D(NewWidth, NewHeight));
}

void UInventoryGridWidget::CreateLineSegments()
{
	
}

int32 UInventoryGridWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	return Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
	                          bParentEnabled);
}
