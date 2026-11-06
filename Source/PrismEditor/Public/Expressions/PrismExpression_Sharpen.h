#pragma once
#include "CoreMinimal.h"
#include "Expressions/TG_Expression.h"
#include "TG_Texture.h"
#include "PrismExpression_Sharpen.generated.h"
/** Four-neighbor high-frequency enhancement. */
UCLASS(meta=(DisplayName="Prism Sharpen"))
class PRISMEDITOR_API UPrismExpression_Sharpen : public UTG_Expression
{
    GENERATED_BODY()
public:
    TG_DECLARE_EXPRESSION(TG_Category::Filter);
    UPROPERTY(meta=(TGType="TG_Input", PinDisplayName="")) FTG_Texture Input;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="0", ClampMax="10")) float Strength = 1.0f;
    UPROPERTY(EditAnywhere, Category=NoCategory, meta=(TGType="TG_Output", PinDisplayName="")) FTG_Texture Output;

    virtual FTG_Name GetDefaultName() const override { return TEXT("Prism Sharpen"); }
    virtual FText GetTooltipText() const override { return FText::FromString(TEXT("Four-neighbor high-frequency enhancement.")); }
    virtual void Evaluate(FTG_EvaluationContext* InContext) override;
};
