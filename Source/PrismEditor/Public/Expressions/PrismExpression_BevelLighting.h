#pragma once
#include "CoreMinimal.h"
#include "Expressions/TG_Expression.h"
#include "TG_Texture.h"
#include "PrismExpression_BevelLighting.generated.h"
/** Edge-weighted directional shading preview. Does not alter or generate beveled height geometry. */
UCLASS(meta=(DisplayName="Prism Bevel Lighting Preview"))
class PRISMEDITOR_API UPrismExpression_BevelLighting : public UTG_Expression
{
    GENERATED_BODY()
public:
    TG_DECLARE_EXPRESSION(TG_Category::Filter);
    UPROPERTY(meta=(TGType="TG_Input", PinDisplayName="")) FTG_Texture Input;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="1", ClampMax="16")) int32 Radius = 1;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="0", ClampMax="10")) float Strength = 1.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="-360", ClampMax="360")) float AngleDegrees = 45.0f;
    UPROPERTY(EditAnywhere, Category=NoCategory, meta=(TGType="TG_Output", PinDisplayName="")) FTG_Texture Output;
    virtual FTG_Name GetDefaultName() const override { return TEXT("Prism Bevel Lighting Preview"); }
    virtual FText GetTooltipText() const override { return FText::FromString(TEXT("Edge-weighted directional shading preview. Does not alter or generate beveled height geometry.")); }
    virtual void Evaluate(FTG_EvaluationContext* InContext) override;
};
