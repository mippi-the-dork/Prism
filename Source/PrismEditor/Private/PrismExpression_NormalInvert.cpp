#include "Expressions/PrismExpression_NormalInvert.h"
#include "PrismTransforms.h"
void UPrismExpression_NormalInvert::Evaluate(FTG_EvaluationContext* InContext)
{
    Super::Evaluate(InContext);
    if (!Input) { Output = FTG_Texture::GetBlack(); return; }
    FPrismOpSettings Settings;
    Settings.A.R=bInvertX?-1.0f:1.0f; Settings.A.G=bInvertY?-1.0f:1.0f;
    Output = FPrismTransforms::Create(EPrismOp::NormalInvert, InContext->Cycle, Output.GetBufferDescriptor(), Input, Settings, InContext->TargetId);
}
