#pragma once
#include "CoreMinimal.h"
#include "Expressions/TG_Expression.h"
#include "TG_Texture.h"
#include "PrismExpression_DirectionalWarp.generated.h"
/** Warp source pixels along an angle using a displacement texture. */
UCLASS(meta=(DisplayName="Prism Directional Warp"))
class PRISMEDITOR_API UPrismExpression_DirectionalWarp : public UTG_Expression
{
    GENERATED_BODY()
public:
    TG_DECLARE_EXPRESSION(TG_Category::Filter);
    UPROPERTY(meta=(TGType="TG_Input", PinDisplayName="")) FTG_Texture Input;
    UPROPERTY(meta=(TGType="TG_Input", PinDisplayName="")) FTG_Texture InputB;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="-512", ClampMax="512")) float Distance = 16.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="-360", ClampMax="360")) float Angle = 0.0f;
    UPROPERTY(EditAnywhere, Category=NoCategory, meta=(TGType="TG_Output", PinDisplayName="")) FTG_Texture Output;
    virtual FTG_Name GetDefaultName() const override { return TEXT("Prism Directional Warp"); }
    virtual FText GetTooltipText() const override { return FText::FromString(TEXT("Warp source pixels along an angle using a displacement texture.")); }
    virtual void Evaluate(FTG_EvaluationContext* InContext) override;
};
