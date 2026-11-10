#pragma once
#include "CoreMinimal.h"
#include "Expressions/TG_Expression.h"
#include "TG_Texture.h"
#include "PrismNodeEnums.h"
#include "PrismExpression_PatternSampler.generated.h"

/** Scatters a pattern using seeded variation and optional density, scale, rotation, and offset maps. */
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
    /** Optional grayscale map sampled once per tile to vary its scale. */
    UPROPERTY(meta=(TGType="TG_Input")) FTG_Texture ScaleMap;
    /** Optional grayscale map sampled once per tile to vary its rotation. */
    UPROPERTY(meta=(TGType="TG_Input")) FTG_Texture RotationMap;
    /** Optional RG map: 0.5 means no offset, 0/1 are opposite directions. */
    UPROPERTY(meta=(TGType="TG_Input")) FTG_Texture OffsetMap;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="1", ClampMax="128")) int32 Columns = 8;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="1", ClampMax="128")) int32 Rows = 8;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="0.01", ClampMax="1.0")) float Scale = 0.9f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="0", ClampMax="0.45")) float PositionJitter = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="0", ClampMax="1")) float RandomRotation = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="0", ClampMax="1")) float ScaleMapInfluence = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="0", ClampMax="1")) float RotationMapInfluence = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="0", ClampMax="1")) float OffsetMapInfluence = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting")) int32 Seed = 0;
    UPROPERTY(EditAnywhere, Category=NoCategory, meta=(TGType="TG_Output", PinDisplayName="")) FTG_Texture Output;
    virtual FTG_Name GetDefaultName() const override { return TEXT("Prism Pattern Sampler"); }
    virtual FText GetTooltipText() const override { return FText::FromString(TEXT("Scatters a pattern using seeded variation and optional density, scale, rotation, and offset maps.")); }
    virtual void Evaluate(FTG_EvaluationContext* InContext) override;
};
