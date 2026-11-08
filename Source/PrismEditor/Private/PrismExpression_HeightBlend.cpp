#include "Expressions/PrismExpression_HeightBlend.h"
#include "PrismTransforms.h"
void UPrismExpression_HeightBlend::Evaluate(FTG_EvaluationContext* InContext)
{
    Super::Evaluate(InContext);
    if (!Input || !InputB) { Output = FTG_Texture::GetBlack(); return; }
    FPrismOpSettings Settings;
    Settings.Radius=FMath::Clamp(Transition,0.0001f,1.0f); Settings.Threshold=FMath::Clamp(Bias,-1.0f,1.0f); Settings.Strength=FMath::Clamp(Opacity,0.0f,1.0f);
    Output = FPrismTransforms::Create(EPrismOp::HeightBlend, InContext->Cycle, Output.GetBufferDescriptor(), Input, Settings, InContext->TargetId, InputB);
}
