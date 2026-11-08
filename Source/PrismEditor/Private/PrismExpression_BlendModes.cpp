#include "Expressions/PrismExpression_BlendModes.h"
#include "PrismTransforms.h"
void UPrismExpression_BlendModes::Evaluate(FTG_EvaluationContext* InContext)
{
    Super::Evaluate(InContext);
    if (!Input || !Layer) { Output = FTG_Texture::GetBlack(); return; }
    FPrismOpSettings Settings;
    Settings.Amount = static_cast<int32>(Mode); Settings.Strength = FMath::Clamp(Opacity,0.0f,1.0f);
    Output = FPrismTransforms::Create(EPrismOp::BlendModes, InContext->Cycle, Output.GetBufferDescriptor(), Input, Settings, InContext->TargetId, Layer);
}
