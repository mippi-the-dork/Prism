#include "Expressions/PrismExpression_EdgeOutline.h"
#include "PrismTransforms.h"
void UPrismExpression_EdgeOutline::Evaluate(FTG_EvaluationContext* InContext)
{
    Super::Evaluate(InContext);
    if (!Input) { Output = FTG_Texture::GetBlack(); return; }
    FPrismOpSettings Settings;
    Settings.Radius = FMath::Clamp(Radius,1,16); Settings.Strength = FMath::Clamp(Strength,0.0f,10.0f);
    Output = FPrismTransforms::Create(EPrismOp::EdgeOutline, InContext->Cycle, Output.GetBufferDescriptor(), Input, Settings, InContext->TargetId);
}
