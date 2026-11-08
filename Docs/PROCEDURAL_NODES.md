# Prism Procedural Nodes — Alpha 5

| Node | Implementation | Limitations |
|---|---|---|
| Multi-Pass Blur | TextureGraph Gaussian processing | Previously validated by manual test |
| Distance (Bounded) | GPU brute-force Euclidean nearest opposite-class pixel | Maximum 24 px radius, signed remapped 0..1, image edges clamp |
| Prism Curve | Embedded Unreal Runtime Float Curve remaps input luminance | Eight samples approximated on GPU; requires input |
| Gradient Map | GPU 4-color linear gradient | Fixed positions at 0, 1/3, 2/3, 1 |
| Region Variation (Local) | GPU local neighborhood minimum position hash | **Not true Flood Fill**; large or complex connected regions may have inconsistent labels |
| Sharpen | GPU 4-neighbor detail boost | Basic kernel, not high-quality unsharp mask |
| High Pass | GPU 4-neighbor residual | Basic kernel |
| Posterize | GPU RGB quantization | Fixed uniform channel levels |

## Important

**Full connected-component Flood Fill is NOT implemented in this alpha.** The region-variation node is an explicitly labeled limited approximation. Accurate Flood Fill will require a connected-component propagation algorithm, label encoding, convergence/iteration management, and a reliable per-island stable ID. Do not use local region variation for production asset labels.

The Distance node is an exact nearest-opposite-class search **within its bounded radius**. For distances beyond that radius, results saturate.

This is uncompiled source; run all tests and verify outputs manually in UE 5.8.x.

## Alpha 7 — processing and compositing batch

The following eleven native TextureGraph expressions have been added. All use the PrismRender shader pipeline, so they are scheduled as normal TextureGraph GPU jobs.

| Node | Inputs and significant settings | What the node does | Caveats |
|---|---|---|---|
| Prism Soft Threshold | Input; Threshold, Width | Smoothstep threshold on luminance | Width is normalized intensity range, not pixel radius |
| Prism Levels Remap | Input; Input Black/White, Gamma, Output Black/White | Input range mapping, gamma, output range mapping | Operates on each RGB channel independently; not a histogram-based auto-levels filter |
| Prism Color Clamp Range | Input; Minimum/Maximum colors | Clamps RGB separately for each channel | Alpha preserved; minimum above maximum is not a supported artistic control |
| Prism Invert Channels | Input; invert R/G/B/A toggles | Selectively inverts individual channels | Standard TextureGraph Invert is still available |
| Prism Sobel Edge Outline | Input; Radius, Strength | Sobel magnitude from grayscale | Not full morphological edge tracing |
| Prism Sobel Normal From Height | Input; Radius, Strength | Height-to-tangent-normal with a 3×3 Sobel kernel | Tangent-space handedness should be checked against your target material/normal-map convention |
| Prism Directional Emboss | Input; Radius, Strength, Angle | Shaded directional height slope | Produces grayscale lighting, not an edited height map |
| Prism Bevel Lighting Preview | Input; Radius, Strength, Angle | Edge-localized height shading preview | **Not** an SDF bevel or geometric height bevel |
| Prism Local Contrast | Input; Radius, Strength | 3×3 local detail contrast | Approximate local enhancement, not multi-scale clarity |
| Prism Layer Blend Modes | Base, Layer; Mode (0–7), Opacity | Normal, Multiply, Screen, Add, Subtract, Overlay, Min, Max | Enum-like integer selector; Layer alpha modulates opacity, output alpha uses source-over coverage |
| Prism Gamma Adjustment | Input; Gamma | Per-channel RGB gamma mapping | Operates on numerical values; color-space management remains the graph author's responsibility |

### Blend mode selector

`0` Normal, `1` Multiply, `2` Screen, `3` Add, `4` Subtract, `5` Overlay, `6` Minimum, `7` Maximum. Each mode starts from the Base input and blends toward the mode result according to `Opacity × Layer Alpha`.

### Alpha 7 regression checks

1. Run `Automation RunTests Prism` and verify **24 tests** pass (13 prior + 11 new registration/category tests). The new tests validate registration and expression signatures; they do **not** compare rendered image pixels.
2. In a TextureGraph, verify all eleven new `Prism` expressions appear in the Palette.
3. With a grayscale gradient, confirm Soft Threshold narrows/widens at different Widths and Levels Remap changes black/white points and gamma.
4. With a colored RGBA source, test Color Clamp Range and individual Invert Channels toggles, including alpha.
5. With a height image containing simple squares or circles, verify Sobel Edge Outline, Sobel Normal, Directional Emboss, and Bevel Lighting Preview; inspect parameter response and normal orientation.
6. With a detailed source, verify Local Contrast strength 0 leaves RGB unchanged and a positive strength enhances local detail.
7. With two distinct textures, verify all eight Layer Blend Modes, opacity 0 (base unchanged) and 1 (full layer-alpha weighting).
8. Verify Gamma Adjustment at 1.0 leaves RGB unchanged; at 0.5 and 2.0 it changes non-endpoint values.
9. Save and reopen a TextureGraph containing several new nodes. Confirm pins, parameters and results survive reload.
10. Test rectangular textures and 256/1024/2048 output resolutions. Verify texture-size changes do not create missing-texture warnings or editor crashes.

