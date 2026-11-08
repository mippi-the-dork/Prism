#pragma once
#include "CoreMinimal.h"
#include "Expressions/TG_Expression.h"
#include "TG_Texture.h"
#include "PrismExpression_LevelsRemap.generated.h"
/** Remaps RGB through black/white input and output levels with gamma; existing TextureGraph Levels remains available. */
UCLASS(meta=(DisplayName="Prism Levels Remap"))
class PRISMEDITOR_API UPrismExpression_LevelsRemap : public UTG_Expression
{
    GENERATED_BODY()
public:
    TG_DECLARE_EXPRESSION(TG_Category::Adjustment);
    UPROPERTY(meta=(TGType="TG_Input", PinDisplayName="")) FTG_Texture Input;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="0", ClampMax="1")) float InputBlack = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="0", ClampMax="1")) float InputWhite = 1.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="0.01", ClampMax="10")) float Gamma = 1.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="0", ClampMax="1")) float OutputBlack = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="0", ClampMax="1")) float OutputWhite = 1.0f;
    UPROPERTY(EditAnywhere, Category=NoCategory, meta=(TGType="TG_Output", PinDisplayName="")) FTG_Texture Output;
    virtual FTG_Name GetDefaultName() const override { return TEXT("Prism Levels Remap"); }
    virtual FText GetTooltipText() const override { return FText::FromString(TEXT("Remaps RGB through black/white input and output levels with gamma; existing TextureGraph Levels remains available.")); }
    virtual void Evaluate(FTG_EvaluationContext* InContext) override;
};
