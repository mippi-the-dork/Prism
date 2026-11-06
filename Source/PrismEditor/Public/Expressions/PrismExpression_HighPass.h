#pragma once
#include "CoreMinimal.h"
#include "Expressions/TG_Expression.h"
#include "TG_Texture.h"
#include "PrismExpression_HighPass.generated.h"
/** High-frequency detail around neutral gray. */
UCLASS(meta=(DisplayName="Prism High Pass"))
class PRISMEDITOR_API UPrismExpression_HighPass : public UTG_Expression
{
    GENERATED_BODY()
public:
    TG_DECLARE_EXPRESSION(TG_Category::Filter);
    UPROPERTY(meta=(TGType="TG_Input", PinDisplayName="")) FTG_Texture Input;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="0", ClampMax="10")) float Strength = 1.0f;
    UPROPERTY(EditAnywhere, Category=NoCategory, meta=(TGType="TG_Output", PinDisplayName="")) FTG_Texture Output;

    virtual FTG_Name GetDefaultName() const override { return TEXT("Prism High Pass"); }
    virtual FText GetTooltipText() const override { return FText::FromString(TEXT("High-frequency detail around neutral gray.")); }
    virtual void Evaluate(FTG_EvaluationContext* InContext) override;
};
