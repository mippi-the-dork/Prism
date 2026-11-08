#pragma once
#include "CoreMinimal.h"
#include "Expressions/TG_Expression.h"
#include "TG_Texture.h"
#include "PrismNodeEnums.h"
#include "PrismExpression_PatternSampler.generated.h"

/** Repeats an input pattern with optional density map, seeded offset and random rotation. */
UCLASS(meta=(DisplayName="Prism Pattern Sampler"))
class PRISMEDITOR_API UPrismExpression_PatternSampler : public UTG_Expression
{
    GENERATED_BODY()
public:
    TG_DECLARE_EXPRESSION(TG_Category::Input);
    /** Image repeated into every cell; transparency is preserved. */
    UPROPERTY(meta=(TGType="TG_Input")) FTG_Texture Pattern;
    /** Optional grayscale probability map; white keeps every tile, black suppresses them. */
    UPROPERTY(meta=(TGType="TG_Input")) FTG_Texture Distribution;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="1", ClampMax="128")) int32 Columns = 8;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="1", ClampMax="128")) int32 Rows = 8;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="0.01", ClampMax="1.0")) float Scale = 0.9f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="0", ClampMax="0.45")) float PositionJitter = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="0", ClampMax="1")) float RandomRotation = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting")) int32 Seed = 0;
    UPROPERTY(EditAnywhere, Category=NoCategory, meta=(TGType="TG_Output", PinDisplayName="")) FTG_Texture Output;
    virtual FTG_Name GetDefaultName() const override { return TEXT("Prism Pattern Sampler"); }
    virtual FText GetTooltipText() const override { return FText::FromString(TEXT("Repeats an input pattern with optional density map, seeded offset and random rotation.")); }
    virtual void Evaluate(FTG_EvaluationContext* InContext) override;
};
