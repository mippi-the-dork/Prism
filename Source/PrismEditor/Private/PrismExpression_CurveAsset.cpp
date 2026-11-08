#include "Expressions/PrismExpression_CurveAsset.h"
#include "PrismTransforms.h"
void UPrismExpression_CurveAsset::Evaluate(FTG_EvaluationContext* InContext)
{
    Super::Evaluate(InContext);
    if (!Input) { Output = FTG_Texture::GetBlack(); return; }
    FPrismOpSettings Settings;
    if (Curve)
    {
        for(int32 i=0;i<8;++i)
        {
            const float V=Curve->GetFloatValue(float(i)/7.0f); Settings.Samples[i]=FLinearColor(V,V,V,1.0f);
        }
    }
    Settings.Amount = 1.0f; // Always sample source luminance.
    Output = FPrismTransforms::Create(EPrismOp::CurveAsset, InContext->Cycle, Output.GetBufferDescriptor(), Input, Settings, InContext->TargetId);
}
