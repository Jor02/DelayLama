<p align="center">
  <img src="assets/reconstructed/Delay%20Lama%20Logo%20Outline.svg" alt="Delay Lama Logo" width="700">
</p>

# Delay Lama Recreation
[![Unit Tests](https://github.com/Jor02/DelayLama/actions/workflows/tests.yml/badge.svg)](https://github.com/Jor02/DelayLama/actions/workflows/tests.yml) ![GitHub contributors](https://img.shields.io/github/contributors/Jor02/DelayLama) ![GitHub commit activity](https://img.shields.io/github/commit-activity/m/Jor02/DelayLama) ![GitHub last commit](https://img.shields.io/github/last-commit/Jor02/DelayLama)

<p align="center">
  <a href="https://jor02.github.io/DelayLama/progress.html">
    <img height="380" src="https://jor02.github.io/DelayLama/progress.svg" alt="reccmp progress">
  </a>
</p>

---

This is an open source recreation of Delay Lama. A plugin that was originally released in 2002 by AudioNerdz. 

I recently learned that DAWs (like Cubase and Ableton) are phasing out support for 32-bit plugins (when my friend tried using this plugin in Cubase), and since the original Delay Lama only has a 32-bit build available, this plugin would no longer be supported either. So I decided to try to reverse engineer it to hopefully be able to make a 64-bit version of the original plugin. With this project I am aiming to update one of my favorite audio synthesizers to support modern DAWs by using "clean-room" reverse engineering (basically meaning, I am not using any public or leaked code for my code) to be able to create a modern 64-bit version of it.

## Alternative Project: MonkSynth

I just found out that [JonET](https://github.com/JonET) coincidentally launched another open-source recreation called [MonkSynth](https://github.com/JonET/monksynth) exactly 4 days before I created this repository.

Their version is more complete and functional than my current project. If you need an operational 64-bit plugin for your DAW right now, use MonkSynth. This repository will still continue to focus on clean-room reverse engineering and binary accuracy. Credit to JonET for also preserving the plugin.

## Goal
The goal is a 1:1 functional recreation of the original plugin, that has all the same features, looks and feels the same, and hopefully also sounds exactly the same.

## My motivation

I've loved this plugin ever since I first found out about it in some random YouTube video, but unfortunately Delay Lama is slowly getting less and less supported by existing DAWs. So because of that, and because I've been wanting to get into reverse engineering for ages now, I decided I would try creating a faithful open-source recreation of my favorite audio synthesizer.

## Current Limitations
Most member names in the DelayLamaAudio class have been checked against what the code does and the original manual (for example: the XY pad's y axis and MIDI pitch bend control the vowel, the x axis the pitch, and the mod wheel the vibrato). A few fields that the original writes but never reads are marked as such in `DelayLamaAudio.h`.

## Development Roadmap
- [x] Fully annotate all functions in the original binary using Ghidra.
- [x] Turn the Ghidra findings and functions into actual C++ code.
- [x] Get a fully working 32-bit build. (Its audio matches the original to within float rounding, checked by the unit tests, and its interface is pixel-identical.)
- [x] Clean up source code to improve the maintainability and readability of the codebase, without changing the functionality. (Names checked against the code and the manual, decompiler leftovers removed, VSTGUI / VST SDK names used; verified by an unchanged VC6 match and the unit tests.)
- [x] Hopefully get a 64-bit build of Delay Lama working. (Builds and matches the original under Wine; testing in real DAWs welcome.)
- [ ] And lastly, if at all possible, get the project to compile to a fully byte accurate binary that 100% matches the original dll. (I've already added [Reccmp](https://github.com/isledecomp/reccmp) to help showing the current progress)

I think it'd also be fun to make a very accurate 3D model of the Monk himself and his environment as a Blend file, but I haven't yet decided if I actually wanna do that.

## DamSDK

Because I don't want this project to use any proprietary or deprecated frameworks, it is built hand in hand with [DamSDK](https://github.com/Jor02/DamSDK).

DamSDK is a custom, VST-compatible plugin interface I am developing specifically for this project. It gives the plugin the interface with existing plugin hosts and basically contains the interface parts and some gui parts of the original Delay Lama.

## Repository Structure

```
DelayLama/
├── src/                    # Source code for the recreation
│   ├── core/               # The synthesizer (DelayLamaAudio), plugin entry point, presets
│   ├── gui/                # The editor and Delay Lama's own controls (Monk, SplashScreen, ...)
│   └── damsdk/             # DamSDK submodule: VST-compatible plugin interface and VSTGUI-style GUI
├── tests/                  # Unit tests (GoogleTest) and fixtures recorded from the original plugin
├── tools/                  # Reference audio renderer and the function map generator
├── docs/                   # Reverse engineering analysis and documentation
│   ├── analysis.md         # History, class hierarchy, synthesis engine, MIDI, resources
│   ├── class-analysis.md   # Every class, its VST SDK / VSTGUI equivalent and confidence
│   └── function_map.json   # Address of every function in the original DLL, by class
├── original/               # Original plugin files for reference
│   ├── decomp/             # Contains unprocessed headers directly exported from Ghidra. 
│   └── docs/               # Original documentation, manual, screenshots from AudioNerdz
└── build/                  # Build output directory (created during compilation)
```

### Prerequisites
- CMake 3.25 or newer (for the presets)
- Visual Studio 2022 or 2026 (for the 32 and 64-bit builds), or Visual C++ 6.0 for the build that is compared with the original binary
- Python 3 with `pefile` (to extract the interface bitmaps)
- A copy of the original `Delay Lama.dll` (for the bitmaps)

### Build Instructions
The interface bitmaps are not in the repository; extract them from your copy of the original plugin first:
```sh
python extract_assets.py "Delay Lama.dll" --output assets/interface
```

32-bit (like the original):
```sh
cmake --preset vs2022
cmake --build --preset vs2022 --config Release
```

64-bit (for modern DAWs):
```sh
cmake --preset vs2022-x64
cmake --build --preset vs2022-x64 --config Release
```
The plugin is written to `build/bin/vs2022/<x32|x64>/Release/DelayLama.dll`. The 64-bit build produces the same audio as the original (checked by the unit tests) and draws the same interface.

### Tests
The unit tests check the recreation against data recorded from the original plugin: the synthesizer's state after initialization, and its audio output for a scripted performance (notes, glide, vibrato, pitch bend, parameter changes), sample by sample.
```sh
cmake --preset vs2022
cmake --build --preset vs2022 --target DelayLamaTests
ctest --test-dir build/vs2022 --output-on-failure
```
CI runs them for both 32 and 64-bit. `tools/render_reference.cpp` re-records the reference audio from the original DLL if the scenario changes.

### Matching the Original Binary
The `vc6` preset builds with Visual C++ 6.0 (the compiler the original was built with), and [reccmp](https://github.com/isledecomp/reccmp) compares every function with the original (`GetProgress.bat`). The results are published on the [progress page](https://jor02.github.io/DelayLama/progress.html).

## Documentation

- [Project Analysis](docs/analysis.md)
- [Class Structure Analysis](docs/class-analysis.md)
- [Original User Manual](original/docs/manual.md)

## Licensing
Unless otherwise noted, the source code in this repository is licensed under the MIT License.

The recreated assets contained in assets/reconstructed/ are licensed separately under the Creative Commons Attribution 4.0 International (CC BY 4.0) license. See assets/reconstructed/README.md and the accompanying LICENSE file in that directory for details.

## Legal Notes

This is a reverse engineering project for educational and historical preservation purposes.
Delay Lama is a product of [AudioNerdz](http://www.audionerdz.nl/). This project is not affiliated with or endorsed by AudioNerdz.

The original VST 2.4 SDK is no longer publicly distributed by Steinberg. This project utilizes my [DamSDK](https://github.com/Jor02/DamSDK) alternative to handle host communication, ensuring the project remains open-source and distributable without infringing on restricted SDK licenses.
