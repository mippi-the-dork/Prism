#pragma once
#include "CoreMinimal.h"
#include "Expressions/TG_Expression.h"
#include "TG_Texture.h"
#include "PrismNodeEnums.h"
#include "Curves/CurveFloat.h"
#include "Curves/CurveLinearColor.h"
#include "PrismExpression_CurveGenerator.generated.h"

/** Generate a spatial grayscale ramp mapped through a Curve Float asset. */
UCLASS(meta=(DisplayName="Prism Curve Generator"))
class PRISMEDITOR_API UPrismExpression_CurveGenerator : public UTG_Expression
{
    GENERATED_BODY()
public:
    TG_DECLARE_EXPRESSION(TG_Category::Input);
    /** Optional curve asset; without one, produces an identity black-to-white ramp. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting")) TObjectPtr<UCurveFloat> Curve;
    /** Choose the spatial gradient direction from the dropdown. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting")) EPrismGradientDirection Direction = EPrismGradientDirection::Horizontal;
    UPROPERTY(EditAnywhere, Category=NoCategory, meta=(TGType="TG_Output", PinDisplayName="")) FTG_Texture Output;
    virtual FTG_Name GetDefaultName() const override { return TEXT("Prism Curve Generator"); }
    virtual FText GetTooltipText() const override { return FText::FromString(TEXT("Generate a spatial grayscale ramp mapped through a Curve Float asset.")); }
    virtual void Evaluate(FTG_EvaluationContext* InContext) override;
};
