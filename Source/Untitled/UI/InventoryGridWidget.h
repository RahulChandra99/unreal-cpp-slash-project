#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryGridWidget.generated.h"

class UBorder;

UCLASS()
class UNTITLED_API UInventoryGridWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	void InitializeGridData();
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	                          FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

	UPROPERTY(meta = (BindWidget))
	UBorder* GridBorder;

private:

	UPROPERTY()
	TArray<float> VerticalLines;

	UPROPERTY()
	TArray<float> HorizontalLines;

	int32 Columns;
	int32 Rows;
	float TileSize;
};