#include "Expressions/PrismExpression_InvertChannels.h"
#include "PrismTransforms.h"
void UPrismExpression_InvertChannels::Evaluate(FTG_EvaluationContext* InContext)
{
    Super::Evaluate(InContext);
    if (!Input) { Output = FTG_Texture::GetBlack(); return; }
    FPrismOpSettings Settings;
    Settings.A = FLinearColor(InvertR ? 1.0f : 0.0f, InvertG ? 1.0f : 0.0f, InvertB ? 1.0f : 0.0f, InvertA ? 1.0f : 0.0f);
    Output = FPrismTransforms::Create(EPrismOp::InvertChannels, InContext->Cycle, Output.GetBufferDescriptor(), Input, Settings, InContext->TargetId);
}
