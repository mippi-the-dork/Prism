#pragma once
#include "CoreMinimal.h"
#include "Expressions/TG_Expression.h"
#include "TG_Texture.h"
#include "PrismExpression_GammaCorrection.generated.h"
/** Gamma correction for RGB, preserving original alpha; 1.0 is identity. */
UCLASS(meta=(DisplayName="Prism Gamma Adjustment"))
class PRISMEDITOR_API UPrismExpression_GammaCorrection : public UTG_Expression
{
    GENERATED_BODY()
public:
    TG_DECLARE_EXPRESSION(TG_Category::Adjustment);
    UPROPERTY(meta=(TGType="TG_Input", PinDisplayName="")) FTG_Texture Input;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="0.01", ClampMax="10")) float Gamma = 1.0f;
    UPROPERTY(EditAnywhere, Category=NoCategory, meta=(TGType="TG_Output", PinDisplayName="")) FTG_Texture Output;
    virtual FTG_Name GetDefaultName() const override { return TEXT("Prism Gamma Adjustment"); }
    virtual FText GetTooltipText() const override { return FText::FromString(TEXT("Gamma correction for RGB, preserving original alpha; 1.0 is identity.")); }
    virtual void Evaluate(FTG_EvaluationContext* InContext) override;
};
