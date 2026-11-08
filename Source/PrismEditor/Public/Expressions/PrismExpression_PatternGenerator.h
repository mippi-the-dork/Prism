#pragma once
#include "CoreMinimal.h"
#include "Expressions/TG_Expression.h"
#include "TG_Texture.h"
#include "PrismNodeEnums.h"
#include "PrismExpression_PatternGenerator.generated.h"

/** Generates repeatable shapes, checkerboards, brick bonds and per-cell seeded position jitter. */
UCLASS(meta=(DisplayName="Prism Pattern Generator"))
class PRISMEDITOR_API UPrismExpression_PatternGenerator : public UTG_Expression
{
    GENERATED_BODY()
public:
    TG_DECLARE_EXPRESSION(TG_Category::Input);
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting")) EPrismPatternShape Shape = EPrismPatternShape::Rectangle;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="1", ClampMax="128")) int32 Columns = 8;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="1", ClampMax="128")) int32 Rows = 8;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="0.01", ClampMax="1.0")) float Fill = 0.8f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="0", ClampMax="0.45")) float Jitter = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting")) int32 Seed = 0;
    UPROPERTY(EditAnywhere, Category=NoCategory, meta=(TGType="TG_Output", PinDisplayName="")) FTG_Texture Output;
    virtual FTG_Name GetDefaultName() const override { return TEXT("Prism Pattern Generator"); }
    virtual FText GetTooltipText() const override { return FText::FromString(TEXT("Generates repeatable shapes, checkerboards, brick bonds and per-cell seeded position jitter.")); }
    virtual void Evaluate(FTG_EvaluationContext* InContext) override;
};
