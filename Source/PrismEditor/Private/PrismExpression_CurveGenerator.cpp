#include "Expressions/PrismExpression_CurveGenerator.h"
#include "PrismTransforms.h"

void UPrismExpression_CurveGenerator::Evaluate(FTG_EvaluationContext* InContext)
{
    Super::Evaluate(InContext);
    FPrismOpSettings Settings;
    Settings.Amount = static_cast<int32>(Direction);
    for (int32 Index = 0; Index < 8; ++Index)
    {
        const float T = static_cast<float>(Index) / 7.0f;
        Settings.Samples[Index] = Curve ? FLinearColor(Curve->GetFloatValue(T), Curve->GetFloatValue(T), Curve->GetFloatValue(T), 1.0f) : FLinearColor(T, T, T, 1.0f);
    }
    BufferDescriptor Desired = Output.GetBufferDescriptor();
    if (Desired.Width <= 0) Desired.Width = 1024;
    if (Desired.Height <= 0) Desired.Height = 1024;
    if (Desired.Format == BufferFormat::Auto) Desired.Format = BufferFormat::Byte;
    if (Desired.ItemsPerPoint == 0) Desired.ItemsPerPoint = 4;
    Output = FPrismTransforms::Create(EPrismOp::CurveGenerator, InContext->Cycle, Desired,
        FTG_Texture::GetWhite().RasterBlob, Settings, InContext->TargetId);
}
