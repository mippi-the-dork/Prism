#include "Expressions/PrismExpression_Morphology.h"
#include "PrismTransforms.h"
void UPrismExpression_Morphology::Evaluate(FTG_EvaluationContext* InContext)
{
    Super::Evaluate(InContext);
    if (!Input) { Output = FTG_Texture::GetBlack(); return; }
    FPrismOpSettings Settings;
    Settings.Radius=FMath::Clamp(Radius,1,12); Settings.Strength=bDilate?1.0f:0.0f; Settings.Amount=bCircular?2.0f:0.0f;
    Output = FPrismTransforms::Create(EPrismOp::Morphology, InContext->Cycle, Output.GetBufferDescriptor(), Input, Settings, InContext->TargetId);
}
