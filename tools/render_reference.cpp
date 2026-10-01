// Renders the audio scenario from tests/helpers/RenderScenario.h with a VST 2.x
// plugin DLL and writes the interleaved stereo float32 samples to a file.
//
// Used to create tests/fixtures/ReferenceAudio.f32 from the original Delay Lama:
//   i686-w64-mingw32-g++ -O2 -static -I tests/helpers tools/render_reference.cpp -o render_reference.exe
//   wine render_reference.exe "Delay Lama.dll" tests/fixtures/ReferenceAudio.f32
#include <windows.h>
#include <cstdio>
#include "RenderScenario.h"

typedef RenderScenario::Plugin* (*PluginMain)(RenderScenario::HostCallback);

int main(int argc, char** argv) {
    if (argc < 3) {
        std::fprintf(stderr, "usage: %s <plugin.dll> <output.f32>\n", argv[0]);
        return 1;
    }
    HMODULE dll = LoadLibraryA(argv[1]);
    if (!dll) {
        std::fprintf(stderr, "cannot load %s\n", argv[1]);
        return 1;
    }
    PluginMain entry = (PluginMain)GetProcAddress(dll, "main");
    if (!entry)
        entry = (PluginMain)GetProcAddress(dll, "VSTPluginMain");
    if (!entry) {
        std::fprintf(stderr, "no plugin entry point\n");
        return 1;
    }
    RenderScenario::Plugin* plugin = entry(&RenderScenario::hostCallback);
    if (!plugin) {
        std::fprintf(stderr, "plugin refused to load\n");
        return 1;
    }

    std::vector<float> samples = RenderScenario::render(plugin);

    FILE* f = std::fopen(argv[2], "wb");
    if (!f)
        return 1;
    std::fwrite(&samples[0], sizeof(float), samples.size(), f);
    std::fclose(f);
    std::printf("wrote %u stereo frames\n", (unsigned)(samples.size() / 2));
    return 0;
}
