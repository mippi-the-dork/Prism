#include "Expressions/PrismExpression_GradientAsset.h"
#include "PrismTransforms.h"
void UPrismExpression_GradientAsset::Evaluate(FTG_EvaluationContext* InContext)
{
    Super::Evaluate(InContext);
    if (!Input) { Output = FTG_Texture::GetBlack(); return; }
    FPrismOpSettings Settings;
    if (Curve)
    {
        for(int32 i=0;i<8;++i)
        {
            Settings.Samples[i]=Curve->GetLinearColorValue(float(i)/7.0f);
        }
    }
    Settings.Amount = 1.0f; // Always sample source luminance.
    Output = FPrismTransforms::Create(EPrismOp::GradientAsset, InContext->Cycle, Output.GetBufferDescriptor(), Input, Settings, InContext->TargetId);
}
