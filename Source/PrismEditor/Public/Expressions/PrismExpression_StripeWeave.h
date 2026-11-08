#pragma once
#include "CoreMinimal.h"
#include "Expressions/TG_Expression.h"
#include "TG_Texture.h"
#include "PrismNodeEnums.h"
#include "PrismExpression_StripeWeave.generated.h"

/** Tileable stripes, checkerboards and simple cross-over weave masks. */
UCLASS(meta=(DisplayName="Prism Stripe & Weave Generator"))
class PRISMEDITOR_API UPrismExpression_StripeWeave : public UTG_Expression
{
    GENERATED_BODY()
public:
    TG_DECLARE_EXPRESSION(TG_Category::Input);
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting")) EPrismStripeMode Mode = EPrismStripeMode::Horizontal;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="1", ClampMax="128")) int32 Columns = 12;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="1", ClampMax="128")) int32 Rows = 12;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=NoCategory, meta=(TGType="TG_Setting", ClampMin="0.01", ClampMax="1.0")) float Width = 0.6f;
    UPROPERTY(EditAnywhere, Category=NoCategory, meta=(TGType="TG_Output", PinDisplayName="")) FTG_Texture Output;
    virtual FTG_Name GetDefaultName() const override { return TEXT("Prism Stripe & Weave Generator"); }
    virtual FText GetTooltipText() const override { return FText::FromString(TEXT("Tileable stripes, checkerboards and simple cross-over weave masks.")); }
    virtual void Evaluate(FTG_EvaluationContext* InContext) override;
};
