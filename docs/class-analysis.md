# Class Analysis
This document describes each class in the recreation, the class of the VST SDK or VSTGUI 2.x it corresponds to, and how certain that is. The addresses of all functions are in [function_map.json](./function_map.json).

## Confidence Legend
| Level | Behavior Confidence | Name Confidence |
| :--- | :--- | :--- |
| **High** | Proven: the recreated code recompiles to (nearly) the same machine code, or the unit tests show the same results as the original. | Matches the VST SDK / VSTGUI class it recompiles like, or strings in the binary. |
| **Medium** | Inferred from structure or context. | Plausible based on typical VST architecture. |
| **Low** | Preliminary guess / speculative. | Placeholder or likely incorrect. |

"Match" is the share of functions that recompile to exactly the original's code with Visual C++ 6.0 (see the [progress page](https://jor02.github.io/DelayLama/progress.html) for per-function numbers).

## Plugin (VST interface)

### AudioBase (VST SDK: `AudioEffect`)
Behavior Confidence: High · Name Confidence: High (equivalent)

The plugin object the host talks to. Owns the `DamPlugin` struct (VST's `AEffect`), forwards the host's dispatcher, parameter and process calls to virtual methods, and wraps the host callback. Its constructor fills `DamPlugin` in the same order as the VST SDK's `AudioEffect` constructor.

### AudioBaseExtended (VST SDK: `AudioEffectX`)
Behavior Confidence: High · Name Confidence: High (equivalent)

Adds the VST 2 opcodes: MIDI events, `canDo` (`pluginSupports`), program names, speaker arrangement and the other `AudioEffectX` features.

### DelayLamaAudio (the synthesizer)
Behavior Confidence: High · Name Confidence: High

The voice synthesizer: grains of three formants plus two fixed formants, overlap-added at the pitch period, with vibrato, glide, a stereo delay and the monk's animation state. See [analysis.md](./analysis.md#the-synthesis-engine-as-implemented). The unit tests check that its tables after `initialize()` and its audio output match the original's.

### DelayLamaPlugin
Behavior Confidence: High · Name Confidence: Medium

The concrete plugin class created by `VSTPluginMain`. It sets the VST unique ID (`'AnDl'`), creates the editor and forwards every parameter change to it.

### Preset
Behavior Confidence: High · Name Confidence: Medium

One of the 5 built-in programs: glide time, delay, head size and a name ("Rabten", "Dorje", ...).

## Editor

### EditorInterface (VST SDK: `AEffEditor`) and EditorBase (VSTGUI: `AEffGUIEditor`)
Behavior Confidence: High · Name Confidence: High (equivalent)

The editor interface the host calls (`open`, `close`, `idle`, `getRect`), and VSTGUI's GUI editor base that owns the frame (`Window`) and redraws it.

### DelayLamaEditor
Behavior Confidence: High · Name Confidence: High

Builds the interface in `open()` the way VSTGUI editors do (one `CRect` reused for each control), forwards control changes to the plugin (`valueChanged`, using `automateHostParameter` so hosts can record automation) and updates the controls when parameters change (`dispatcher`).

## GUI (VSTGUI 2.x)

### View (`CView`) and Control (`CControl`)
Behavior Confidence: High · Name Confidence: High (equivalent)

Base classes of everything drawn. `View` has the rect, dirty flag, mouse-enable flag and reference count (`remember`/`release` = VSTGUI's `remember`/`forget`); `onDrop` is VSTGUI's drop handler. `Control` adds the value, default value, parameter id, background bitmap and the listener.

### Window (`CFrame`)
Behavior Confidence: High · Name Confidence: Medium

The plugin window: creates the Win32 child window, holds the controls, routes mouse, wheel and drop events to the control under the mouse, handles the modal view (the about screen), drag and drop (`setDragAndDropState` = `setDropActive`) and the cursor.

### GDIDrawingContext (`CDrawContext`) and OffscreenGDIDrawingContext (`COffscreenContext`)
Behavior Confidence: High · Name Confidence: Medium

Drawing on a Win32 device context: pens, brushes, colors, mouse state, and an offscreen bitmap that sliders draw into before copying to the screen (`copyToScreen` = `copyFrom`).

### Bitmap (`CBitmap`)
Behavior Confidence: High · Name Confidence: High (equivalent)

A bitmap resource with a reference count. `blit` draws it; `drawMasked` is VSTGUI's `drawTransparent` (white is transparent).

### Rect / Point (`CRect` / `CPoint`)
Behavior Confidence: High · Name Confidence: High (equivalent)

Used where the original passes them by value; the original's `CRect` copy constructor is a real function.

### HorizontalSlider / VerticalSlider (`CHorizontalSlider` / `CVerticalSlider`)
Behavior Confidence: High · Name Confidence: High (equivalent)

Sliders with a handle bitmap over a background; Ctrl+click resets to the default value, Shift drags finely. Used for the delay fader and the two indicator handles next to the XY pad.

### TwoAxisSlider (the XY pad)
Behavior Confidence: High · Name Confidence: Medium

Delay Lama's own control, derived from `HorizontalSlider`. Clicking starts singing; dragging reports the pitch (x) and the vowel (y) through one value (see [analysis.md](./analysis.md#1-the-xy-controller-tibetan-flag)).

### RotaryControl (`CKnob`) and Knob (`CAnimKnob`)
Behavior Confidence: High · Name Confidence: High (equivalent)

The Glide and Voice knobs: a 60-frame sprite sheet chosen by the value.

### TileGrid (`CMovieBitmap`) and Monk
Behavior Confidence: High · Name Confidence: High

`TileGrid` shows one frame of a sprite sheet. `Monk` lays its 30 frames out as a 5 x 6 grid (resource 131) and draws the frame for the current mouth / idle animation.

### SplashScreen (`CSplashScreen`)
Behavior Confidence: High · Name Confidence: High (equivalent)

The '?' button; clicking it shows the about screen (resource 160) as a modal view, clicking again closes it.

### DropTarget (`UDropTarget`)
Behavior Confidence: High · Name Confidence: High (equivalent)

The OLE drop target: dropped files (with `.lnk` shortcuts resolved by `checkResolveLink`) or text are handed to the control under the mouse. Delay Lama's controls ignore drops.
