# Analysis of the Delay Lama binary.
This document outlines my findings from active research and reverse engineering of the legacy Delay Lama VST plugin using Ghidra.

## History & Technical Context

### Release and Origins
**Delay Lama** was released on **May 23, 2002**, by **AudioNerdz**. Developed by students at the Utrecht School of the Arts (HKU), it began as a vocal model in **DirectCSound** before being ported to C++. It was a pioneer in integrating real-time 3D animation with vocal synthesis (FOF synthesis) in a VSTi format.

### The 2002 Development Stack
The plugin’s architecture reflects the technical constraints and standards of the early 2000s.

#### Period-Accurate Tools (Available in 2002)
* **VST SDK 2.2 / 2.3:** The industry standard; enabled the then-novel VSTi (instrument) support.
* **VSTGUI 2.x:** A basic drawing library by Steinberg. The GUI code has since been confirmed to be VSTGUI 2.x: functions such as `CFrame::removeView`/`setDropActive`, `CSlider::draw`/`mouse`, `CSplashScreen::mouse`, `CMovieBitmap::draw`, `CBitmap::drawTransparent`, `UDropTarget::Drop` and the out-of-line `CRect` copy constructor recompile to byte-identical code when written in VSTGUI's form. [Web Archive showing versions at time of development](https://web.archive.org/web/20020105044706/http://ygrabit.steinberg.de/users/ygrabit/public_html/index.html)
* **Microsoft Visual C++ 6.0:** The primary IDE for Windows VST development at the time.
* **C++98 Standard:** Relies on raw pointers and manual memory management; lacks modern features like smart pointers or lambdas.

#### Modern Tools (Not available around the development time of delay lama)
* **JUCE (2004):** The current industry-standard framework did not exist, requiring the team to build the Monk's animation logic and XY-pad handling from scratch.
* **VST 3.0 (2008):** The plugin predates modern VST standards and 64-bit architecture.
* **Modern C++ (C++11/14/17/20):** Features like `std::thread` or `auto` were years away.
* **CMake / Git:** Development likely used monolithic Visual Studio project files and older version control like CVS or SVN.

## Classes Hierarchy
This section details the class hierarchy of the identified classes. [class-analysis.md](./class-analysis.md) describes each class, the VST SDK / VSTGUI class it corresponds to, and how certain that is. [function_map.json](./function_map.json) lists the address of every function in the original DLL (generated from the source annotations with `tools/generate_function_map.py`).

Names are this project's; the VST SDK / VSTGUI equivalent is in brackets:
```json
{
    "AudioBase (AudioEffect)": {
        "AudioBaseExtended (AudioEffectX)": {
            "DelayLamaAudio (the synthesizer)": {
                "DelayLamaPlugin": {}
            }
        }
    },
    "EditorInterface (AEffEditor)": {
        "EditorBase (AEffGUIEditor)": {
            "DelayLamaEditor (+ ControlListener)": {}
        }
    },
    "View (CView)": {
        "Window (CFrame)": {},
        "Control (CControl)": {
            "HorizontalSlider (CHorizontalSlider)": {
                "TwoAxisSlider (the XY pad)": {}
            },
            "VerticalSlider (CVerticalSlider)": {},
            "RotaryControl (CKnob)": {
                "Knob (CAnimKnob)": {}
            },
            "TileGrid (CMovieBitmap)": {
                "Monk": {}
            },
            "SplashScreen (CSplashScreen)": {}
        }
    },
    "GDIDrawingContext (CDrawContext)": {
        "OffscreenGDIDrawingContext (COffscreenContext)": {}
    },
    "Bitmap (CBitmap)": {},
    "Rect / Point (CRect / CPoint)": {},
    "DropTarget (UDropTarget)": {}
}
```

## DSP Architecture & Algorithm Origins

