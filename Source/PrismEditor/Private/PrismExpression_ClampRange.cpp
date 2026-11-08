#include "Expressions/PrismExpression_ClampRange.h"
#include "PrismTransforms.h"
void UPrismExpression_ClampRange::Evaluate(FTG_EvaluationContext* InContext)
{
    Super::Evaluate(InContext);
    if (!Input) { Output = FTG_Texture::GetBlack(); return; }
    FPrismOpSettings Settings;
    Settings.A = Minimum; Settings.B = Maximum;
    Output = FPrismTransforms::Create(EPrismOp::ClampRange, InContext->Cycle, Output.GetBufferDescriptor(), Input, Settings, InContext->TargetId);
}
