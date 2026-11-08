#include "PrismImageAlgorithms.h"

namespace PrismImagePrivate
{
    constexpr double InfiniteDistance = 1.0e12;

    // Felzenszwalb-Huttenlocher lower-envelope 1D squared Euclidean distance transform.
    void Distance1D(const TArray<double>& Source, int32 N, TArray<double>& Output)
    {
        TArray<int32> Sites;
        TArray<double> Boundaries;
        Sites.SetNumUninitialized(N);
        Boundaries.SetNumUninitialized(N + 1);
        Output.SetNumUninitialized(N);
        int32 K = 0;
        Sites[0] = 0;
        Boundaries[0] = -InfiniteDistance;
        Boundaries[1] = InfiniteDistance;
        for (int32 Q = 1; Q < N; ++Q)
        {
            double Intersection = 0.0;
            while (true)
            {
                const int32 V = Sites[K];
                Intersection = ((Source[Q] + double(Q) * Q) - (Source[V] + double(V) * V)) / (2.0 * (Q - V));
                if (Intersection > Boundaries[K] || K == 0) break;
                --K;
            }
            if (Intersection <= Boundaries[K] && K == 0)
            {
                Sites[0] = Q;
                Boundaries[0] = -InfiniteDistance;
                Boundaries[1] = InfiniteDistance;
            }
            else
            {
                ++K;
                Sites[K] = Q;
                Boundaries[K] = Intersection;
                Boundaries[K + 1] = InfiniteDistance;
            }
        }
        K = 0;
        for (int32 Q = 0; Q < N; ++Q)
        {
            while (Boundaries[K + 1] < Q) ++K;
            const double Delta = double(Q - Sites[K]);
            Output[Q] = Delta * Delta + Source[Sites[K]];
        }
    }

    void DistanceToClass(const TArray<uint8>& Mask, int32 Width, int32 Height,
        uint8 TargetClass, TArray<double>& OutSquared)
    {
        TArray<double> Intermediate;
        Intermediate.SetNumUninitialized(Width * Height);
        TArray<double> InputLine, OutputLine;
        InputLine.SetNumUninitialized(FMath::Max(Width, Height));
        for (int32 Y = 0; Y < Height; ++Y)
        {
            for (int32 X = 0; X < Width; ++X)
                InputLine[X] = ((Mask[Y * Width + X] != 0) == (TargetClass != 0)) ? 0.0 : InfiniteDistance;
            Distance1D(InputLine, Width, OutputLine);
            for (int32 X = 0; X < Width; ++X) Intermediate[Y * Width + X] = OutputLine[X];
        }
        OutSquared.SetNumUninitialized(Width * Height);
        InputLine.SetNumUninitialized(Height);
        for (int32 X = 0; X < Width; ++X)
        {
            for (int32 Y = 0; Y < Height; ++Y) InputLine[Y] = Intermediate[Y * Width + X];
            Distance1D(InputLine, Height, OutputLine);
            for (int32 Y = 0; Y < Height; ++Y) OutSquared[Y * Width + X] = OutputLine[Y];
        }
    }
}

bool FPrismImageAlgorithms::LabelRegions(const TArray<uint8>& BinaryMask, int32 Width, int32 Height,
    bool bEightConnected, TArray<int32>& OutLabels, int32& OutRegionCount)
{
    OutLabels.Reset();
    OutRegionCount = 0;
    if (Width <= 0 || Height <= 0 || int64(Width) * Height != BinaryMask.Num()) return false;
    const int32 Num = BinaryMask.Num();
    OutLabels.Init(0, Num);
    TArray<int32> Queue;
    Queue.Reserve(Num);
    constexpr int32 DX[8] = {-1, 1, 0, 0, -1, 1, -1, 1};
    constexpr int32 DY[8] = {0, 0, -1, 1, -1, -1, 1, 1};
    for (int32 Start = 0; Start < Num; ++Start)
    {
        if (!BinaryMask[Start] || OutLabels[Start]) continue;
        ++OutRegionCount;
        Queue.Reset();
        Queue.Add(Start);
        OutLabels[Start] = OutRegionCount;
        for (int32 Head = 0; Head < Queue.Num(); ++Head)
        {
            const int32 Index = Queue[Head];
            const int32 X = Index % Width, Y = Index / Width;
            for (int32 Direction = 0; Direction < (bEightConnected ? 8 : 4); ++Direction)
            {
                const int32 NX = X + DX[Direction], NY = Y + DY[Direction];
                if (NX < 0 || NX >= Width || NY < 0 || NY >= Height) continue;
                const int32 Neighbor = NY * Width + NX;
                if (!BinaryMask[Neighbor] || OutLabels[Neighbor]) continue;
                OutLabels[Neighbor] = OutRegionCount;
                Queue.Add(Neighbor);
            }
        }
    }
    return true;
}

bool FPrismImageAlgorithms::SignedEuclideanDistance(const TArray<uint8>& BinaryMask, int32 Width, int32 Height,
    TArray<float>& OutDistances)
{
    OutDistances.Reset();
    if (Width <= 0 || Height <= 0 || int64(Width) * Height != BinaryMask.Num()) return false;
    TArray<double> ToBackground, ToForeground;
    PrismImagePrivate::DistanceToClass(BinaryMask, Width, Height, 0, ToBackground);
    PrismImagePrivate::DistanceToClass(BinaryMask, Width, Height, 1, ToForeground);
    const float Cap = FMath::Sqrt(float(Width) * Width + float(Height) * Height);
    OutDistances.SetNumUninitialized(BinaryMask.Num());
    for (int32 Index = 0; Index < BinaryMask.Num(); ++Index)
    {
        const bool bInside = BinaryMask[Index] != 0;
        const double Squared = bInside ? ToBackground[Index] : ToForeground[Index];
        const float D = Squared >= PrismImagePrivate::InfiniteDistance * 0.5 ? Cap : FMath::Sqrt(float(Squared));
        OutDistances[Index] = bInside ? D : -D;
    }
    return true;
}
