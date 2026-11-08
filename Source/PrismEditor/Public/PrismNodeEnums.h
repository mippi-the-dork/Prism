#pragma once
#include "CoreMinimal.h"
#include "PrismNodeEnums.generated.h"

/** Compositing operations for Prism Layer Blend Modes. */
UENUM(BlueprintType)
enum class EPrismBlendMode : uint8
{
    Normal UMETA(DisplayName="Normal"),
    Multiply UMETA(DisplayName="Multiply"),
    Screen UMETA(DisplayName="Screen"),
    Add UMETA(DisplayName="Add"),
    Subtract UMETA(DisplayName="Subtract"),
    Overlay UMETA(DisplayName="Overlay"),
    Minimum UMETA(DisplayName="Minimum"),
    Maximum UMETA(DisplayName="Maximum")
};

/** The area of the mask used to create the outline. */
UENUM(BlueprintType)
enum class EPrismOutlineMode : uint8
{
    Outer UMETA(DisplayName="Outer"),
    Inner UMETA(DisplayName="Inner"),
    Centered UMETA(DisplayName="Centered")
};

/** Spatial gradient orientation. */
UENUM(BlueprintType)
enum class EPrismGradientDirection : uint8
{
    Horizontal UMETA(DisplayName="Horizontal (Left to Right)"),
    Vertical UMETA(DisplayName="Vertical (Bottom to Top)"),
    Radial UMETA(DisplayName="Radial (Center to Corners)"),
    Angular UMETA(DisplayName="Angular")
};

/** Procedural noise algorithm. */
UENUM(BlueprintType)
enum class EPrismNoiseType : uint8
{
    Value UMETA(DisplayName="Value Noise"),
    Fractal UMETA(DisplayName="Fractal Brownian Motion (fBm)"),
    Ridged UMETA(DisplayName="Ridged Fractal"),
    Billow UMETA(DisplayName="Billow Fractal")
};

/** Shape drawn inside each repeat cell. */
UENUM(BlueprintType)
enum class EPrismPatternShape : uint8
{
    Rectangle UMETA(DisplayName="Rectangle"),
    Circle UMETA(DisplayName="Circle"),
    Diamond UMETA(DisplayName="Diamond"),
    Hexagon UMETA(DisplayName="Hexagon"),
    RoundedRectangle UMETA(DisplayName="Rounded Rectangle"),
    Checkerboard UMETA(DisplayName="Checkerboard"),
    Brick UMETA(DisplayName="Brick Bond"),
    Cross UMETA(DisplayName="Cross")
};

UENUM(BlueprintType)
enum class EPrismCellDataMode : uint8
{
    RandomGrayscale UMETA(DisplayName="Random Grayscale"),
    RandomColor UMETA(DisplayName="Random Color"),
    CellID UMETA(DisplayName="Cell ID (Normalized)"),
    PositionX UMETA(DisplayName="Cell Position X"),
    PositionY UMETA(DisplayName="Cell Position Y")
};

UENUM(BlueprintType)
enum class EPrismStripeMode : uint8
{
    Horizontal UMETA(DisplayName="Horizontal Stripes"),
    Vertical UMETA(DisplayName="Vertical Stripes"),
    Checker UMETA(DisplayName="Checker"),
    Weave UMETA(DisplayName="Simple Weave")
};
