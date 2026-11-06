#pragma once
#include "CoreMinimal.h"
#include "Expressions/TG_Expression.h"
#include "TG_Texture.h"
#include "PrismExpression_GradientMap.generated.h"
/** Maps grayscale intensity to four editable RGBA color stops. */
UCLASS(meta=(DisplayName="Prism Gradient Map"))
class PRISMEDITOR_API UPrismExpression_GradientMap : public UTG_Expression
{
    GENERATED_BODY()
public:
    TG_DECLARE_EXPRESSION(TG_Category::Adjustment);
    UPROPERTY(meta=(TGType="TG_Input", PinDisplayName="")) FTG_Texture Input;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting")) FLinearColor ColorA = FLinearColor::Black;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting")) FLinearColor ColorB = FLinearColor::Red;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting")) FLinearColor ColorC = FLinearColor::Yellow;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting")) FLinearColor ColorD = FLinearColor::White;
    UPROPERTY(EditAnywhere, Category=NoCategory, meta=(TGType="TG_Output", PinDisplayName="")) FTG_Texture Output;

    virtual FTG_Name GetDefaultName() const override { return TEXT("Prism Gradient Map"); }
    virtual FText GetTooltipText() const override { return FText::FromString(TEXT("Maps grayscale intensity to four editable RGBA color stops.")); }
    virtual void Evaluate(FTG_EvaluationContext* InContext) override;
};
