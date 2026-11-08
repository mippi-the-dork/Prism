#include "Expressions/PrismExpression_LevelsRemap.h"
#include "PrismTransforms.h"
void UPrismExpression_LevelsRemap::Evaluate(FTG_EvaluationContext* InContext)
{
    Super::Evaluate(InContext);
    if (!Input) { Output = FTG_Texture::GetBlack(); return; }
    FPrismOpSettings Settings;
    Settings.A.R = FMath::Clamp(InputBlack,0.0f,1.0f); Settings.B.R = FMath::Clamp(InputWhite,0.0f,1.0f); Settings.C.R = FMath::Clamp(OutputBlack,0.0f,1.0f); Settings.D.R = FMath::Clamp(OutputWhite,0.0f,1.0f); Settings.Strength = FMath::Clamp(Gamma,0.01f,10.0f);
    Output = FPrismTransforms::Create(EPrismOp::LevelsRemap, InContext->Cycle, Output.GetBufferDescriptor(), Input, Settings, InContext->TargetId);
}
