#pragma once
#include "CoreMinimal.h"
#include "Expressions/TG_Expression.h"
#include "TG_Texture.h"
#include "PrismExpression_DynamicGradient.generated.h"
/** Map grayscale input through connected horizontal gradient texture. */
UCLASS(meta=(DisplayName="Prism Dynamic Gradient"))
class PRISMEDITOR_API UPrismExpression_DynamicGradient : public UTG_Expression
{
    GENERATED_BODY()
public:
    TG_DECLARE_EXPRESSION(TG_Category::Adjustment);
    UPROPERTY(meta=(TGType="TG_Input", PinDisplayName="")) FTG_Texture Input;
    UPROPERTY(meta=(TGType="TG_Input", PinDisplayName="")) FTG_Texture InputB;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="0", ClampMax="1")) float GradientRow = 0.5f;
    UPROPERTY(EditAnywhere, Category=NoCategory, meta=(TGType="TG_Output", PinDisplayName="")) FTG_Texture Output;
    virtual FTG_Name GetDefaultName() const override { return TEXT("Prism Dynamic Gradient"); }
    virtual FText GetTooltipText() const override { return FText::FromString(TEXT("Map grayscale input through connected horizontal gradient texture.")); }
    virtual void Evaluate(FTG_EvaluationContext* InContext) override;
};
