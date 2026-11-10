#include "Expressions/PrismExpression_PatternSampler.h"
#include "PrismTransforms.h"

void UPrismExpression_PatternSampler::Evaluate(FTG_EvaluationContext* InContext)
{
    Super::Evaluate(InContext);
    FPrismOpSettings Settings;
    Settings.Radius = static_cast<float>(FMath::Clamp(Columns,1,128));
    Settings.Threshold = static_cast<float>(FMath::Clamp(Rows,1,128));
    Settings.Strength = FMath::Clamp(Scale,0.01f,1.0f);
    Settings.A = FLinearColor(FMath::Clamp(PositionJitter,0.0f,0.45f),FMath::Clamp(RandomRotation,0.0f,1.0f),static_cast<float>(Seed),1);
    Settings.B = FLinearColor(
        FMath::Clamp(ScaleMapInfluence, 0.0f, 1.0f),
        FMath::Clamp(RotationMapInfluence, 0.0f, 1.0f),
        FMath::Clamp(OffsetMapInfluence, 0.0f, 1.0f), 1.0f);
    BufferDescriptor Desired = Output.GetBufferDescriptor();
    if (Desired.Width <= 0) Desired.Width = 1024;
    if (Desired.Height <= 0) Desired.Height = 1024;
    if (Desired.Format == BufferFormat::Auto) Desired.Format = BufferFormat::Byte;
    if (Desired.ItemsPerPoint == 0) Desired.ItemsPerPoint = 4;
    Output = FPrismTransforms::Create(EPrismOp::PatternSampler, InContext->Cycle, Desired,
        (Pattern ? Pattern.RasterBlob : FTG_Texture::GetWhite().RasterBlob), Settings, InContext->TargetId,
        (Distribution ? Distribution.RasterBlob : FTG_Texture::GetWhite().RasterBlob),
        (ScaleMap ? ScaleMap.RasterBlob : nullptr),
        (RotationMap ? RotationMap.RasterBlob : nullptr),
        (OffsetMap ? OffsetMap.RasterBlob : nullptr));
}
