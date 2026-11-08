#include "Expressions/PrismExpression_SoftThreshold.h"
#include "PrismTransforms.h"
void UPrismExpression_SoftThreshold::Evaluate(FTG_EvaluationContext* InContext)
{
    Super::Evaluate(InContext);
    if (!Input) { Output = FTG_Texture::GetBlack(); return; }
    FPrismOpSettings Settings;
    Settings.Threshold = FMath::Clamp(Threshold,0.0f,1.0f); Settings.Radius = FMath::Clamp(Width,0.0001f,1.0f);
    Output = FPrismTransforms::Create(EPrismOp::SoftThreshold, InContext->Cycle, Output.GetBufferDescriptor(), Input, Settings, InContext->TargetId);
}
