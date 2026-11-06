# Prism

Prism expands Unreal Engine's TextureGraph with a searchable reusable-graph library and native procedural processing nodes. It does not replace or modify TextureGraph or change existing graph assets.

**Version:** 0.1.0-alpha.3 — Unreal Engine 5.8.x, editor-only, source distribution.

## Install

Extract the ZIP so the plugin lives at `YourProject/Plugins/Prism/Prism.uplugin`. Enable TextureGraph and Prism, restart the editor, and build your project editor target.

## Library

Open **Tools > Prism Library**. Select a TextureGraph asset in the Content Browser and click **Register Selected** to create a `PL_` library metadata asset under `/Game/PrismLibrary`. Save the entry; set its category, subcategory, tags, description and display name in its Details editor. Prism's browser lets you search and open the graph, browse it in the Content Browser, or edit its metadata. Drag the source TextureGraph asset from the Content Browser into another TextureGraph canvas to insert the native subgraph.

Library search supports multiple case-insensitive terms across labels, descriptions, category, subcategory, tags and asset path. Registrations for the same graph are rejected by **Register Selected**; pre-existing duplicate metadata assets can still be created manually and are not destroyed. Prism listens to Asset Registry add/update/delete/rename events for `UPrismLibraryEntry` and batches a deferred refresh instead of polling. **Refresh** remains available for unsaved metadata edits.

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
8. Run `Automation RunTests Prism` and confirm six successes.

## Scope and limitations

- The TextureGraph native Palette does not provide a supported third-party library-entry registration API. Prism uses a separate dockable library browser; Prism's native C++ expressions appear in TextureGraph's native node list.
- Distance Transform, Curve, Gradient Map and Flood Fill are planned and **not implemented** in this alpha. They require correct GPU/CPU image-processing pathways and deterministic rendered-output tests; they will not be simulated as empty nodes.
- No changes are made to Epic TextureGraph source. No Probe dependency is required.
- This source package has not been compiled against a local Unreal Editor installation in the packaging environment. Test in your UE 5.8.3 project before shipping.

See `Docs/ROADMAP.md`.


## Alpha 4 build correction

The PrismEditor module now explicitly links Unreal Engine `RHI` and `RenderCore` because the Multi-Pass Blur expression references sampler-state and render-resource implementations from those modules. This corrects the unresolved external linker symbols reported in Alpha 3. No processing behavior or library UI was changed.

## Alpha 5 procedural node batch

New TextureGraph nodes: Distance (Bounded), Curve (Four-Knot), Gradient Map, Region Variation (Local), Sharpen, High Pass, and Posterize. These are initial implementations with documented restrictions. **The local variation node is not a true Flood Fill.** See `Docs/PROCEDURAL_NODES.md` for usage and limitations. The shader module loads early to register global shaders; the editor UI continues loading separately.
