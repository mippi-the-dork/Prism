#include "Expressions/PrismExpression_LocalContrast.h"
#include "PrismTransforms.h"
void UPrismExpression_LocalContrast::Evaluate(FTG_EvaluationContext* InContext)
{
    Super::Evaluate(InContext);
    if (!Input) { Output = FTG_Texture::GetBlack(); return; }
    FPrismOpSettings Settings;
    Settings.Radius = FMath::Clamp(Radius,1,16); Settings.Strength = FMath::Clamp(Strength,0.0f,10.0f);
    Output = FPrismTransforms::Create(EPrismOp::LocalContrast, InContext->Cycle, Output.GetBufferDescriptor(), Input, Settings, InContext->TargetId);
}