### FOF Synthesis (CHANT)
The core vocal synthesis algorithm powering Delay Lama is heavily indebted to the research of **Xavier Rodet** at **IRCAM** (Institut de Recherche et Coordination Acoustique/Musique). The manual explicitly credits Rodet for the "original algorithm," which is based on **FOF (Fonction d'Onde Formantique)** synthesis. 

Developed in the late 1970s and 1980s as part of IRCAM's **CHANT** program, FOF synthesis generates vocal sounds by simulating the excitation of the vocal tract. Instead of basic subtractive synthesis, FOF uses streams of grain-like sine-wave envelopes to model formants (the resonant frequencies of the human vocal tract). 
* **Vowel Control:** The Y-axis of the plugin shifts the relative peaks of these formants to morph between distinct vowel sounds.
* **Formant/Gender Shifting:** The "Voice" knob utilizes spectral envelope manipulation (similar to Rodet's later work on the TRAX Transformer) to shift the formant structure independent of the fundamental pitch, changing the perceived physical size/gender of the vocal model (from baritone to soprano).

*(Note: AudioEase's Arjen van der Schoot and Peter Bakker are also credited. Given AudioEase's expertise in early plugin development and convolution/DSP, they likely provided technical consultation, DSP optimization, or assistance with platform porting).*

## UI Parameter & Technical Mapping
### 1. The XY-Controller (Tibetan Flag)
* **Function:** Controls the primary vocal articulation.
    * **X-Axis:** Controls the fundamental pitch.
    * **Y-Axis:** Controls the vowel sound (formant morphing).
* **Class Implementation:** Mapped to the `TwoAxisSlider`. It reports several things through one value: -2..3 is the pitch (x axis, parameter 11 `PadPitchParameterId`, one octave from MIDI note 36), 98..103 the vowel (y axis, parameter 10 `PadVowelParameterId`), and 200 / 201 singing off / on. The two small handles next to the pad only show its position; they ignore the mouse. 
* **Resources:** Uses **Resource 142** (Singing X handle, 10x10) and **Resource 141** (Singing Y handle, 10x10)—referred to in the manual as the indicator "triangles" ('dingetjes').

### 2. The 'Glide' Knob
* **Function:** Sets the portamento-time (glide between notes). The manual notes this is only active when triggered via a MIDI keyboard: the engine only glides while notes overlap (legato, `isLegato`); otherwise it jumps to the new note.
* **Class Implementation:** Mapped to the left `Knob` (parameter 0, `GlideKnobParameterId`).
* **Resource:** **Resource 152** (Left 'Glide' knob sprite sheet). Renders a 270-degree range of motion using 60 frames.

### 3. The Delay Fader
* **Function:** Controls the wet/dry mix of the built-in delay effect. Panning right introduces the echoing environment, while panning left outputs a completely dry signal.
* **Class Implementation:** Mapped to a `HorizontalSlider` (parameter 2, `DelaySliderParameterId`). The delay itself is a stereo echo with taps at about 310 ms (left) and 398 ms (right) and a feedback of 0.5.
* **Resource:** **Resource 151** (Reverb 'Delay' handle, 20x17). 

### 4. The 'Voice' Knob
* **Function:** Controls the formant shift / vocal character. Center is default; left shifts the formants down (baritone/larger vocal tract); right shifts the formants up (soprano/smaller vocal tract). In the code this is `headSize`: all three formant frequencies are multiplied by `0.75 + 0.5 * headSize`, i.e. from 0.75x to 1.25x.
* **Class Implementation:** Mapped to the right `Knob` (parameter 3, `VoiceKnobParameterId`).
* **Resource:** **Resource 153** (Right 'Voice' knob sprite sheet). Identical mechanical implementation to the Glide knob (60 frames, 270-degree rotation).

### 5. The '?' Button (Help Screen)
* **Function:** Triggers the quick-help and credits window.
* **Class Implementation:** Opens to the `SplashScreen` class.
* **Resource:** **Resource 160** (Splash/Help screen overlay, 253x275). Contains the hardcoded credits and brief instructions.


## The Synthesis Engine (as implemented)
These are the details of `DelayLamaAudio`, verified against the original binary (the unit tests compare the recreation's output with the original's, sample by sample).

* **Grains:** Every pitch period, a 20 ms grain (`grainBuffer`) is synthesized and overlap-added into a ring buffer (`voiceBuffer`). The period comes from a pitch-to-frequency table with 384 steps per octave, starting at MIDI note 0 (8.1758 Hz).
* **Formants:** A grain is the sum of three formants. Each one reads a sine wavetable (`formantWave`) at its formant frequency, and is shaped by an exponential decay table (`decayTable`) read at its own rate (steps of 32.5, 47.5 and 62.5 per sample). Two fixed high formants at about 4950 Hz and 3800 Hz (`fixedFormantBuffer`) are added at half level, and the grain is multiplied by a window (`grainWindow`): a 1.8 ms cosine fade-in, then 1.0, and from 13 ms to the end a cosine curve (see the quirks below).
* **Vowels:** The formant frequencies come from three curves (`formantTable1..3`, 1280 positions each), each a spline through 5 control points: F1 280, 450, 800, 350, 270 Hz; F2 600, 800, 1150, 2000, 2140 Hz; F3 2240, 2830, 2900, 2800, 2950 Hz. These are roughly the vowels u, o, a, e, i along the vowel axis.
* **Vibrato:** A sine LFO (controlled by the mod wheel) whose rate is re-randomized between about 4 and 6 Hz every 104 ms.
* **Smoothing:** Vowel and pad-pitch changes are smoothed over 100 ms in 10 ms steps.
* **Glide:** Between overlapping notes the pitch moves at 12 semitones per (glide time + 0.01) seconds.

### MIDI Implementation
| MIDI | Effect |
| :--- | :--- |
| Note on/off (notes 4 to 72) | Sings the note; the last held note wins |
| Pitch bend | Vowel (14-bit) |
| Controller 1 (mod wheel) | Vibrato depth |
| Controller 5 | Glide (portamento) time |
| Controller 7 | Volume |
| Controller 11 | Pitch from the XY pad (the plugin sends this itself when the pad is used) |
| Controller 12 | Delay amount |
| Controller 13 | Voice (head size) |

When the XY pad is used, the plugin sends MIDI to the host so the moves can be recorded: note 40 on/off for singing, pitch bend for the vowel and controller 11 for the pitch. Pitch bends arriving at the very start of a buffer are spread evenly across it.

### Quirks of the Original
* The random generator's state is never initialized, so the vibrato's random rate depends on what the heap held (normally zero).
* The grain window's release (from 13 ms) computes its cosine from the absolute sample position (`n * pi / 7 ms`) instead of the position since 13 ms, so it is not a clean fade-out. The recreation does the same.
* A few fields are written but never read; they are marked in `DelayLamaAudio.h`.

## Resources
This section lists and describes all resources (bitmaps) bundled within the DLL.

### Precomposed background (360x510)
Resource ID: 130
Offset: 1000e1b0
A fully pre-rendered composite image used as the main UI background for the plugin. It contains the complete static interface layout, including visual elements such as the background behind the sliders, and placeholders for the both monk and the two knobs.

### Monk sprite sheet (1570x1866)
Resource ID: 131
Offset: 1003b310
This is a 5x6 grid containing tiles of 314x311 pixels. Each tile contains a slightly different pose of the monk. 

### Singing Y handle (10x10)
Resource ID: 141
Offset: 103079a8
A handle for dragging vertically.

### Singing X handle (10x10)
Resource ID: 142
Offset: 10307b18
A handle for dragging horizontally.

### Reverb 'Delay' handle (20x17)
Resource ID: 151
Offset: 10307c88
A handle for dragging horizontally. A slightly bigger version of the `Singing X handle` image (resource id 142).

### Left 'Glide' knob sprite sheet (50x3000)
Resource ID: 152
Offset: 103080b0
This is a 1x60 grid containing tiles of 50x50 pixels. Each tile is rotated by ~4.5 degrees in relation to the previous one. The total range of motion is 270 degrees

### Right 'Voice' knob sprite sheet (50x3000)
Resource ID: 153
Offset: 1032e640
This is a 1x60 grid containing tiles of 50x50 pixels. Each tile is rotated by ~4.5 degrees in relation to the previous one. The total range of motion is 270 degrees

### Splash/Help screen overlay (253x275)
Resource ID: 160
Offset: 10354bd0
This is basically the "About" screen/popup you have in normal desktop application, it credits the creators and has some instructions.
#### Hardcoded Text Content:
```md
AudioNerdz
www.audionerdz.com
Interface & 3D Design: Frank Post
Development & Audlo Design: Aram Verwoest, Daan Hermans & Steven Kruyswijk
*VST plugin technology by Steinberg*

[ please donate at www.savetibet.org ]
Use pitchbender to control vowel
Full MIDI implementation in manual

Click & drag for pitch and vowel

Glide - Delay - Voice

close window - v1.1
```