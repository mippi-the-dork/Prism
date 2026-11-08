#pragma once
#include "CoreMinimal.h"
#include "Expressions/TG_Expression.h"
#include "TG_Texture.h"
#include "PrismExpression_Emboss.generated.h"
/** Produces directional slope shading from a grayscale height map, using Sobel gradients. */
UCLASS(meta=(DisplayName="Prism Directional Emboss"))
class PRISMEDITOR_API UPrismExpression_Emboss : public UTG_Expression
{
    GENERATED_BODY()
public:
    TG_DECLARE_EXPRESSION(TG_Category::Filter);
    UPROPERTY(meta=(TGType="TG_Input", PinDisplayName="")) FTG_Texture Input;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="1", ClampMax="16")) int32 Radius = 1;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="0", ClampMax="10")) float Strength = 1.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="-360", ClampMax="360")) float AngleDegrees = 45.0f;
    UPROPERTY(EditAnywhere, Category=NoCategory, meta=(TGType="TG_Output", PinDisplayName="")) FTG_Texture Output;
    virtual FTG_Name GetDefaultName() const override { return TEXT("Prism Directional Emboss"); }
    virtual FText GetTooltipText() const override { return FText::FromString(TEXT("Produces directional slope shading from a grayscale height map, using Sobel gradients.")); }
    virtual void Evaluate(FTG_EvaluationContext* InContext) override;
};
