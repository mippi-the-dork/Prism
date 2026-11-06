#pragma once
#include "CoreMinimal.h"
#include "Expressions/TG_Expression.h"
#include "TG_Texture.h"
#include "PrismExpression_Posterize.generated.h"
/** Quantizes each RGB channel to N levels. */
UCLASS(meta=(DisplayName="Prism Posterize"))
class PRISMEDITOR_API UPrismExpression_Posterize : public UTG_Expression
{
    GENERATED_BODY()
public:
    TG_DECLARE_EXPRESSION(TG_Category::Filter);
    UPROPERTY(meta=(TGType="TG_Input", PinDisplayName="")) FTG_Texture Input;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="2", ClampMax="256")) int32 Levels = 6;
    UPROPERTY(EditAnywhere, Category=NoCategory, meta=(TGType="TG_Output", PinDisplayName="")) FTG_Texture Output;

    virtual FTG_Name GetDefaultName() const override { return TEXT("Prism Posterize"); }
    virtual FText GetTooltipText() const override { return FText::FromString(TEXT("Quantizes each RGB channel to N levels.")); }
    virtual void Evaluate(FTG_EvaluationContext* InContext) override;
};
