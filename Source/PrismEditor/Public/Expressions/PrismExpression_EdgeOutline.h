#pragma once
#include "CoreMinimal.h"
#include "Expressions/TG_Expression.h"
#include "TG_Texture.h"
#include "PrismExpression_EdgeOutline.generated.h"
/** Sobel edge detection from luminance; pixel-step radius controls the sampling scale. */
UCLASS(meta=(DisplayName="Prism Sobel Edge Outline"))
class PRISMEDITOR_API UPrismExpression_EdgeOutline : public UTG_Expression
{
    GENERATED_BODY()
public:
    TG_DECLARE_EXPRESSION(TG_Category::Filter);
    UPROPERTY(meta=(TGType="TG_Input", PinDisplayName="")) FTG_Texture Input;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="1", ClampMax="16")) int32 Radius = 1;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="0", ClampMax="10")) float Strength = 1.0f;
    UPROPERTY(EditAnywhere, Category=NoCategory, meta=(TGType="TG_Output", PinDisplayName="")) FTG_Texture Output;
    virtual FTG_Name GetDefaultName() const override { return TEXT("Prism Sobel Edge Outline"); }
    virtual FText GetTooltipText() const override { return FText::FromString(TEXT("Sobel edge detection from luminance; pixel-step radius controls the sampling scale.")); }
    virtual void Evaluate(FTG_EvaluationContext* InContext) override;
};
