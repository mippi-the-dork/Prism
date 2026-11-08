#pragma once
#include "CoreMinimal.h"
#include "Expressions/TG_Expression.h"
#include "TG_Texture.h"
#include "PrismNodeEnums.h"
#include "PrismExpression_CellData.generated.h"

/** Stable per-grid-cell grayscale, color, ID or position data. Not connected-component flood fill. */
UCLASS(meta=(DisplayName="Prism Cell Data Generator"))
class PRISMEDITOR_API UPrismExpression_CellData : public UTG_Expression
{
    GENERATED_BODY()
public:
    TG_DECLARE_EXPRESSION(TG_Category::Input);
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting")) EPrismCellDataMode Mode = EPrismCellDataMode::RandomGrayscale;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="1", ClampMax="128")) int32 Columns = 8;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="1", ClampMax="128")) int32 Rows = 8;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting")) int32 Seed = 0;
    UPROPERTY(EditAnywhere, Category=NoCategory, meta=(TGType="TG_Output", PinDisplayName="")) FTG_Texture Output;
    virtual FTG_Name GetDefaultName() const override { return TEXT("Prism Cell Data Generator"); }
    virtual FText GetTooltipText() const override { return FText::FromString(TEXT("Stable per-grid-cell grayscale, color, ID or position data. Not connected-component flood fill.")); }
    virtual void Evaluate(FTG_EvaluationContext* InContext) override;
};
