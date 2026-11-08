#include "Expressions/PrismExpression_Curve.h"
#include "PrismTransforms.h"

UPrismExpression_Curve::UPrismExpression_Curve()
{
    // A useful identity remap when first created.
    if (FRichCurve* RichCurve = Curve.GetRichCurve())
    {
        RichCurve->AddKey(0.0f, 0.0f);
        RichCurve->AddKey(1.0f, 1.0f);
    }
}

void UPrismExpression_Curve::Evaluate(FTG_EvaluationContext* InContext)
{
    Super::Evaluate(InContext);
    if (!Input) { Output = FTG_Texture::GetBlack(); return; }
    FPrismOpSettings Settings;
    if (const FRichCurve* RichCurve = Curve.GetRichCurveConst())
    {
        for (int32 Index = 0; Index < 8; ++Index)
        {
            const float Value = RichCurve->Eval(static_cast<float>(Index) / 7.0f);
            Settings.Samples[Index] = FLinearColor(Value, Value, Value, 1.0f);
        }
    }
    Settings.Amount = 1.0f;
    Output = FPrismTransforms::Create(EPrismOp::CurveAsset, InContext->Cycle, Output.GetBufferDescriptor(), Input, Settings, InContext->TargetId);
}
