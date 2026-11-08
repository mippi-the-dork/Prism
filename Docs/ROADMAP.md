# Prism development roadmap

## Delivered in 0.1.0-alpha.3

- Standalone library metadata assets with TextureGraph soft references.
- Dockable library browser, multi-term search, duplicate-registration protection, category sorting and browsable source graphs.
- Asset Registry event-driven updates; no polling.
- Shared catalog validation helpers reused by browser and native automation.
- Native TextureGraph `Prism Multi-Pass Blur` expression using engine Gaussian GPU processing.
- Six automation tests.

## Next — procedural foundations

1. Distance Transform: signed/unsigned mask distance and wrap behavior; deterministic pixel comparisons.
2. Gradient Map: editable gradient ramp, grayscale/color-space semantics and GPU evaluation.
3. Curve: reflected editable key curves with deterministic LUT generation.
4. Flood Fill: stable region IDs, connectivity, tiling and island statistics.
5. Asset lifecycle tests for registry updates and UI behavior.

## Later — authoring library

- Flood Fill transforms and per-island randomization; non-uniform blur/slope blur; pattern sampler; noise and material functions.
- Starter TextureGraph compound assets, structured categorization and previews.
- Mesh-derived maps and material bundles.

## Compatibility policy

Do not patch engine-private TextureGraph editor code. Preserve existing native graph nodes. Prefer Prism-owned expression classes and public TextureGraph APIs. Maintain Native Unreal Automation Framework tests without third-party testing dependencies.

## Alpha 5 notes

Seven batch-created processing nodes using PrismRender shader module. High-quality unbounded distance transform, true connected-component Flood Fill, and editable curve/gradient stop UI are next-phase correctness requirements. See PROCEDURAL_NODES.md.

## Delivered for testing — Alpha 7

- Eleven additional TextureGraph expressions in Adjustment and Filter, sharing PrismRender's GPU operation shader.
- Two-input layered compositing support in the shared shader transform.
- Eleven new native automation registration/category tests (24 total).
- No engine source edits or changes to the established Prism Library.

## Priority follow-up

- **True flood fill:** stable connected-component labels across entire textures and tile boundaries; per-island variation and statistics.
- **Full distance transform:** beyond Alpha 6's 24-pixel limit; signed/unsigned, wrap mode, resolution-independent behavior.
- **Artist-editable curve:** freeform keys, tangent handling and predictable LUT upload.
- **Artist-editable gradient:** arbitrary stop positions/colors, interpolation and color-space behavior.
- **GPU regression suite:** execute known test textures and compare read-back output; current native automation checks only construction, reflection and metadata.
- Consider a proper enum dropdown and per-mode tooltips for Layer Blend Modes after first editor validation.
