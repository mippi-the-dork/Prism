#include "Expressions/PrismExpression_Distance.h"
#include "PrismTransforms.h"
void UPrismExpression_Distance::Evaluate(FTG_EvaluationContext* InContext)
{
    Super::Evaluate(InContext);
    if (!Input) { Output = FTG_Texture::GetBlack(); return; }
    FPrismOpSettings Settings;
    Settings.Radius = FMath::Clamp(Radius, 1, 24); Settings.Threshold = FMath::Clamp(Threshold,0.0f,1.0f);
    Output = FPrismTransforms::Create(EPrismOp::Distance, InContext->Cycle, Output.GetBufferDescriptor(), Input, Settings, InContext->TargetId);
}
