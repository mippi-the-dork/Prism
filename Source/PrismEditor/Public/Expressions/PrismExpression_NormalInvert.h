#pragma once
#include "CoreMinimal.h"
#include "Expressions/TG_Expression.h"
#include "TG_Texture.h"
#include "PrismExpression_NormalInvert.generated.h"
/** Flip X/Y normal channels and renormalize. */
UCLASS(meta=(DisplayName="Prism Normal Invert"))
class PRISMEDITOR_API UPrismExpression_NormalInvert : public UTG_Expression
{
    GENERATED_BODY()
public:
    TG_DECLARE_EXPRESSION(TG_Category::Filter);
    UPROPERTY(meta=(TGType="TG_Input", PinDisplayName="")) FTG_Texture Input;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting")) bool bInvertX = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting")) bool bInvertY = true;
    UPROPERTY(EditAnywhere, Category=NoCategory, meta=(TGType="TG_Output", PinDisplayName="")) FTG_Texture Output;
    virtual FTG_Name GetDefaultName() const override { return TEXT("Prism Normal Invert"); }
    virtual FText GetTooltipText() const override { return FText::FromString(TEXT("Flip X/Y normal channels and renormalize.")); }
    virtual void Evaluate(FTG_EvaluationContext* InContext) override;
};
