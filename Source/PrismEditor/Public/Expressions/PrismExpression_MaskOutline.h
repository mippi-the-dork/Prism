#pragma once
#include "CoreMinimal.h"
#include "Expressions/TG_Expression.h"
#include "TG_Texture.h"
#include "PrismNodeEnums.h"
#include "PrismExpression_MaskOutline.generated.h"
/** Outer, inner or centered outlines around a grayscale mask. */
UCLASS(meta=(DisplayName="Prism Mask Outline"))
class PRISMEDITOR_API UPrismExpression_MaskOutline : public UTG_Expression
{
    GENERATED_BODY()
public:
    TG_DECLARE_EXPRESSION(TG_Category::Filter);
    UPROPERTY(meta=(TGType="TG_Input", PinDisplayName="")) FTG_Texture Input;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="1", ClampMax="12")) int32 Radius = 3;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting")) EPrismOutlineMode Mode = EPrismOutlineMode::Centered;
    UPROPERTY(EditAnywhere, Category=NoCategory, meta=(TGType="TG_Output", PinDisplayName="")) FTG_Texture Output;
    virtual FTG_Name GetDefaultName() const override { return TEXT("Prism Mask Outline"); }
    virtual FText GetTooltipText() const override { return FText::FromString(TEXT("Outer, inner or centered outlines around a grayscale mask.")); }
    virtual void Evaluate(FTG_EvaluationContext* InContext) override;
};
