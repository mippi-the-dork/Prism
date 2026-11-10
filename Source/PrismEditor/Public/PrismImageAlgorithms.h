#pragma once
#include "CoreMinimal.h"

/** Pure CPU reference algorithms for full-image mask processing.
 * They are independent of TextureGraph so their outputs are deterministic and unit-testable.
 * The Alpha 8 GPU nodes do NOT yet call these reference routines.
 */
class PRISMEDITOR_API FPrismImageAlgorithms
{
public:
    /** 4- or 8-connected component IDs, 0 for background; deterministic row-major labels. */
    static bool LabelRegions(const TArray<uint8>& BinaryMask, int32 Width, int32 Height,
        bool bEightConnected, TArray<int32>& OutLabels, int32& OutRegionCount);

    /** Exact pixel-center Euclidean distance (signed: positive foreground, negative background).
     * Distance is to nearest pixel of the opposite class. If no opposite pixel exists, distance
     * is capped to the image diagonal. Returns false for invalid dimensions/buffer size.
     */
    static bool SignedEuclideanDistance(const TArray<uint8>& BinaryMask, int32 Width, int32 Height,
        TArray<float>& OutDistances);
};

/** Data derived from stable connected-component labels. Background pixels are always zero. */
enum class EPrismRegionChannel : uint8
{
    RandomGrayscale,
    NormalizedIndex,
    PositionX,
    PositionY,
    LocalGradientX,
    LocalGradientY,
    AreaNormalized,
    CentroidX,
    CentroidY,
    BoundingBoxWidth,
    BoundingBoxHeight
};

/** Measurements for one connected component. Pixel coordinates are inclusive. */
struct FPrismRegionStats
{
    int32 Label = 0;
    int32 PixelCount = 0;
    int32 MinX = 0;
    int32 MinY = 0;
    int32 MaxX = 0;
    int32 MaxY = 0;
    double CentroidX = 0.0;
    double CentroidY = 0.0;
};

/** Pure CPU reference implementation for connected-region procedural data. */
class PRISMEDITOR_API FPrismRegionData
{
public:
    /** Derive exact region measurements; index 0 is the background placeholder. */
    static bool Measure(const TArray<int32>& Labels, int32 Width, int32 Height,
        TArray<FPrismRegionStats>& OutStats);

    /** Generate a grayscale buffer from 1-based region labels, where label 0 is background.
     * Invalid labels or dimensions return false; results are deterministic for a given seed.
     */
    static bool Generate(const TArray<int32>& Labels, int32 Width, int32 Height,
        EPrismRegionChannel Channel, int32 Seed, TArray<float>& OutValues);

    /** Complete CPU reference path: binary mask -> connected regions -> per-region pixels.
     * This remains a CPU reference and is not yet a TextureGraph evaluation node.
     */
    static bool GenerateFromMask(const TArray<uint8>& BinaryMask, int32 Width, int32 Height,
        bool bEightConnected, EPrismRegionChannel Channel, int32 Seed,
        TArray<float>& OutValues, int32& OutRegionCount);
};
