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
