#include "Expressions/PrismExpression_GradientMap.h"
#include "PrismTransforms.h"
void UPrismExpression_GradientMap::Evaluate(FTG_EvaluationContext* InContext)
{
    Super::Evaluate(InContext);
    if (!Input) { Output = FTG_Texture::GetBlack(); return; }
    FPrismOpSettings Settings;
    Settings.A = ColorA; Settings.B = ColorB; Settings.C = ColorC; Settings.D = ColorD;
    Output = FPrismTransforms::Create(EPrismOp::GradientMap, InContext->Cycle, Output.GetBufferDescriptor(), Input, Settings, InContext->TargetId);
}
