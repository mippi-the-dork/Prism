#pragma once

#include "CoreMinimal.h"
#include "Expressions/TG_Expression.h"
#include "TG_Texture.h"
#include "PrismExpression_MultiPassBlur.generated.h"

/** Multi-stage Gaussian smoothing. Each pass is a real TextureGraph GPU blur job. */
UCLASS(meta=(DisplayName="Prism Multi-Pass Blur"))
class PRISMEDITOR_API UPrismExpression_MultiPassBlur : public UTG_Expression
{
    GENERATED_BODY()
public:
    TG_DECLARE_EXPRESSION(TG_Category::Filter);

    UPROPERTY(meta=(TGType="TG_Input", PinDisplayName=""))
    FTG_Texture Input;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="0", ClampMax="100", UIMin="0", UIMax="32"))
    int32 Radius = 4;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="1", ClampMax="8", UIMin="1", UIMax="4"))
    int32 Passes = 2;

    UPROPERTY(EditAnywhere, Category=NoCategory, meta=(TGType="TG_Output", PinDisplayName=""))
    FTG_Texture Output;

    virtual FTG_Name GetDefaultName() const override { return TEXT("Prism Multi-Pass Blur"); }
    virtual FText GetTooltipText() const override
    {
        return FText::FromString(TEXT("Apply multiple Gaussian blur passes using TextureGraph's tiled GPU processing. Zero radius passes through the input."));
    }
    virtual void Evaluate(FTG_EvaluationContext* InContext) override;
};
