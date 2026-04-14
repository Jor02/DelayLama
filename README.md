# Delay Lama Recreation

# `⚠️ THIS PROJECT IS STILL IN EARLY DEVELOPMENT ⚠️`

This is an open source recreation of Delay Lama. A plugin that was originally released in 2002 by AudioNerdz. 

I recently learned that DAWs (like Cubase and Ableton) are phasing out support for 32-bit plugins, since the original Delay Lama only has 32-bit build available, I decided to try to reverse engineer it to hopefully be able to make a 64-bit version of the original plugin. With this project i am aiming to preserve one of my favorite audio plugins for modern DAW's by using clean-room reverse engineering to be able to create modern 64-bit version of it.

## Goal
The goal is a 1:1 functional recreation of the original plugin, that has all the same features, looks and feels the same, and hopefully also sounds exactly the same.

## Motivation

I've loved this plugin ever since I first found out about it, but unfortunately delay lama is slowly getting less and less supported by existing DAWs. So because of that, and because I've been wanting to get into reverse engineering for ages now, I decided I would try creating a faithul open-source recreation of my favorite audio synthesizer.

## Development Roadmap
- [x] Fully annotate all functions in the original binary using Ghidra.
- [ ] Turn the ghidra findings and functions into actual C++ code.
- [ ] Get a fully working 32-bit build.
- [ ] Clean up source code to improve the maintainability and readablitiy of the codebase.
- [ ] Hopefully get a 64-bit build of Delay Lama working.
- [ ] And lastly, if at all possible, get the project to compile to a fully byte accurate binary that 100% matches the original dll. (I've already added [Reccmp](https://github.com/isledecomp/reccmp) to help showing the progress.)

I think it'd also be fun to make a very accurate 3D model of the Monk himself and his environment as a Blend file, but I haven't yet decided If I actually wanna do that.

## Built on DamSDK

To achieve independence from proprietary and deprecated frameworks, this project is built atop [DamSDK](https://github.com/Jor02/DamSDK).

DamSDK is a custom, VST-compatible plugin interface developed specifically for this project. It provides the necessary architectural backbone to interface with plugin hosts while maintaining the specific logic requirements of the original Delay Lama.

## Repository Structure

```
DelayLama/
├── src/                    # Source code for the recreation
├── docs/                   # Reverse engineering analysis and documentation
│   ├── analysis.md         # Overall project analysis
│   ├── class-analysis.md   # Class structure analysis
│   └── function_map.json   # Map of all functions in the binary, categorized by class and namespace
├── original/               # Original plugin files for reference
│   ├── decomp/             # Contains unprocessed headers directly exported from Ghidra. 
│   └── docs/               # Original documentation, manual, screenshots fom AudioNerdz
└── build/                  # Build output directory (created during compilation)
```

## Building
This project uses CMake for the build system.

### Prerequisites
- Not yet determined

cmake

### Build Instructions
- Not yet determined

cmake -B build -A Win32
cmake --build build --config Release

## Documentation

- [Project Analysis](docs/analysis.md)
- [Class Structure Analysis](docs/class-analysis.md)
- [Original User Manual](original/docs/manual.md)

## Legal Notes

This is a reverse engineering project for educational and historical preservation purposes.
Delay Lama is a product of [AudioNerdz](http://www.audionerdz.nl/). This project is not affiliated with or endorsed by AudioNerdz.

The original VST 2.4 SDK is no longer publicly distributed by Steinberg. This project utilizes my [DamSDK](https://github.com/Jor02/DamSDK) alternative to handle host communication, ensuring the project remains open-source and distributable without infringing on restricted SDK licenses.