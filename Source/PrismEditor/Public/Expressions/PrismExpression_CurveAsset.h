#pragma once
#include "CoreMinimal.h"
#include "Expressions/TG_Expression.h"
#include "TG_Texture.h"
#include "Curves/CurveFloat.h"
#include "Curves/CurveLinearColor.h"
#include "PrismExpression_CurveAsset.generated.h"
/** Remap source luminance using a reusable Unreal Curve Float asset. Requires a source texture; samples curve at eight points. */
UCLASS(meta=(DisplayName="Prism Curve Asset"))
class PRISMEDITOR_API UPrismExpression_CurveAsset : public UTG_Expression
{
    GENERATED_BODY()
public:
    TG_DECLARE_EXPRESSION(TG_Category::Adjustment);
    /** Required input texture whose luminance is remapped through the referenced curve asset. */
    UPROPERTY(meta=(TGType="TG_Input", PinDisplayName="Source")) FTG_Texture Input;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting")) TObjectPtr<UCurveFloat> Curve;
    UPROPERTY(EditAnywhere, Category=NoCategory, meta=(TGType="TG_Output", PinDisplayName="")) FTG_Texture Output;
    virtual FTG_Name GetDefaultName() const override { return TEXT("Prism Curve Asset"); }
    virtual FText GetTooltipText() const override { return FText::FromString(TEXT("Remap source luminance using a reusable Unreal Curve Float asset. Requires a source texture; samples curve at eight points.")); }
    virtual void Evaluate(FTG_EvaluationContext* InContext) override;
};
