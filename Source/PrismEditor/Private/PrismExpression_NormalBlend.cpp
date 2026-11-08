#include "Expressions/PrismExpression_NormalBlend.h"
#include "PrismTransforms.h"
void UPrismExpression_NormalBlend::Evaluate(FTG_EvaluationContext* InContext)
{
    Super::Evaluate(InContext);
    if (!Input || !InputB) { Output = FTG_Texture::GetBlack(); return; }
    FPrismOpSettings Settings;
    Settings.Strength=FMath::Clamp(Strength,0.0f,4.0f);
    Output = FPrismTransforms::Create(EPrismOp::NormalBlend, InContext->Cycle, Output.GetBufferDescriptor(), Input, Settings, InContext->TargetId, InputB);
}
