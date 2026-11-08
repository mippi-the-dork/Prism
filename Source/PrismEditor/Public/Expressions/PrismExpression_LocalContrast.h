#pragma once
#include "CoreMinimal.h"
#include "Expressions/TG_Expression.h"
#include "TG_Texture.h"
#include "PrismExpression_LocalContrast.generated.h"
/** Boosts local RGB contrast relative to a 3x3 neighborhood; approximate spatial detail enhancement. */
UCLASS(meta=(DisplayName="Prism Local Contrast"))
class PRISMEDITOR_API UPrismExpression_LocalContrast : public UTG_Expression
{
    GENERATED_BODY()
public:
    TG_DECLARE_EXPRESSION(TG_Category::Filter);
    UPROPERTY(meta=(TGType="TG_Input", PinDisplayName="")) FTG_Texture Input;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="1", ClampMax="16")) int32 Radius = 1;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="0", ClampMax="10")) float Strength = 1.0f;
    UPROPERTY(EditAnywhere, Category=NoCategory, meta=(TGType="TG_Output", PinDisplayName="")) FTG_Texture Output;
    virtual FTG_Name GetDefaultName() const override { return TEXT("Prism Local Contrast"); }
    virtual FText GetTooltipText() const override { return FText::FromString(TEXT("Boosts local RGB contrast relative to a 3x3 neighborhood; approximate spatial detail enhancement.")); }
    virtual void Evaluate(FTG_EvaluationContext* InContext) override;
};
