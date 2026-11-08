#include "Expressions/PrismExpression_NoiseGenerator.h"
#include "PrismTransforms.h"

void UPrismExpression_NoiseGenerator::Evaluate(FTG_EvaluationContext* InContext)
{
    Super::Evaluate(InContext);
    FPrismOpSettings Settings;
    Settings.Radius = static_cast<float>(FMath::Clamp(Frequency, 1, 256));
    Settings.Amount = static_cast<float>(static_cast<int32>(NoiseType));
    Settings.Strength = FMath::Clamp(Persistence, 0.01f, 1.0f);
    Settings.Threshold = static_cast<float>(FMath::Clamp(Octaves, 1, 8));
    Settings.A = FLinearColor(static_cast<float>(Seed), 0, 0, 1);
    BufferDescriptor Desired = Output.GetBufferDescriptor();
    if (Desired.Width <= 0) Desired.Width = 1024;
    if (Desired.Height <= 0) Desired.Height = 1024;
    if (Desired.Format == BufferFormat::Auto) Desired.Format = BufferFormat::Byte;
    if (Desired.ItemsPerPoint == 0) Desired.ItemsPerPoint = 4;
    Output = FPrismTransforms::Create(EPrismOp::NoiseGenerator, InContext->Cycle, Desired,
        FTG_Texture::GetWhite().RasterBlob, Settings, InContext->TargetId);
}
