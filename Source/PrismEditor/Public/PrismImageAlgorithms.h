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
