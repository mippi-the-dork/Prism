#pragma once
#include "CoreMinimal.h"
#include "Expressions/TG_Expression.h"
#include "TG_Texture.h"
#include "PrismExpression_NormalNormalize.generated.h"
/** Normalize tangent space normal vectors. */
UCLASS(meta=(DisplayName="Prism Normal Normalize"))
class PRISMEDITOR_API UPrismExpression_NormalNormalize : public UTG_Expression
{
    GENERATED_BODY()
public:
    TG_DECLARE_EXPRESSION(TG_Category::Filter);
    UPROPERTY(meta=(TGType="TG_Input", PinDisplayName="")) FTG_Texture Input;
    UPROPERTY(EditAnywhere, Category=NoCategory, meta=(TGType="TG_Output", PinDisplayName="")) FTG_Texture Output;
    virtual FTG_Name GetDefaultName() const override { return TEXT("Prism Normal Normalize"); }
    virtual FText GetTooltipText() const override { return FText::FromString(TEXT("Normalize tangent space normal vectors.")); }
    virtual void Evaluate(FTG_EvaluationContext* InContext) override;
};
