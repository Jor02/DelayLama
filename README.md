# Delay Lama Recreation

# `⚠️ THIS PROJECT IS STILL IN EARLY DEVELOPMENT ⚠️`

An open-source, faithful recreation of the legendary Delay Lama VST instrument. Originally released in 2002 by AudioNerdz, the "Singing Monk" became a cult classic for its unique vowel synthesis and charming animated interface. 

As modern DAWs phase out support for legacy 32-bit plugins, the original Delay Lama risks becoming "abandonware." This project aims to preserve the plugin for the modern era through clean-room reverse engineering and a 64-bit modernization.

## Goal
The goal is a 1:1 functional recreation of the original plugin, that has all the same features, looks and feels the same, and hopefully also sounds the same.

## Motivation

I've loved it ever since I first found out about it, but unfortunately delay lama is slowly getting less and less supported by existing DAWs, and it only having a 32-bit build doesn't help with that fact. So because of that, and because I've been wanting to get into reverse engineering for ages now, I decided I would try creating a faithul open-source recreation of this beloved extension.

## Development Roadmap
- [x] Fully annotate all functions in the original binary using Ghidra.
- [ ] Turn the ghidra findings and functions into actual C++ code.
- [ ] Get a fully working 32-bit build.
- [ ] Clean up source code to improve the maintainability and readablitiy of the codebase.
- [ ] Hopefully get a 64-bit build of Delay Lama working.
- [ ] And lastly, if at all possible, get the project to compile to a fully byte accurate binary that 100% matches the original dll.

I think it'd also be fun to make a very accurate 3D model of the Monk himself and his environment as a Blend file, but I haven't yet decided If I actually wanna do that.

## Built on DamSDK

To achieve independence from proprietary or deprecated frameworks, this project is built atop [DamSDK](./src/damsdk/).

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