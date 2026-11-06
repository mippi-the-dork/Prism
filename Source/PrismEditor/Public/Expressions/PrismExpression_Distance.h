#pragma once
#include "CoreMinimal.h"
#include "Expressions/TG_Expression.h"
#include "TG_Texture.h"
#include "PrismExpression_Distance.generated.h"
/** Distance to the nearest opposite threshold class; search limited to 24 px. Output 0.5 at edges, positive inside. */
UCLASS(meta=(DisplayName="Prism Distance (Bounded)"))
class PRISMEDITOR_API UPrismExpression_Distance : public UTG_Expression
{
    GENERATED_BODY()
public:
    TG_DECLARE_EXPRESSION(TG_Category::Filter);
    UPROPERTY(meta=(TGType="TG_Input", PinDisplayName="")) FTG_Texture Input;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="1", ClampMax="24", UIMin="1", UIMax="24")) int32 Radius = 8;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="0", ClampMax="1")) float Threshold = 0.5f;
    UPROPERTY(EditAnywhere, Category=NoCategory, meta=(TGType="TG_Output", PinDisplayName="")) FTG_Texture Output;

    virtual FTG_Name GetDefaultName() const override { return TEXT("Prism Distance (Bounded)"); }
    virtual FText GetTooltipText() const override { return FText::FromString(TEXT("Distance to the nearest opposite threshold class; search limited to 24 px. Output 0.5 at edges, positive inside.")); }
    virtual void Evaluate(FTG_EvaluationContext* InContext) override;
};
