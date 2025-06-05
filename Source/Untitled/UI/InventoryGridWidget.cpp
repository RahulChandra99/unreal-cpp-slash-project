#include "InventoryGridWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Untitled/UntitledCharacter.h"
#include "Untitled/Components/InventoryComponent.h"

void UInventoryGridWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InitializeGridData();
}

void UInventoryGridWidget::InitializeGridData()
{
	// Get owner character
	if (AUntitledCharacter* CharacterRef = Cast<AUntitledCharacter>(GetOwningPlayerPawn()))
	{
		if (UInventoryComponent* InventoryComponent = CharacterRef->InventoryComponent)
		{
			Columns = InventoryComponent->Columns;
			Rows = InventoryComponent->Rows;
			TileSize = InventoryComponent->TileSize;

			// Resize Grid Background to fit new grid size
			const FVector2D NewSize(Columns * TileSize, Rows * TileSize);
			if (UCanvasPanelSlot* BorderSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridBorder)) //Accessing the canvas panel which is the parent of the Gridborder, to set the size of the GridBorder
			{
				BorderSlot->SetSize(NewSize); // Widget size is controlled by parent slot and not directly by the4 widget
			}
		}
	}
}

int32 UInventoryGridWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	FPaintContext PaintContext(AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	constexpr FLinearColor GridColor(0.5f, 0.5f, 0.5f, 0.5f);

	// Offset to draw inside GridBorder
	const FVector2D TopLeft = GridBorder ? GridBorder->GetCachedGeometry().GetLocalPositionAtCoordinates(FVector2D(0.f, 0.f)) : FVector2D::ZeroVector;

	// Draw vertical grid lines
	for (int32 Col = 0; Col <= Columns; ++Col)
	{
		const float X = Col * TileSize;
		const FVector2D Start = FVector2D(X, 0.f) + TopLeft;
		const FVector2D End = FVector2D(X, Rows * TileSize) + TopLeft;
		UWidgetBlueprintLibrary::DrawLine(PaintContext, Start, End, GridColor);
	}

	// Draw horizontal grid lines
	for (int32 Row = 0; Row <= Rows; ++Row)
	{
		const float Y = Row * TileSize;
		const FVector2D Start = FVector2D(0.f, Y) + TopLeft;
		const FVector2D End = FVector2D(Columns * TileSize, Y) + TopLeft;
		UWidgetBlueprintLibrary::DrawLine(PaintContext, Start, End, GridColor);
	}

	return LayerId;
}