#pragma once
#include "CoreMinimal.h"
#include "Expressions/TG_Expression.h"
#include "TG_Texture.h"
#include "PrismExpression_FloodFill.generated.h"
/** Local neighborhood variation. NOT full connected-component Flood Fill; results can vary across large islands. */
UCLASS(meta=(DisplayName="Prism Region Variation (Local)"))
class PRISMEDITOR_API UPrismExpression_FloodFill : public UTG_Expression
{
    GENERATED_BODY()
public:
    TG_DECLARE_EXPRESSION(TG_Category::Filter);
    UPROPERTY(meta=(TGType="TG_Input", PinDisplayName="")) FTG_Texture Input;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="1", ClampMax="24", UIMin="1", UIMax="24")) int32 Radius = 8;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="0", ClampMax="1")) float Threshold = 0.5f;
    UPROPERTY(EditAnywhere, Category=NoCategory, meta=(TGType="TG_Output", PinDisplayName="")) FTG_Texture Output;

    virtual FTG_Name GetDefaultName() const override { return TEXT("Prism Region Variation (Local)"); }
    virtual FText GetTooltipText() const override { return FText::FromString(TEXT("Local neighborhood variation. NOT full connected-component Flood Fill; results can vary across large islands.")); }
    virtual void Evaluate(FTG_EvaluationContext* InContext) override;
};
