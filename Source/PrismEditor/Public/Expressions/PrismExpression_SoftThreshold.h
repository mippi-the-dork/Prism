#pragma once
#include "CoreMinimal.h"
#include "Expressions/TG_Expression.h"
#include "TG_Texture.h"
#include "PrismExpression_SoftThreshold.generated.h"
/** Soft grayscale threshold; Width controls the transition band. */
UCLASS(meta=(DisplayName="Prism Soft Threshold"))
class PRISMEDITOR_API UPrismExpression_SoftThreshold : public UTG_Expression
{
    GENERATED_BODY()
public:
    TG_DECLARE_EXPRESSION(TG_Category::Adjustment);
    UPROPERTY(meta=(TGType="TG_Input", PinDisplayName="")) FTG_Texture Input;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="0", ClampMax="1")) float Threshold = 0.5f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="0.0001", ClampMax="1")) float Width = 0.1f;
    UPROPERTY(EditAnywhere, Category=NoCategory, meta=(TGType="TG_Output", PinDisplayName="")) FTG_Texture Output;
    virtual FTG_Name GetDefaultName() const override { return TEXT("Prism Soft Threshold"); }
    virtual FText GetTooltipText() const override { return FText::FromString(TEXT("Soft grayscale threshold; Width controls the transition band.")); }
    virtual void Evaluate(FTG_EvaluationContext* InContext) override;
};
