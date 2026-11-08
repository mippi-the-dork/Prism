#pragma once
#include "CoreMinimal.h"
#include "Expressions/TG_Expression.h"
#include "TG_Texture.h"
#include "PrismExpression_HeightBlend.generated.h"
/** Blend two layers using height contrast; uses grayscale of RGB as height. */
UCLASS(meta=(DisplayName="Prism Height Blend"))
class PRISMEDITOR_API UPrismExpression_HeightBlend : public UTG_Expression
{
    GENERATED_BODY()
public:
    TG_DECLARE_EXPRESSION(TG_Category::Filter);
    UPROPERTY(meta=(TGType="TG_Input", PinDisplayName="")) FTG_Texture Input;
    UPROPERTY(meta=(TGType="TG_Input", PinDisplayName="")) FTG_Texture InputB;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="0.0001", ClampMax="1")) float Transition = 0.15f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="-1", ClampMax="1")) float Bias = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="0", ClampMax="1")) float Opacity = 1.0f;
    UPROPERTY(EditAnywhere, Category=NoCategory, meta=(TGType="TG_Output", PinDisplayName="")) FTG_Texture Output;
    virtual FTG_Name GetDefaultName() const override { return TEXT("Prism Height Blend"); }
    virtual FText GetTooltipText() const override { return FText::FromString(TEXT("Blend two layers using height contrast; uses grayscale of RGB as height.")); }
    virtual void Evaluate(FTG_EvaluationContext* InContext) override;
};
