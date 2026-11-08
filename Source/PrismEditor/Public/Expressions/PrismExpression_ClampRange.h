#pragma once
#include "CoreMinimal.h"
#include "Expressions/TG_Expression.h"
#include "TG_Texture.h"
#include "PrismExpression_ClampRange.generated.h"
/** Per-channel RGB clamp using minimum and maximum colors; input alpha is preserved. */
UCLASS(meta=(DisplayName="Prism Color Clamp Range"))
class PRISMEDITOR_API UPrismExpression_ClampRange : public UTG_Expression
{
    GENERATED_BODY()
public:
    TG_DECLARE_EXPRESSION(TG_Category::Adjustment);
    UPROPERTY(meta=(TGType="TG_Input", PinDisplayName="")) FTG_Texture Input;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting")) FLinearColor Minimum = FLinearColor::Black;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting")) FLinearColor Maximum = FLinearColor::White;
    UPROPERTY(EditAnywhere, Category=NoCategory, meta=(TGType="TG_Output", PinDisplayName="")) FTG_Texture Output;
    virtual FTG_Name GetDefaultName() const override { return TEXT("Prism Color Clamp Range"); }
    virtual FText GetTooltipText() const override { return FText::FromString(TEXT("Per-channel RGB clamp using minimum and maximum colors; input alpha is preserved.")); }
    virtual void Evaluate(FTG_EvaluationContext* InContext) override;
};
