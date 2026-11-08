#pragma once
#include "CoreMinimal.h"
#include "Expressions/TG_Expression.h"
#include "TG_Texture.h"
#include "Curves/CurveFloat.h"
#include "PrismExpression_Curve.generated.h"
/** Per-pixel luminance remapping through an editable Unreal float curve. */
UCLASS(meta=(DisplayName="Prism Curve"))
class PRISMEDITOR_API UPrismExpression_Curve : public UTG_Expression
{
    GENERATED_BODY()
public:
    TG_DECLARE_EXPRESSION(TG_Category::Adjustment);
    UPROPERTY(meta=(TGType="TG_Input", PinDisplayName="Source")) FTG_Texture Input;
    /** Editable through Unreal's standard curve editing UI; x and y are normalized 0..1. */
    UPROPERTY(EditAnywhere, Category=NoCategory, meta=(TGType="TG_Setting")) FRuntimeFloatCurve Curve;
    UPROPERTY(EditAnywhere, Category=NoCategory, meta=(TGType="TG_Output", PinDisplayName="")) FTG_Texture Output;

    UPrismExpression_Curve();
    virtual FTG_Name GetDefaultName() const override { return TEXT("Prism Curve"); }
    virtual FText GetTooltipText() const override { return FText::FromString(TEXT("Remap source luminance through an embedded editable float curve (eight GPU samples).")); }
    virtual void Evaluate(FTG_EvaluationContext* InContext) override;
};
