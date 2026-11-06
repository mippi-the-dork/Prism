#include "Expressions/PrismExpression_Curve.h"
#include "PrismTransforms.h"
void UPrismExpression_Curve::Evaluate(FTG_EvaluationContext* InContext)
{
    Super::Evaluate(InContext);
    if (!Input) { Output = FTG_Texture::GetBlack(); return; }
    FPrismOpSettings Settings;
    Settings.A.R = FMath::Clamp(ValueA,0.0f,1.0f); Settings.B.R = FMath::Clamp(ValueB,0.0f,1.0f); Settings.C.R = FMath::Clamp(ValueC,0.0f,1.0f); Settings.D.R = FMath::Clamp(ValueD,0.0f,1.0f);
    Output = FPrismTransforms::Create(EPrismOp::Curve, InContext->Cycle, Output.GetBufferDescriptor(), Input, Settings, InContext->TargetId);
}
