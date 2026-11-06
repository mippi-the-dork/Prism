#include "Expressions/PrismExpression_Posterize.h"
#include "PrismTransforms.h"
void UPrismExpression_Posterize::Evaluate(FTG_EvaluationContext* InContext)
{
    Super::Evaluate(InContext);
    if (!Input) { Output = FTG_Texture::GetBlack(); return; }
    FPrismOpSettings Settings;
    Settings.Amount = FMath::Clamp(Levels,2,256);
    Output = FPrismTransforms::Create(EPrismOp::Posterize, InContext->Cycle, Output.GetBufferDescriptor(), Input, Settings, InContext->TargetId);
}