**Validation status:** source and package consistency checks passed in the authoring environment; C++ compilation, HLSL compilation, GPU output and rendering correctness require validation in UE 5.8.3. Older Alpha 6 was confirmed to compile and its 13 tests passed in the user's environment.

---

## Alpha 8: Expanded node batch

Alpha 8 **adds 11 expressions** without modifying or replacing any Alpha 7 classes. All eleven use the established PrismRender image-processing job path.

| Node | Purpose | Important limits |
|---|---|---|
| Prism Morphology | Grayscale erosion or dilation; square or circular footprint | Radius 1–12 pixels, O(radius²) sampling |
| Prism Mask Outline | Outer, inner, or centered outline extracted from a grayscale mask | Radius 1–12 pixels; uses a circular neighborhood |
| Prism Normal Blend | Reoriented tangent-space normal mapping (RNM) | Encoded tangent-space RGB; verify Y convention for your materials |
| Prism Normal Normalize | Normalize RGB-encoded tangent normal | Neutral maps should use (0.5,0.5,1) |
| Prism Normal Invert | Invert X and/or Y orientation | Normal is renormalized |
| Prism Dynamic Gradient | Look up gray intensity along an input gradient texture | Gradient runs left to right, row is selectable |
| Prism Directional Warp | Warp an input along an angle, displacement from second image | Distance in pixel units; texture sampler clamps UVs |
| Prism Height Blend | Height-difference-dependent blending of two textures | Uses each texture's RGB luminance as height, not independent height channels |
| Prism Tile Seam Preview | Repeat a texture and draw magenta seam guides | Diagnostic preview; not a seam-removal algorithm |
| Prism Curve Asset | Adjustment: map source luminance through Curve Float asset | Requires texture input; eight-sample GPU approximation |
| Prism Gradient Asset | Adjustment: map source luminance through Curve Linear Color asset | Requires texture input; eight-sample GPU approximation |

**Curve/Gradient workflow:** Create a `Curve Float` or `Curve Linear Color` asset in the Content Browser, edit it in Unreal's curve editor, and assign it to the corresponding Prism node. Both curve assets are sampled over the normalized 0–1 domain. The original four-knot Prism nodes remain untouched for existing graphs.

### Reference algorithm groundwork (not yet production TextureGraph evaluation)

`FPrismImageAlgorithms` provides two deterministic full-image CPU reference operations:

- `LabelRegions`: exact four-/eight-connected component IDs using a row-major flood traversal.
- `SignedEuclideanDistance`: exact per-pixel-center Euclidean signed distance transform with separable one-dimensional lower envelopes. The sign is positive inside. All-uniform masks use image-diagonal capping.

**These two reference functions are not yet connected to TextureGraph's GPU image readback/upload pipeline.** Consequently, the Alpha 8 shipping GPU `Prism Distance (Bounded)` node still searches within 24 pixels, and the shipping `Prism Flood Fill` remains a local-hash approximation, not true global region labels. They are not represented as complete full-distance/flood-fill nodes.

### Testing Alpha 8

Run `Automation RunTests Prism`: expected **37 tests**, comprising 24 previous tests, 11 registration/signature tests and 2 CPU algorithm correctness tests. These are not GPU pixel-output tests. Manual checks:

1. Morphology: feed a thin white line; dilation expands, erosion shrinks; check square versus round footprint.
2. Outline: use a white circle; inspect inner/outer/center modes, radius 1 and 12.
3. Normal: test blue-neutral normal (0.5, 0.5, 1); flipping either axis should alter encoded X/Y but not Z; blend distinct detail normals and check shading.
4. Dynamic Gradient: use a black-to-white source and a colored horizontal gradient image; swapping the gradient should change the output.
5. Directional Warp: feed a striped image and smooth noise, test 0/90° and positive/negative distances.
6. Height Blend: test zero/one opacity, bias, and transition width. Remember this is a two-color-input utility, not a material-set mixer.
7. Tile Seam Preview: show 2–4 repeats; guides should appear at repeated tile boundaries.
8. CurveAsset/GradientAsset: create editable curve assets, assign them, edit keys and verify outputs update; check both smooth and sharp ramps for approximation error.
9. Save/reopen graphs and verify old Alpha 7 nodes, curve asset links, and all new node settings persist.
10. Test non-square images and high resolutions. Morphology and Outline have a bounded but comparatively expensive O(radius²) fragment sampling cost.

**Next infrastructure milestone:** add a safe tiled-blob readback/upload bridge, schedule exact connected-region and EDT results as TextureGraph jobs, establish GPU pixel-output fixture comparisons, and then build per-region outputs (random gray/color, position, gradient) using stable region IDs. Implement this before claiming production parity for these two families.

## Dedicated spatial generators (Alpha 10)

| Node | Inputs | Purpose |
|---|---|---|
| Prism Gradient Generator | Optional Curve Linear Color asset | Generate color ramp in horizontal / vertical / radial / angular UV patterns |
| Prism Curve Generator | Optional Curve Float asset | Generate grayscale ramp in horizontal / vertical / radial / angular UV patterns |

`Direction`: 0 = horizontal (left to right); 1 = vertical (bottom to top); 2 = radial (center outward); 3 = angular (polar angle). The nodes do not accept a source texture. Adjustments do not generate textures without a source. Existing four-stop Gradient Map is still limited to four stops. Both asset-backed adjustments and generators use eight sampled colors/values. The resulting pixel fidelity at sharp transitions is not yet exact.
