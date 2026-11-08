#include "Expressions/PrismExpression_GradientGenerator.h"
#include "PrismTransforms.h"

void UPrismExpression_GradientGenerator::Evaluate(FTG_EvaluationContext* InContext)
{
    Super::Evaluate(InContext);
    FPrismOpSettings Settings;
    Settings.Amount = static_cast<int32>(Direction);
    for (int32 Index = 0; Index < 8; ++Index)
    {
        const float T = static_cast<float>(Index) / 7.0f;
        Settings.Samples[Index] = Curve ? Curve->GetLinearColorValue(T) : FLinearColor(T, T, T, 1.0f);
    }
    BufferDescriptor Desired = Output.GetBufferDescriptor();
    if (Desired.Width <= 0) Desired.Width = 1024;
    if (Desired.Height <= 0) Desired.Height = 1024;
    if (Desired.Format == BufferFormat::Auto) Desired.Format = BufferFormat::Byte;
    if (Desired.ItemsPerPoint == 0) Desired.ItemsPerPoint = 4;
    Output = FPrismTransforms::Create(EPrismOp::GradientGenerator, InContext->Cycle, Desired,
        FTG_Texture::GetWhite().RasterBlob, Settings, InContext->TargetId);
}
