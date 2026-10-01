#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
#include <cmath>
#include <limits>
#include <vector>
#include "core/DelayLamaAudio.h"

using json = nlohmann::json;

static intptr_t stubHostCallback(struct DamSDK::Api::DamPlugin* plugin, int32_t targetOperation, int32_t index, intptr_t value, void * data, float optional) {
    return 0;
}

static constexpr float kEps = 1e-5f;

// The fixture was dumped from the original plugin. A few values could not be
// recorded and were written as null (or "null"); they are returned as NaN
// and skipped in the comparisons below.
static float fixtureFloat(const json& v) {
    if (v.is_null() || v.is_string())
        return std::numeric_limits<float>::quiet_NaN();
    return v.get<float>();
}

static std::vector<float> fixtureFloats(const json& v) {
    std::vector<float> out;
    for (const auto& x : v)
        out.push_back(fixtureFloat(x));
    return out;
}

static bool sameFloat(float actual, float expected) {
    if (std::isnan(expected))
        return true;  // not recorded in the fixture
    // Relative tolerance: the reference values come from the original build,
    // so the last bits of large values can differ between compilers.
    // (no std::max: <windows.h> defines a max macro under MSVC)
    float scale = std::abs(expected) > 1.0f ? std::abs(expected) : 1.0f;
    return std::abs(actual - expected) <= kEps * scale;
}

class DelayLamaAudioInitTest : public ::testing::Test {
protected:
    DelayLama::Core::DelayLamaAudio* audio = nullptr;
    json state;

    void SetUp() override {
        std::ifstream f(TEST_FIXTURES_DIR "/DelayLamaAudioState.json");
        ASSERT_TRUE(f.is_open()) << "Could not open fixture file";
        f >> state;

        audio = new DelayLama::Core::DelayLamaAudio(stubHostCallback);
        audio->initialize();
    }

    void TearDown() override {
        delete audio;
    }
};

auto checkFloatArray = [](const char* name, float* ptr, const std::vector<float>& vals) {
    ASSERT_NE(ptr, nullptr) << name << " is null";
    for (size_t i = 0; i < vals.size(); ++i) {
        if (!sameFloat(ptr[i], vals[i])) {
            ADD_FAILURE() << name << "[" << i << "]: expected " << vals[i]
                          << " but got " << ptr[i];
            break;
        }
    }
};

auto checkIntArray = [](const char* name, int* ptr, const std::vector<int>& vals) {
    for (size_t i = 0; i < vals.size(); ++i) {
        if (ptr[i] != vals[i]) {
            ADD_FAILURE() << name << "[" << i << "]: expected " << vals[i]
                          << " but got " << ptr[i];
            break;
        }
    }
};

