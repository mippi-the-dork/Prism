#pragma once
#include "CoreMinimal.h"
#include "Expressions/TG_Expression.h"
#include "TG_Texture.h"
#include "PrismNodeEnums.h"
#include "PrismExpression_BlendModes.generated.h"
/** Combines Base and Layer textures with eight standard blend modes and layer opacity. */
UCLASS(meta=(DisplayName="Prism Layer Blend Modes"))
class PRISMEDITOR_API UPrismExpression_BlendModes : public UTG_Expression
{
    GENERATED_BODY()
public:
    TG_DECLARE_EXPRESSION(TG_Category::Filter);
    UPROPERTY(meta=(TGType="TG_Input", PinDisplayName="Base")) FTG_Texture Input;
    UPROPERTY(meta=(TGType="TG_Input", PinDisplayName="Layer")) FTG_Texture Layer;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting")) EPrismBlendMode Mode = EPrismBlendMode::Normal;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="0", ClampMax="1")) float Opacity = 1.0f;
    UPROPERTY(EditAnywhere, Category=NoCategory, meta=(TGType="TG_Output", PinDisplayName="")) FTG_Texture Output;
    virtual FTG_Name GetDefaultName() const override { return TEXT("Prism Layer Blend Modes"); }
    virtual FText GetTooltipText() const override { return FText::FromString(TEXT("Combines Base and Layer textures with eight standard blend modes and layer opacity.")); }
    virtual void Evaluate(FTG_EvaluationContext* InContext) override;
};
