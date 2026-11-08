#pragma once
#include "CoreMinimal.h"
#include "Expressions/TG_Expression.h"
#include "TG_Texture.h"
#include "PrismNodeEnums.h"
#include "PrismExpression_NoiseGenerator.generated.h"

/** Deterministic seamless procedural value noise and fractal variants. */
UCLASS(meta=(DisplayName="Prism Fractal Noise"))
class PRISMEDITOR_API UPrismExpression_NoiseGenerator : public UTG_Expression
{
    GENERATED_BODY()
public:
    TG_DECLARE_EXPRESSION(TG_Category::Input);
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting")) EPrismNoiseType NoiseType = EPrismNoiseType::Fractal;
    /** Base number of cells across the texture. Integer frequencies ensure seamless tiling. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="1", ClampMax="256")) int32 Frequency = 8;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="1", ClampMax="8")) int32 Octaves = 5;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="0.01", ClampMax="1.0")) float Persistence = 0.5f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting")) int32 Seed = 0;
    UPROPERTY(EditAnywhere, Category=NoCategory, meta=(TGType="TG_Output", PinDisplayName="")) FTG_Texture Output;
    virtual FTG_Name GetDefaultName() const override { return TEXT("Prism Fractal Noise"); }
    virtual FText GetTooltipText() const override { return FText::FromString(TEXT("Tileable value noise, fBm, ridged, and billow noise. Integer frequency and octaves; deterministic seed.")); }
    virtual void Evaluate(FTG_EvaluationContext* InContext) override;
};
