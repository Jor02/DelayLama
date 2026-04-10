# Analysis of the Delay Lama binary.
This document outlines my findings from active research and reverse engineering of the legacy Delay Lama VST plugin using Ghidra.

## History & Technical Context

### Release and Origins
**Delay Lama** was released on **May 23, 2002**, by **AudioNerdz**. Developed by students at the Utrecht School of the Arts (HKU), it began as a vocal model in **DirectCSound** before being ported to C++. It was a pioneer in integrating real-time 3D animation with vocal synthesis (FOF synthesis) in a VSTi format.

### The 2002 Development Stack
The plugin’s architecture reflects the technical constraints and standards of the early 2000s.

#### Period-Accurate Tools (Available in 2002)
* **VST SDK 2.2 / 2.3:** The industry standard; enabled the then-novel VSTi (instrument) support.
* **VSTGUI 2.0 / 2.1:** A basic drawing library by Steinberg. I've based the (`AEffGUIEditor`, `CBitmap`) on this library’s naming conventions, but I haven't yet actually verified if these these classes are actually correct. [Web Archive showing versions at time of development](https://web.archive.org/web/20020105044706/http://ygrabit.steinberg.de/users/ygrabit/public_html/index.html)
* **Microsoft Visual C++ 6.0:** The primary IDE for Windows VST development at the time.
* **C++98 Standard:** Relies on raw pointers and manual memory management; lacks modern features like smart pointers or lambdas.

#### Modern Tools (Not available around the development time of delay lama)
* **JUCE (2004):** The current industry-standard framework did not exist, requiring the team to build the Monk's animation logic and XY-pad handling from scratch.
* **VST 3.0 (2008):** The plugin predates modern VST standards and 64-bit architecture.
* **Modern C++ (C++11/14/17/20):** Features like `std::thread` or `auto` were years away.
* **CMake / Git:** Development likely used monolithic Visual Studio project files and older version control like CVS or SVN.

## Classes Hierarchy
This section details the class hierarchy of currently identified classes. If you want more information on the classes themselves there's [class-analysis.md](./class-analysis.md) which shows both the purpose, and the current level of certainty regarding implementation of the classes

The following class hierarchy has been mapped based on inheritance patterns and function calls:
```json
{
    "GDIDrawingContext": {},
    "View": {
        "Frame": {},
        "Control": {
            "HorizontalSlider": {
                "TwoAxisSlider": {}
            },
            "RotaryControl": {
                "Knob": {}
            },
            "VerticalSlider": {},
            "SplashScreen": {},
            "TileGrid": {
                "Monk": {}
            }
        }
    },
    "Bitmap": {},
    "AEffGUIEditor": {
        "DelayLamaEditor": {}
    },
    "DropTarget": {},
    "AudioEffect": {
        "AudioEffectX": {
            "VstPluginHelper": {
                "DelayLama": {}
            }
        }
    }
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
* **Class Implementation:** Mapped to the `TwoAxisSlider`. 
* **Resources:** Uses **Resource 142** (Singing X handle, 10x10) and **Resource 141** (Singing Y handle, 10x10)—referred to in the manual as the indicator "triangles" ('dingetjes').

### 2. The 'Glide' Knob
* **Function:** Sets the portamento-time (glide between notes). The manual notes this is only active when triggered via a MIDI keyboard, meaning the DSP engine bypasses this slew limiter when pitch is controlled directly via the UI's X-axis.
* **Class Implementation:** Mapped to the left `Knob`.
* **Resource:** **Resource 152** (Left 'Glide' knob sprite sheet). Renders a 270-degree range of motion using 60 frames.

### 3. The Delay Fader
* **Function:** Controls the wet/dry mix of the built-in delay effect. Panning right introduces the echoing environment, while panning left outputs a completely dry signal.
* **Class Implementation:** Mapped to a `HorizontalSlider` class.
* **Resource:** **Resource 151** (Reverb 'Delay' handle, 20x17). 

### 4. The 'Voice' Knob
* **Function:** Controls the formant shift / vocal character. Center is default; left shifts the formants down (baritone/larger vocal tract); right shifts the formants up (soprano/smaller vocal tract). 
* **Class Implementation:** Mapped to the right `Knob`.
* **Resource:** **Resource 153** (Right 'Voice' knob sprite sheet). Identical mechanical implementation to the Glide knob (60 frames, 270-degree rotation).

### 5. The '?' Button (Help Screen)
* **Function:** Triggers the quick-help and credits window.
* **Class Implementation:** Opens to the `SplashScreen` class.
* **Resource:** **Resource 160** (Splash/Help screen overlay, 253x275). Contains the hardcoded credits and brief instructions.


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