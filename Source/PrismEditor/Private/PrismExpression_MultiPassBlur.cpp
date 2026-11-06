#include "Expressions/PrismExpression_MultiPassBlur.h"
#include "Transform/Expressions/T_Blur.h"

void UPrismExpression_MultiPassBlur::Evaluate(FTG_EvaluationContext* InContext)
{
    Super::Evaluate(InContext);
    if (!Input)
    {
        Output = FTG_Texture::GetBlack();
        return;
    }
    Output = Input;
    if (Radius <= 0) return;
    const int32 SafeRadius = FMath::Clamp(Radius, 0, 100);
    const int32 SafePasses = FMath::Clamp(Passes, 1, 8);
    for (int32 PassIndex = 0; PassIndex < SafePasses; ++PassIndex)
    {
        Output = T_Blur::CreateGaussian(InContext->Cycle, Output.GetBufferDescriptor(), Output, SafeRadius, InContext->TargetId);
    }
}
