# Prism

Prism expands Unreal Engine's TextureGraph with a searchable reusable-graph library and native procedural processing nodes. It does not replace or modify TextureGraph or change existing graph assets.

**Version:** 0.1.0-alpha.11 — Unreal Engine 5.8.x, editor-only, source distribution.

## Install

Extract the ZIP so the plugin lives at `YourProject/Plugins/Prism/Prism.uplugin`. Enable TextureGraph and Prism, restart the editor, and build your project editor target.

## Library

Open **Tools > Prism Library**. Select a TextureGraph asset in the Content Browser and click **Register Selected** to create a `PF_` library metadata asset under `/Game/PrismLibrary`. Save the entry; set its category, subcategory, tags, description and display name in its Details editor. Prism's browser lets you search and open the graph, browse it in the Content Browser, or edit its metadata. Drag the source TextureGraph asset from the Content Browser into another TextureGraph canvas to insert the native subgraph.

Library search supports multiple case-insensitive terms across labels, descriptions, category, subcategory, tags and asset path. Registrations for the same graph are rejected by **Register Selected**; pre-existing duplicate metadata assets can still be created manually and are not destroyed. Prism listens to Asset Registry add/update/delete/rename events for `UPrismFunction` and batches a deferred refresh instead of polling. **Refresh** remains available for unsaved metadata edits.

Prism metadata is separate from TextureGraph assets. Original graphs and native TextureGraph capabilities are unaffected.

## Procedural nodes

**Prism Multi-Pass Blur** is a native TextureGraph expression under **Filter**. It applies 1–8 sequential Gaussian passes through TextureGraph's existing GPU blur implementation. Use `Radius` and `Passes` to control smoothing. Radius 0 passes through the source image; disconnected input produces black. This is **multi-pass Gaussian filtering**, not a novel high-quality Gaussian kernel or a substitute for the planned variable/non-uniform blur.

## Automation

Use the Unreal editor console command:

```text
Automation RunTests Prism
```

Or use **Tools > Session Frontend > Automation**, filter `Prism`, then run the tests.

Available tests:

- `Prism.Library.EntryDefaults`
- `Prism.Library.ReferenceMetadata`
- `Prism.Library.SearchAndCategories`
- `Prism.Library.DuplicateGraphProtection`
- `Prism.Library.DeprecationAndReplacement`
- `Prism.Nodes.MultiPassBlurRegistration`

These tests exercise metadata, filtering, duplicate protection, replacement validation and native expression registration. They **do not** yet execute a rendered GPU blur pass, automate Content Browser UI gestures, or verify graph insertion. Run the manual checks below as well.

## Manual verification

1. Open Tools > Prism Library; verify existing library records appear.
2. Select an unregistered TextureGraph asset; Register Selected; save the created entry.
3. Try registering the same graph again; verify Prism logs a duplicate warning without creating another entry.
4. Change the entry's tags and category, save, then search for each; verify search is case insensitive and uses all terms.
5. Rename or delete a Prism entry in the Content Browser; verify the docked library updates without manually clicking Refresh.
6. Locate and drag a registered source TextureGraph asset onto another TextureGraph canvas, then verify the native subgraph evaluates.
7. Add **Prism Multi-Pass Blur** to a TextureGraph. Connect an input, vary Radius/Passes, verify image output; test Radius 0 and disconnected input.
8. Run `Automation RunTests Prism` and confirm 24 successes (13 existing and 11 new node-registration tests).

## Scope and limitations

- The TextureGraph native Palette does not provide a supported third-party library-entry registration API. Prism uses a separate dockable library browser; Prism's native C++ expressions appear in TextureGraph's native node list.
- The Alpha 5/6 baseline has limited Distance (bounded search), Curve (editable embedded remap), Gradient Map (four fixed stops), and Region Variation (local proxy, **not true Flood Fill**). Full production-equivalent implementations remain planned. See `Docs/PROCEDURAL_NODES.md`.
- No changes are made to Epic TextureGraph source. No Probe dependency is required.
- This source package has not been compiled against a local Unreal Editor installation in the packaging environment. Test in your UE 5.8.3 project before shipping.

See `Docs/ROADMAP.md`.


## Alpha 4 build correction

The PrismEditor module now explicitly links Unreal Engine `RHI` and `RenderCore` because the Multi-Pass Blur expression references sampler-state and render-resource implementations from those modules. This corrects the unresolved external linker symbols reported in Alpha 3. No processing behavior or library UI was changed.

## Alpha 5 procedural node batch

