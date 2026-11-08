#include "Expressions/PrismExpression_MaskOutline.h"
#include "PrismTransforms.h"
void UPrismExpression_MaskOutline::Evaluate(FTG_EvaluationContext* InContext)
{
    Super::Evaluate(InContext);
    if (!Input) { Output = FTG_Texture::GetBlack(); return; }
    FPrismOpSettings Settings;
    Settings.Radius=FMath::Clamp(Radius,1,12); Settings.Amount=static_cast<int32>(Mode);
    Output = FPrismTransforms::Create(EPrismOp::MaskOutline, InContext->Cycle, Output.GetBufferDescriptor(), Input, Settings, InContext->TargetId);
}
