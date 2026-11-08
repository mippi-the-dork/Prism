#include "Expressions/PrismExpression_GammaCorrection.h"
#include "PrismTransforms.h"
void UPrismExpression_GammaCorrection::Evaluate(FTG_EvaluationContext* InContext)
{
    Super::Evaluate(InContext);
    if (!Input) { Output = FTG_Texture::GetBlack(); return; }
    FPrismOpSettings Settings;
    Settings.Strength = FMath::Clamp(Gamma,0.01f,10.0f);
    Output = FPrismTransforms::Create(EPrismOp::GammaCorrection, InContext->Cycle, Output.GetBufferDescriptor(), Input, Settings, InContext->TargetId);
}
