#pragma once
#include "CoreMinimal.h"
#include "Expressions/TG_Expression.h"
#include "TG_Texture.h"
#include "PrismNodeEnums.h"
#include "Curves/CurveFloat.h"
#include "Curves/CurveLinearColor.h"
#include "PrismExpression_GradientGenerator.generated.h"

/** Generate a spatial color gradient from a Curve Linear Color asset or a default black-to-white ramp. */
UCLASS(meta=(DisplayName="Prism Gradient Generator"))
class PRISMEDITOR_API UPrismExpression_GradientGenerator : public UTG_Expression
{
    GENERATED_BODY()
public:
    TG_DECLARE_EXPRESSION(TG_Category::Input);
    /** Optional curve asset; without one, produces an identity black-to-white ramp. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting")) TObjectPtr<UCurveLinearColor> Curve;
    /** Choose the spatial gradient direction from the dropdown. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting")) EPrismGradientDirection Direction = EPrismGradientDirection::Horizontal;
    UPROPERTY(EditAnywhere, Category=NoCategory, meta=(TGType="TG_Output", PinDisplayName="")) FTG_Texture Output;
    virtual FTG_Name GetDefaultName() const override { return TEXT("Prism Gradient Generator"); }
    virtual FText GetTooltipText() const override { return FText::FromString(TEXT("Generate a spatial color gradient from a Curve Linear Color asset or a default black-to-white ramp.")); }
    virtual void Evaluate(FTG_EvaluationContext* InContext) override;
};
