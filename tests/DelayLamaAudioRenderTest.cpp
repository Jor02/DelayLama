#include <gtest/gtest.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include "core/DelayLamaAudio.h"
#include "damsdk/api/DamPlugin.h"
#include "RenderScenario.h"

// The plugin entry point, as a host would call it.
extern "C" DamSDK::Api::DamPlugin* __cdecl VSTPluginMain(DamSDK::Api::dispatchFunc hostCallback);

// Samples must match to within last-bit float rounding. A few isolated samples may
// differ a little more: the original rounds some 80-bit x87 products to float, and
// with SSE2 (e.g. Visual Studio 2022) the rare tie can round the other way, moving one
// wavetable lookup by one step. A real bug changes hundreds of samples by far more.
static const float kSampleTolerance = 1e-6f;
static const float kOutlierTolerance = 1e-4f;
static const double kMaxOutlierFraction = 0.0005;

// tests/fixtures/ReferenceAudio.f32 is the output of the original Delay Lama for the
// scenario in tests/helpers/RenderScenario.h (rendered with tools/render_reference.cpp).
static std::vector<float> loadReference() {
    std::vector<float> samples;
    FILE* f = std::fopen(TEST_FIXTURES_DIR "/ReferenceAudio.f32", "rb");
    if (!f)
        return samples;
    float buffer[4096];
    size_t n;
    while ((n = std::fread(buffer, sizeof(float), 4096, f)) > 0)
        samples.insert(samples.end(), buffer, buffer + n);
    std::fclose(f);
    return samples;
}

// The original never initialises the random generator's state (only getRandomFloat
// touches it), so its vibrato randomness starts from whatever the heap held. A fresh
// allocation is normally zero, as it was when the reference was rendered; a
// Visual Studio Debug build fills new memory with 0xCD instead.
static void resetRandomState(RenderScenario::Plugin* plugin) {
    DamSDK::Api::AudioBase* base = (DamSDK::Api::AudioBase*)((DamSDK::Api::DamPlugin*)plugin)->object;
    static_cast<DelayLama::Core::DelayLamaAudio*>(base)->rngState = 0;
}

static std::vector<float> renderThisBuild() {
    DamSDK::Api::DamPlugin* plugin = VSTPluginMain((DamSDK::Api::dispatchFunc)&RenderScenario::hostCallback);
    if (!plugin)
        return std::vector<float>();
    std::vector<float> samples = RenderScenario::render((RenderScenario::Plugin*)plugin, &resetRandomState);

    // Set DELAYLAMA_RENDER_OUT to keep this build's output for listening or analysis.
    if (const char* path = std::getenv("DELAYLAMA_RENDER_OUT")) {
        if (FILE* f = std::fopen(path, "wb")) {
            std::fwrite(&samples[0], sizeof(float), samples.size(), f);
            std::fclose(f);
        }
    }
    return samples;
}

TEST(DelayLamaAudioRenderTest, OutputMatchesOriginal) {
    std::vector<float> expected = loadReference();
    ASSERT_FALSE(expected.empty()) << "missing fixture ReferenceAudio.f32";

    std::vector<float> actual = renderThisBuild();
    ASSERT_EQ(actual.size(), expected.size());

    const int channels = 2;
    const int frames = (int)(expected.size() / channels);
    int outliers = 0;
    int failures = 0;
    for (int i = 0; i < frames * channels; i++) {
        ASSERT_FALSE(std::isnan(actual[i])) << "NaN at frame " << i / channels;
        float diff = std::fabs(actual[i] - expected[i]);
        if (diff <= kSampleTolerance)
            continue;
        outliers++;
        if (diff > kOutlierTolerance && ++failures <= 10) {
            ADD_FAILURE() << "frame " << i / channels << " (block " << i / channels / RenderScenario::kBlockSize
                          << ") channel " << i % channels << ": expected " << expected[i] << " but got " << actual[i];
        }
    }
    EXPECT_EQ(failures, 0) << "samples differ from the original by more than " << kOutlierTolerance;
    EXPECT_LE(outliers, (int)(expected.size() * kMaxOutlierFraction))
        << "too many samples differ from the original by more than " << kSampleTolerance;
}
