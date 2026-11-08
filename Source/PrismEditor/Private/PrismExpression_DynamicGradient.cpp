#include "Expressions/PrismExpression_DynamicGradient.h"
#include "PrismTransforms.h"
void UPrismExpression_DynamicGradient::Evaluate(FTG_EvaluationContext* InContext)
{
    Super::Evaluate(InContext);
    if (!Input || !InputB) { Output = FTG_Texture::GetBlack(); return; }
    FPrismOpSettings Settings;
    Settings.Amount=FMath::Clamp(GradientRow,0.0f,1.0f);
    Output = FPrismTransforms::Create(EPrismOp::DynamicGradient, InContext->Cycle, Output.GetBufferDescriptor(), Input, Settings, InContext->TargetId, InputB);
}
