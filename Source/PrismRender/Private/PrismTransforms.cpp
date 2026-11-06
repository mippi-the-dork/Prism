#include "PrismTransforms.h"
#include "FxMat/MaterialManager.h"
#include "FxMat/FxMaterial.h"
#include "Job/JobBatch.h"
#include "TextureGraphEngine.h"
#include "Transform/Utility/T_CombineTiledBlob.h"
#include "DataDrivenShaderPlatformInfo.h"

class FSH_PrismOp : public FSH_Base
{
public:
    DECLARE_GLOBAL_SHADER(FSH_PrismOp);
    SHADER_USE_PARAMETER_STRUCT(FSH_PrismOp, FSH_Base);
    BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
        SHADER_PARAMETER_STRUCT(FTileInfo, TileInfo)
        SHADER_PARAMETER_STRUCT(FStandardSamplerStates, SamplerStates)
        SHADER_PARAMETER_TEXTURE(Texture2D, Input)
        SHADER_PARAMETER(int32, Operation)
        SHADER_PARAMETER(float, Radius)
        SHADER_PARAMETER(float, Threshold)
        SHADER_PARAMETER(float, Strength)
        SHADER_PARAMETER(float, Amount)
        SHADER_PARAMETER(FVector4f, ColorA)
        SHADER_PARAMETER(FVector4f, ColorB)
        SHADER_PARAMETER(FVector4f, ColorC)
        SHADER_PARAMETER(FVector4f, ColorD)
    END_SHADER_PARAMETER_STRUCT()
    static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
    {
        return IsPCPlatform(Parameters.Platform) && EnumHasAllFlags(Parameters.Flags, EShaderPermutationFlags::HasEditorOnlyData);
    }
};
IMPLEMENT_GLOBAL_SHADER(FSH_PrismOp, "/Plugin/Prism/PrismOps.usf", "FSH_PrismOp", SF_Pixel);

TiledBlobPtr FPrismTransforms::Create(EPrismOp Op, MixUpdateCyclePtr Cycle, BufferDescriptor DesiredDesc,
    TiledBlobPtr Input, const FPrismOpSettings& Settings, int32 TargetId)
{
    if (!Input || !Cycle) return TextureHelper::GetBlack();
    const RenderMaterial_FXPtr Material = TextureGraphEngine::GetMaterialManager()
        ->CreateMaterial_FX<VSH_Simple, FSH_PrismOp>(TEXT("Prism_ProceduralOperation"));
    check(Material);
    const TiledBlobPtr Combined = T_CombineTiledBlob::Create(Cycle, Input->GetDescriptor(), TargetId, Input);
    FTileInfo TileInfo;
    JobUPtr RenderJob = std::make_unique<Job>(Cycle->GetMix(), TargetId, std::static_pointer_cast<BlobTransform>(Material));
    RenderJob->AddArg(ARG_TILEINFO(TileInfo, "TileInfo"))
        ->AddArg(ARG_BLOB(Combined, "Input"))
        ->AddArg(ARG_INT(static_cast<int32>(Op), "Operation"))
        ->AddArg(ARG_FLOAT(Settings.Radius, "Radius"))
        ->AddArg(ARG_FLOAT(Settings.Threshold, "Threshold"))
        ->AddArg(ARG_FLOAT(Settings.Strength, "Strength"))
        ->AddArg(ARG_FLOAT(Settings.Amount, "Amount"))
        ->AddArg(ARG_VECTOR(Settings.A, "ColorA"))
        ->AddArg(ARG_VECTOR(Settings.B, "ColorB"))
        ->AddArg(ARG_VECTOR(Settings.C, "ColorC"))
        ->AddArg(ARG_VECTOR(Settings.D, "ColorD"));
    const FString Name = FString::Printf(TEXT("PrismOp_%d_Batch_%llu"), static_cast<int32>(Op), Cycle->GetBatch()->GetBatchId());
    TiledBlobPtr Result = RenderJob->InitResult(Name, &DesiredDesc);
    Cycle->AddJob(TargetId, std::move(RenderJob));
    return Result;
}
