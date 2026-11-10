#include "PrismImageAlgorithms.h"

namespace
{
    float StableRegionRandom(int32 Region, int32 Seed)
    {
        // Integer hash makes region randomization stable independent of thread order.
        uint32 V = static_cast<uint32>(Region) * 0x9E3779B9u + static_cast<uint32>(Seed);
        V ^= V >> 16;
        V *= 0x7FEB352Du;
        V ^= V >> 15;
        V *= 0x846CA68Bu;
        V ^= V >> 16;
        return float(V & 0x00FFFFFFu) / float(0x00FFFFFFu);
    }
}

bool FPrismRegionData::Measure(const TArray<int32>& Labels, int32 Width, int32 Height,
    TArray<FPrismRegionStats>& OutStats)
{
    OutStats.Reset();
    if (Width <= 0 || Height <= 0 || int64(Width) * Height != Labels.Num()) return false;
    int32 MaxLabel = 0;
    for (int32 Label : Labels)
    {
        // A connected-component label can never exceed the number of pixels.
        // Reject invalid inputs before allocating one element per label.
        if (Label < 0 || Label > Labels.Num()) return false;
        MaxLabel = FMath::Max(MaxLabel, Label);
    }
    OutStats.SetNum(MaxLabel + 1);
    TArray<double> SumX, SumY;
    SumX.Init(0.0, MaxLabel + 1);
    SumY.Init(0.0, MaxLabel + 1);
    for (int32 Label = 1; Label <= MaxLabel; ++Label)
    {
        FPrismRegionStats& Stat = OutStats[Label];
        Stat.Label = Label;
        Stat.MinX = Width;
        Stat.MinY = Height;
        Stat.MaxX = -1;
        Stat.MaxY = -1;
    }
    for (int32 Y = 0; Y < Height; ++Y)
    {
        for (int32 X = 0; X < Width; ++X)
        {
            const int32 Label = Labels[Y * Width + X];
            if (Label == 0) continue;
            FPrismRegionStats& Stat = OutStats[Label];
            ++Stat.PixelCount;
            Stat.MinX = FMath::Min(Stat.MinX, X);
            Stat.MinY = FMath::Min(Stat.MinY, Y);
            Stat.MaxX = FMath::Max(Stat.MaxX, X);
            Stat.MaxY = FMath::Max(Stat.MaxY, Y);
            SumX[Label] += X + 0.5;
            SumY[Label] += Y + 0.5;
        }
    }
    for (int32 Label = 1; Label <= MaxLabel; ++Label)
    {
        FPrismRegionStats& Stat = OutStats[Label];
        if (!Stat.PixelCount) return false; // sparse IDs are not a valid region map
        Stat.CentroidX = SumX[Label] / Stat.PixelCount;
        Stat.CentroidY = SumY[Label] / Stat.PixelCount;
    }
    return true;
}

bool FPrismRegionData::Generate(const TArray<int32>& Labels, int32 Width, int32 Height,
    EPrismRegionChannel Channel, int32 Seed, TArray<float>& OutValues)
{
    OutValues.Reset();
    TArray<FPrismRegionStats> Stats;
    if (!Measure(Labels, Width, Height, Stats)) return false;
    const int32 MaxLabel = Stats.Num() - 1;
    OutValues.SetNumZeroed(Labels.Num());
    for (int32 Y = 0; Y < Height; ++Y)
    {
        for (int32 X = 0; X < Width; ++X)
        {
            const int32 Index = Y * Width + X;
            const int32 Label = Labels[Index];
            if (Label == 0) continue;
            const FPrismRegionStats& Stat = Stats[Label];
            float Value = 0.0f;
            switch (Channel)
            {
                case EPrismRegionChannel::RandomGrayscale: Value = StableRegionRandom(Label, Seed); break;
                case EPrismRegionChannel::NormalizedIndex: Value = float(Label) / FMath::Max(MaxLabel, 1); break;
                case EPrismRegionChannel::PositionX: Value = (float(Stat.MinX + Stat.MaxX) * 0.5f + 0.5f) / Width; break;
                case EPrismRegionChannel::PositionY: Value = (float(Stat.MinY + Stat.MaxY) * 0.5f + 0.5f) / Height; break;
                case EPrismRegionChannel::LocalGradientX: Value = (float(X - Stat.MinX) + 0.5f) / (Stat.MaxX - Stat.MinX + 1); break;
                case EPrismRegionChannel::LocalGradientY: Value = (float(Y - Stat.MinY) + 0.5f) / (Stat.MaxY - Stat.MinY + 1); break;
                case EPrismRegionChannel::AreaNormalized: Value = float(Stat.PixelCount) / float(Width * Height); break;
                case EPrismRegionChannel::CentroidX: Value = float(Stat.CentroidX / Width); break;
                case EPrismRegionChannel::CentroidY: Value = float(Stat.CentroidY / Height); break;
                case EPrismRegionChannel::BoundingBoxWidth: Value = float(Stat.MaxX - Stat.MinX + 1) / Width; break;
                case EPrismRegionChannel::BoundingBoxHeight: Value = float(Stat.MaxY - Stat.MinY + 1) / Height; break;
                default: return false;
            }
            OutValues[Index] = FMath::Clamp(Value, 0.0f, 1.0f);
        }
    }
    return true;
}

bool FPrismRegionData::GenerateFromMask(const TArray<uint8>& BinaryMask, int32 Width, int32 Height,
    bool bEightConnected, EPrismRegionChannel Channel, int32 Seed,
    TArray<float>& OutValues, int32& OutRegionCount)
{
    OutValues.Reset();
    OutRegionCount = 0;
    TArray<int32> Labels;
    if (!FPrismImageAlgorithms::LabelRegions(BinaryMask, Width, Height,
        bEightConnected, Labels, OutRegionCount)) return false;
    if (!Generate(Labels, Width, Height, Channel, Seed, OutValues))
    {
        OutValues.Reset();
        OutRegionCount = 0;
        return false;
    }
    return true;
}
