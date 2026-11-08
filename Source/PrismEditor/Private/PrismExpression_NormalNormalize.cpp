#include "Expressions/PrismExpression_NormalNormalize.h"
#include "PrismTransforms.h"
void UPrismExpression_NormalNormalize::Evaluate(FTG_EvaluationContext* InContext)
{
    Super::Evaluate(InContext);
    if (!Input) { Output = FTG_Texture::GetBlack(); return; }
    FPrismOpSettings Settings;
    
    Output = FPrismTransforms::Create(EPrismOp::NormalNormalize, InContext->Cycle, Output.GetBufferDescriptor(), Input, Settings, InContext->TargetId);
}
