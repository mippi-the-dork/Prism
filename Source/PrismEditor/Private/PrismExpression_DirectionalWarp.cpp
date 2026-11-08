#include "Expressions/PrismExpression_DirectionalWarp.h"
#include "PrismTransforms.h"
void UPrismExpression_DirectionalWarp::Evaluate(FTG_EvaluationContext* InContext)
{
    Super::Evaluate(InContext);
    if (!Input || !InputB) { Output = FTG_Texture::GetBlack(); return; }
    FPrismOpSettings Settings;
    Settings.Strength=FMath::Clamp(Distance,-512.0f,512.0f); Settings.Amount=Angle;
    Output = FPrismTransforms::Create(EPrismOp::DirectionalWarp, InContext->Cycle, Output.GetBufferDescriptor(), Input, Settings, InContext->TargetId, InputB);
}
