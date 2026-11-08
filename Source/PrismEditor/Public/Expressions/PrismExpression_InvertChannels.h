#pragma once
#include "CoreMinimal.h"
#include "Expressions/TG_Expression.h"
#include "TG_Texture.h"
#include "PrismExpression_InvertChannels.generated.h"
/** Independently invert R, G, B and A; built-in TextureGraph Invert remains available. */
UCLASS(meta=(DisplayName="Prism Invert Channels"))
class PRISMEDITOR_API UPrismExpression_InvertChannels : public UTG_Expression
{
    GENERATED_BODY()
public:
    TG_DECLARE_EXPRESSION(TG_Category::Adjustment);
    UPROPERTY(meta=(TGType="TG_Input", PinDisplayName="")) FTG_Texture Input;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting")) bool InvertR = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting")) bool InvertG = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting")) bool InvertB = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting")) bool InvertA = false;
    UPROPERTY(EditAnywhere, Category=NoCategory, meta=(TGType="TG_Output", PinDisplayName="")) FTG_Texture Output;
    virtual FTG_Name GetDefaultName() const override { return TEXT("Prism Invert Channels"); }
    virtual FText GetTooltipText() const override { return FText::FromString(TEXT("Independently invert R, G, B and A; built-in TextureGraph Invert remains available.")); }
    virtual void Evaluate(FTG_EvaluationContext* InContext) override;
};
