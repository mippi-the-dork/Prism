#include "Expressions/PrismExpression_PatternGenerator.h"
#include "PrismTransforms.h"

void UPrismExpression_PatternGenerator::Evaluate(FTG_EvaluationContext* InContext)
{
    Super::Evaluate(InContext);
    FPrismOpSettings Settings;
    Settings.Radius = static_cast<float>(FMath::Clamp(Columns,1,128));
    Settings.Threshold = static_cast<float>(FMath::Clamp(Rows,1,128));
    Settings.Amount = static_cast<float>(static_cast<int32>(Shape));
    Settings.Strength = FMath::Clamp(Fill,0.01f,1.0f);
    Settings.A = FLinearColor(FMath::Clamp(Jitter,0.0f,0.45f),static_cast<float>(Seed),0,1);
    BufferDescriptor Desired = Output.GetBufferDescriptor();
    if (Desired.Width <= 0) Desired.Width = 1024;
    if (Desired.Height <= 0) Desired.Height = 1024;
    if (Desired.Format == BufferFormat::Auto) Desired.Format = BufferFormat::Byte;
    if (Desired.ItemsPerPoint == 0) Desired.ItemsPerPoint = 4;
    Output = FPrismTransforms::Create(EPrismOp::PatternGenerator, InContext->Cycle, Desired,
        FTG_Texture::GetWhite().RasterBlob, Settings, InContext->TargetId);
}