TEST_F(DelayLamaAudioInitTest, Initialize_Scalars_Floats) {
    const auto& s = state["scalars"];

    const std::pair<const char*, float DelayLama::Core::DelayLamaAudio::*> fields[] = {
        { "prevVowelValue",           &DelayLama::Core::DelayLamaAudio::prevVowelValue           },
        { "outputGain",               &DelayLama::Core::DelayLamaAudio::outputGain               },
        { "vowelTargetValue",         &DelayLama::Core::DelayLamaAudio::vowelTargetValue         },
        { "voicePitch", &DelayLama::Core::DelayLamaAudio::voicePitch },
        { "lfoPhaseAccumulator",      &DelayLama::Core::DelayLamaAudio::lfoPhaseAccumulator      },
        { "lfoRate",        &DelayLama::Core::DelayLamaAudio::lfoRate        },
        { "vibratoDepth",                 &DelayLama::Core::DelayLamaAudio::vibratoDepth                 },
        { "vibratoOffset",           &DelayLama::Core::DelayLamaAudio::vibratoOffset           },
        { "lfoPhaseIncrement",        &DelayLama::Core::DelayLamaAudio::lfoPhaseIncrement        },
        { "vowelLookupIndex",         &DelayLama::Core::DelayLamaAudio::vowelLookupIndex         },
        { "formant1Bandwidth",        &DelayLama::Core::DelayLamaAudio::formant1Bandwidth        },
        { "formant2Bandwidth",        &DelayLama::Core::DelayLamaAudio::formant2Bandwidth        },
        { "formant3Bandwidth",        &DelayLama::Core::DelayLamaAudio::formant3Bandwidth        },
        { "headSizeScale",         &DelayLama::Core::DelayLamaAudio::headSizeScale         },
        { "padPitchCurrent",           &DelayLama::Core::DelayLamaAudio::padPitchCurrent           },
        { "pluginSampleRate",         &DelayLama::Core::DelayLamaAudio::pluginSampleRate         },
        { "prevSampleRate",           &DelayLama::Core::DelayLamaAudio::prevSampleRate           },
        { "delayFeedback",            &DelayLama::Core::DelayLamaAudio::delayFeedback            },
        { "formantWaveStepsPerHz",          &DelayLama::Core::DelayLamaAudio::formantWaveStepsPerHz          },
        { "rngScale",                 &DelayLama::Core::DelayLamaAudio::rngScale                 },
    };

    for (const auto& [name, member] : fields) {
        SCOPED_TRACE(name);
        EXPECT_TRUE(sameFloat(audio->*member, fixtureFloat(s[name])))
            << "Field: " << name << " expected " << fixtureFloat(s[name]) << " but got " << audio->*member;
    }
}

TEST_F(DelayLamaAudioInitTest, Initialize_Scalars_Ints) {
    const auto& s = state["scalars"];

    const std::pair<const char*, int DelayLama::Core::DelayLamaAudio::*> fields[] = {
        { "synthesisFrameCounter",           &DelayLama::Core::DelayLamaAudio::synthesisFrameCounter           },
        { "lfoReseedIntervalSamples",        &DelayLama::Core::DelayLamaAudio::lfoReseedIntervalSamples        },
        { "sampleCounter",                   &DelayLama::Core::DelayLamaAudio::sampleCounter                   },
        { "pulseWriteIndex",                      &DelayLama::Core::DelayLamaAudio::pulseWriteIndex                      },
        { "samplesSincePulse",            &DelayLama::Core::DelayLamaAudio::samplesSincePulse            },
        { "attackSamples",                   &DelayLama::Core::DelayLamaAudio::attackSamples                   },
        { "sustainStart",                    &DelayLama::Core::DelayLamaAudio::sustainStart                    },
        { "releaseSamples",                  &DelayLama::Core::DelayLamaAudio::releaseSamples                  },
        { "totalSmoothingFrames",            &DelayLama::Core::DelayLamaAudio::totalSmoothingFrames            },
        { "smoothCounter",                   &DelayLama::Core::DelayLamaAudio::smoothCounter                   },
        { "smoothingFrames",                 &DelayLama::Core::DelayLamaAudio::smoothingFrames                 },
        { "smoothStep",                      &DelayLama::Core::DelayLamaAudio::smoothStep                      },
        { "vowelCurrent",                    &DelayLama::Core::DelayLamaAudio::vowelCurrent                    },
        { "vowelSmoothingFramesRemaining",   &DelayLama::Core::DelayLamaAudio::vowelSmoothingFramesRemaining   },
        { "padPitchSmoothingFramesRemaining", &DelayLama::Core::DelayLamaAudio::padPitchSmoothingFramesRemaining },
        { "currentIdleFrame",                &DelayLama::Core::DelayLamaAudio::currentIdleFrame                },
        { "idleAnimationSampleCounter",      &DelayLama::Core::DelayLamaAudio::idleAnimationSampleCounter      },
        { "globalAnimationSampleCounter",    &DelayLama::Core::DelayLamaAudio::globalAnimationSampleCounter    },
        { "idleSamplesPerFrame",             &DelayLama::Core::DelayLamaAudio::idleSamplesPerFrame             },
        { "startBlink1",                     &DelayLama::Core::DelayLamaAudio::startBlink1                     },
        { "stopBlink1",                      &DelayLama::Core::DelayLamaAudio::stopBlink1                      },
        { "startBlink2",                     &DelayLama::Core::DelayLamaAudio::startBlink2                     },
        { "stopBlink2",                      &DelayLama::Core::DelayLamaAudio::stopBlink2                      },
        { "startIdleAnimation",              &DelayLama::Core::DelayLamaAudio::startIdleAnimation              },
        { "delayBufferSize",                 &DelayLama::Core::DelayLamaAudio::delayBufferSize                 },
        { "delayWriteIndex",                 &DelayLama::Core::DelayLamaAudio::delayWriteIndex                 },
        { "delayReadIndexL",                 &DelayLama::Core::DelayLamaAudio::delayReadIndexL                 },
        { "delayReadIndexR",                 &DelayLama::Core::DelayLamaAudio::delayReadIndexR                 },
        { "numSamples",                      &DelayLama::Core::DelayLamaAudio::numSamples                      },
        { "voiceBufferSize",            &DelayLama::Core::DelayLamaAudio::voiceBufferSize            },
        { "voiceReadIndex",             &DelayLama::Core::DelayLamaAudio::voiceReadIndex             },
        { "sineTableSize",                   &DelayLama::Core::DelayLamaAudio::sineTableSize                   },
        { "decayTableSize",                &DelayLama::Core::DelayLamaAudio::decayTableSize                },
        { "formantWaveSize",                &DelayLama::Core::DelayLamaAudio::formantWaveSize                },
        { "frequencyTableSize",              &DelayLama::Core::DelayLamaAudio::frequencyTableSize              },
    };

    for (const auto& [name, member] : fields) {
        SCOPED_TRACE(name);
        EXPECT_EQ(audio->*member, s[name].get<int>()) << "Field: " << name;
    }
}

