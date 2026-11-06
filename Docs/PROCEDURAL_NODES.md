# Prism Procedural Nodes — Alpha 5

| Node | Implementation | Limitations |
|---|---|---|
| Multi-Pass Blur | TextureGraph Gaussian processing | Previously validated by manual test |
| Distance (Bounded) | GPU brute-force Euclidean nearest opposite-class pixel | Maximum 24 px radius, signed remapped 0..1, image edges clamp |
| Curve (Four-Knot) | GPU smooth interpolation of 4 values at normalized 0, 1/3, 2/3, 1 | Not a freeform spline curve editor |
| Gradient Map | GPU 4-color linear gradient | Fixed positions at 0, 1/3, 2/3, 1 |
| Region Variation (Local) | GPU local neighborhood minimum position hash | **Not true Flood Fill**; large or complex connected regions may have inconsistent labels |
| Sharpen | GPU 4-neighbor detail boost | Basic kernel, not high-quality unsharp mask |
| High Pass | GPU 4-neighbor residual | Basic kernel |
| Posterize | GPU RGB quantization | Fixed uniform channel levels |

## Important

**Full connected-component Flood Fill is NOT implemented in this alpha.** The region-variation node is an explicitly labeled limited approximation. Accurate Flood Fill will require a connected-component propagation algorithm, label encoding, convergence/iteration management, and a reliable per-island stable ID. Do not use local region variation for production asset labels.

The Distance node is an exact nearest-opposite-class search **within its bounded radius**. For distances beyond that radius, results saturate.

This is uncompiled source; run all tests and verify outputs manually in UE 5.8.x.
