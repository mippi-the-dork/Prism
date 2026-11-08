#pragma once
#include "CoreMinimal.h"
#include "Expressions/TG_Expression.h"
#include "TG_Texture.h"
#include "PrismExpression_TileSeamPreview.generated.h"
/** Visualize repeated texture with highlighted tile boundaries. */
UCLASS(meta=(DisplayName="Prism Tile Seam Preview"))
class PRISMEDITOR_API UPrismExpression_TileSeamPreview : public UTG_Expression
{
    GENERATED_BODY()
public:
    TG_DECLARE_EXPRESSION(TG_Category::Filter);
    UPROPERTY(meta=(TGType="TG_Input", PinDisplayName="")) FTG_Texture Input;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="1", ClampMax="16")) int32 Repeat = 3;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="0.1", ClampMax="20")) float SeamWidth = 2.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="0", ClampMax="1")) float Overlay = 0.7f;
    UPROPERTY(EditAnywhere, Category=NoCategory, meta=(TGType="TG_Output", PinDisplayName="")) FTG_Texture Output;
    virtual FTG_Name GetDefaultName() const override { return TEXT("Prism Tile Seam Preview"); }
    virtual FText GetTooltipText() const override { return FText::FromString(TEXT("Visualize repeated texture with highlighted tile boundaries.")); }
    virtual void Evaluate(FTG_EvaluationContext* InContext) override;
};