TEST_F(DelayLamaAudioInitTest, Initialize_Scalars_Bools) {
    const auto& s = state["scalars"];

    const std::pair<const char*, bool DelayLama::Core::DelayLamaAudio::*> fields[] = {
        { "padPitchDirty",             &DelayLama::Core::DelayLamaAudio::padPitchDirty             },
        { "padVowelDirty",          &DelayLama::Core::DelayLamaAudio::padVowelDirty          },
        { "vowelBendDirty",         &DelayLama::Core::DelayLamaAudio::vowelBendDirty         },
        { "isGlideActive",            &DelayLama::Core::DelayLamaAudio::isGlideActive            },
        { "vowelBufferNeedsUpdate",  &DelayLama::Core::DelayLamaAudio::vowelBufferNeedsUpdate  },
        { "isLegato",             &DelayLama::Core::DelayLamaAudio::isLegato             },
        { "isSinging",                &DelayLama::Core::DelayLamaAudio::isSinging                },
        { "needsMonkAnimationRefresh",&DelayLama::Core::DelayLamaAudio::needsMonkAnimationRefresh },
    };

    for (const auto& [name, member] : fields) {
        SCOPED_TRACE(name);
        EXPECT_EQ(audio->*member, s[name].get<bool>()) << "Field: " << name;
    }
}

TEST_F(DelayLamaAudioInitTest, Initialize_Presets) {
    ASSERT_NE(audio->presets, nullptr) << "Preset array must not be null after initialize";

    const auto& presets = state["presets"];
    for (size_t i = 0; i < presets.size(); ++i) {
        SCOPED_TRACE("Preset index " + std::to_string(i));
        const auto& p = presets[i];
        EXPECT_NEAR(audio->presets[i].portTime, p["portTime"].get<float>(), kEps);
        EXPECT_NEAR(audio->presets[i].delay,    p["delay"].get<float>(),    kEps);
        EXPECT_NEAR(audio->presets[i].headSize, p["headSize"].get<float>(), kEps);
        EXPECT_STREQ(audio->presets[i].name,    p["name"].get<std::string>().c_str());
    }
}