New TextureGraph nodes: Distance (Bounded), Curve (Four-Knot), Gradient Map, Region Variation (Local), Sharpen, High Pass, and Posterize. These are initial implementations with documented restrictions. **The local variation node is not a true Flood Fill.** See `Docs/PROCEDURAL_NODES.md` for usage and limitations. The shader module loads early to register global shaders; the editor UI continues loading separately.

## Alpha 7 — filter/compositing batch

Eleven new native TextureGraph expressions: Soft Threshold, Levels Remap, Color Clamp Range, Invert Channels, Sobel Edge Outline, Sobel Normal From Height, Directional Emboss, Bevel Lighting Preview, Local Contrast, Layer Blend Modes (two texture inputs, eight modes), and Gamma Adjustment.

These **augment** the existing TextureGraph operations rather than replace them. Their behavior, constraints, numeric blend-mode selector, and manual test procedures are in `Docs/PROCEDURAL_NODES.md`. Alpha 7 has 24 automation registration/library tests; rendered GPU results still need manual verification.


## Alpha 8 additions

Adds 11 more authoring expressions for morphological masks, normal processing, warp and blending, tiled seam visualization, and curve-asset-based gradient/curve mapping. Exact CPU reference algorithms for connected regions and Euclidean distances are included with tests, but are not yet wired to the TextureGraph GPU evaluator. See `Docs/ALPHA8_NOTES.md` and `Docs/PROCEDURAL_NODES.md` for limitations and manual test instructions.

## Alpha 10: Separate adjustments from generators

Curve, Gradient Map, Curve Asset, and Gradient Asset are now adjustment operations. They require a source texture and remap its luminance; they no longer silently create gradients when the source is missing. The built-in **Curve** uses an editable Unreal runtime float curve instead of four fixed numeric controls. **Gradient Map** currently retains four embedded color stops (arbitrary embedded stops remain future work).

Dedicated **Gradient Generator** and **Curve Generator** input nodes generate spatial ramps without source textures. Both support horizontal, vertical, radial, and angular patterns using a `Direction` setting (`0`–`3`). They may reference Unreal Curve Linear Color / Curve Float assets, respectively; with no asset, they produce a simple black-to-white ramp.

Unreal curve data is still uniformly sampled into **eight samples** for GPU evaluation. Curves with sharp keys or complex tangents are approximations, not exact interpolations. This package changes experimental node data structures without migrations, because Prism is not released yet.

Run `Automation RunTests Prism` (40 tests). Verify generator behavior without inputs and adjustment behavior with inputs. Neither test registration nor manual operation establishes pixel-perfect GPU accuracy.

## Alpha 11: clear option dropdowns and procedural noise

- **Layer Blend Modes → Mode**: named choices Normal, Multiply, Screen, Add, Subtract, Overlay, Minimum, Maximum.
- **Mask Outline → Mode**: Outer, Inner, Centered.
- **Curve Generator and Gradient Generator → Direction**: Horizontal, Vertical, Radial, Angular.
- These categorical options are native reflected enums with dropdowns. Radius, frequency, and other actual quantities remain numeric.
- **Prism Fractal Noise** (Input): seamless tileable Value, fBm, Ridged, and Billow variants. Controls: Noise Type dropdown, Frequency, Octaves, Persistence, Seed. One node covers four related generators.
- All prior nodes and existing Prism library actions remain in place.

### Development limitations

- Shader noise is deterministic but not yet validated by GPU pixel-readback automation. Frequency * 2^octaves can become expensive or alias at high frequencies; use moderate settings for texture resolutions.
- The original bounded Distance and local Region Variation effects are still approximations. The exact CPU reference algorithms do not yet feed TextureGraph's GPU evaluation.
- Prism is pre-release; node schema changes do not include migration for old experimental graphs.


## Development batch: Pattern Geometry & Modulated Sampling (Alpha 14)

The Pattern Generator now uses a true staggered honeycomb grid for Hexagon, and handles brick, checkerboard, and weave parity across UV seams. Pattern Sampler adds optional ScaleMap, RotationMap, and OffsetMap inputs with per-map influence sliders. See [Pattern Geometry & Sampling](Docs/PATTERN_GEOMETRY_AND_SAMPLING.md) for mode-by-mode behavior, remaining limitations, and validation.

This build remains **experimental**. Test registration and reference geometry are automated, but actual GPU pixel output must still be checked in the Unreal Editor.


## Function assets

Use **Content Browser → Add → Texture → Prism → Prism Function** to create a reusable graph registration directly, or select a TextureGraph and use **Tools → Prism Library → Register Selected**. Each Prism Function points to exactly one TextureGraph; the library browser displays all registered functions.
