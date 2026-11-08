#include "Expressions/PrismExpression_SobelNormal.h"
#include "PrismTransforms.h"
void UPrismExpression_SobelNormal::Evaluate(FTG_EvaluationContext* InContext)
{
    Super::Evaluate(InContext);
    if (!Input) { Output = FTG_Texture::GetBlack(); return; }
    FPrismOpSettings Settings;
    Settings.Radius = FMath::Clamp(Radius,1,16); Settings.Strength = FMath::Clamp(Strength,0.0f,10.0f);
    Output = FPrismTransforms::Create(EPrismOp::SobelNormal, InContext->Cycle, Output.GetBufferDescriptor(), Input, Settings, InContext->TargetId);
}