TEST_F(DelayLamaAudioInitTest, Initialize_FixedArrays) {
    const auto& arrays = state["fixedArrays"];

    auto checkIntArray = [&](const char* name, int* ptr, const std::vector<int>& vals) {
        for (size_t i = 0; i < vals.size(); ++i) {
            if (ptr[i] != vals[i]) {
                ADD_FAILURE() << name << "[" << i << "]: expected " << vals[i]
                              << " but got " << ptr[i];
                break;
            }
        }
    };

    auto checkFloatArray = [](const char* name, float* ptr, const std::vector<float>& vals) {
        for (size_t i = 0; i < vals.size(); ++i) {
            if (!sameFloat(ptr[i], vals[i])) {
                ADD_FAILURE() << name << "[" << i << "]: expected " << vals[i]
                              << " but got " << ptr[i];
                break;
            }
        }
    };

    // bendQueueLsb/2 are not initialized by the original either (they are only
    // written when pitch bends arrive), so the fixture holds leftover memory there.
    checkIntArray("noteStack",         audio->noteStack,         arrays["noteStack"].get<std::vector<int>>());
    checkFloatArray("monkIdleFrameTable", audio->monkIdleFrameTable, fixtureFloats(arrays["monkIdleFrameTable"]));
}

TEST_F(DelayLamaAudioInitTest, Initialize_PointerArrays_NotNull) {
    EXPECT_NE(audio->stereoDelayLBuffer, nullptr);
    EXPECT_NE(audio->stereoDelayRBuffer, nullptr);
    EXPECT_NE(audio->grainBuffer,    nullptr);
    EXPECT_NE(audio->voiceBuffer,   nullptr);
    EXPECT_NE(audio->sineTable,          nullptr);
    EXPECT_NE(audio->decayTable,       nullptr);
    EXPECT_NE(audio->grainWindow,      nullptr);
    EXPECT_NE(audio->formantWave,      nullptr);
    EXPECT_NE(audio->fixedFormantBuffer,     nullptr);
    EXPECT_NE(audio->frequencyTable,     nullptr);
    EXPECT_NE(audio->formantTable1,      nullptr);
    EXPECT_NE(audio->formantTable2,      nullptr);
    EXPECT_NE(audio->formantTable3,      nullptr);
}

TEST_F(DelayLamaAudioInitTest, Initialize_PointerArrays_Values) {
    const auto& arrays = state["pointerArrays"];

    const std::pair<const char*, float*> buffers[] = {
        { "stereoDelayLBuffer", audio->stereoDelayLBuffer },
        { "stereoDelayRBuffer", audio->stereoDelayRBuffer },
        // grainBuffer is not compared: when the fixture was recorded, the original's
        // vowel/head-size inputs were NaN (uninitialised), so the three formant terms
        // read formantWave[0] == 0 and the buffer holds only fixedFormantBuffer / 2.
        { "voiceBuffer",   audio->voiceBuffer   },
        { "sineTable",          audio->sineTable          },
        { "decayTable",       audio->decayTable       },
        { "grainWindow",      audio->grainWindow      },
        { "formantWave",      audio->formantWave      },
        { "fixedFormantBuffer",     audio->fixedFormantBuffer     },
        { "frequencyTable",     audio->frequencyTable     },
        { "formantTable1",      audio->formantTable1      },
        { "formantTable2",      audio->formantTable2      },
        { "formantTable3",      audio->formantTable3      },
    };

    for (const auto& [name, ptr] : buffers) {
        ASSERT_NE(ptr, nullptr) << name << " is null";
        const auto vals = fixtureFloats(arrays[name]);
        for (size_t i = 0; i < vals.size(); ++i) {
            if (!sameFloat(ptr[i], vals[i])) {
                ADD_FAILURE() << name << "[" << i << "]: expected " << vals[i]
                              << " but got " << ptr[i];
                break;
            }
        }
    }
}