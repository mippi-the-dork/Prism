#pragma once
#include "CoreMinimal.h"
#include "Expressions/TG_Expression.h"
#include "TG_Texture.h"
#include "PrismExpression_Curve.generated.h"
/** Four fixed-position control values with smoothstep interpolation. */
UCLASS(meta=(DisplayName="Prism Curve (Four-Knot)"))
class PRISMEDITOR_API UPrismExpression_Curve : public UTG_Expression
{
    GENERATED_BODY()
public:
    TG_DECLARE_EXPRESSION(TG_Category::Filter);
    UPROPERTY(meta=(TGType="TG_Input", PinDisplayName="")) FTG_Texture Input;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="0", ClampMax="1")) float ValueA = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="0", ClampMax="1")) float ValueB = 0.333333f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="0", ClampMax="1")) float ValueC = 0.666667f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="0", ClampMax="1")) float ValueD = 1.0f;
    UPROPERTY(EditAnywhere, Category=NoCategory, meta=(TGType="TG_Output", PinDisplayName="")) FTG_Texture Output;

    virtual FTG_Name GetDefaultName() const override { return TEXT("Prism Curve (Four-Knot)"); }
    virtual FText GetTooltipText() const override { return FText::FromString(TEXT("Four fixed-position control values with smoothstep interpolation.")); }
    virtual void Evaluate(FTG_EvaluationContext* InContext) override;
};
