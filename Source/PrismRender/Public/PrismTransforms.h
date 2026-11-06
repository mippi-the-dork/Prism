#pragma once
#include "CoreMinimal.h"
#include "Job/Job.h"
#include "Model/Mix/MixUpdateCycle.h"
#include "Data/TiledBlob.h"


/** Supported image operations. Distance/Flood Fill have documented finite search radii. */
enum class EPrismOp : uint8 { Distance, Curve, GradientMap, FloodFill, Sharpen, HighPass, Posterize };

struct FPrismOpSettings
{
    float Radius = 8.0f;
    float Threshold = 0.5f;
    float Strength = 1.0f;
    float Amount = 6.0f;
    FLinearColor A = FLinearColor::Black;
    FLinearColor B = FLinearColor(0.333f, 0.333f, 0.333f, 1);
    FLinearColor C = FLinearColor(0.667f, 0.667f, 0.667f, 1);
    FLinearColor D = FLinearColor::White;
};

class PRISMRENDER_API FPrismTransforms
{
public:
    static TiledBlobPtr Create(EPrismOp Op, MixUpdateCyclePtr Cycle, BufferDescriptor DesiredDesc,
        TiledBlobPtr Input, const FPrismOpSettings& Settings, int32 TargetId);
};
