#include "Expressions/PrismExpression_HighPass.h"
#include "PrismTransforms.h"
void UPrismExpression_HighPass::Evaluate(FTG_EvaluationContext* InContext)
{
    Super::Evaluate(InContext);
    if (!Input) { Output = FTG_Texture::GetBlack(); return; }
    FPrismOpSettings Settings;
    Settings.Strength = FMath::Clamp(Strength,0.0f,10.0f);
    Output = FPrismTransforms::Create(EPrismOp::HighPass, InContext->Cycle, Output.GetBufferDescriptor(), Input, Settings, InContext->TargetId);
}
