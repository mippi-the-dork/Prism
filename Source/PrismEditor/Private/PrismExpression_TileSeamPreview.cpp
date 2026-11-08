#include "Expressions/PrismExpression_TileSeamPreview.h"
#include "PrismTransforms.h"
void UPrismExpression_TileSeamPreview::Evaluate(FTG_EvaluationContext* InContext)
{
    Super::Evaluate(InContext);
    if (!Input) { Output = FTG_Texture::GetBlack(); return; }
    FPrismOpSettings Settings;
    Settings.Amount=FMath::Clamp(Repeat,1,16); Settings.Radius=FMath::Clamp(SeamWidth,0.1f,20.0f); Settings.Strength=FMath::Clamp(Overlay,0.0f,1.0f);
    Output = FPrismTransforms::Create(EPrismOp::TileSeamPreview, InContext->Cycle, Output.GetBufferDescriptor(), Input, Settings, InContext->TargetId);
}
