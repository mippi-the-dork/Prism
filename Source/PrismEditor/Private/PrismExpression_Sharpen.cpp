#include "Expressions/PrismExpression_Sharpen.h"
#include "PrismTransforms.h"
void UPrismExpression_Sharpen::Evaluate(FTG_EvaluationContext* InContext)
{
    Super::Evaluate(InContext);
    if (!Input) { Output = FTG_Texture::GetBlack(); return; }
    FPrismOpSettings Settings;
    Settings.Strength = FMath::Clamp(Strength,0.0f,10.0f);
    Output = FPrismTransforms::Create(EPrismOp::Sharpen, InContext->Cycle, Output.GetBufferDescriptor(), Input, Settings, InContext->TargetId);
}
