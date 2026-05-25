#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
#include "core/DelayLamaAudio.h"

using json = nlohmann::json;

static intptr_t stubHostCallback(struct DamSDK::Api::DamPlugin* plugin, int32_t targetOperation, int32_t index, int32_t value, void * data, float optional) {
    return 0;
}

static constexpr float kEps = 1e-5f;

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

auto checkFloatArray = [&](const char* name, float* ptr, const std::vector<float>& vals) {
    ASSERT_NE(ptr, nullptr) << name << " is null";
    for (size_t i = 0; i < vals.size(); ++i) {
        if (std::abs(ptr[i] - vals[i]) > kEps) {
            ADD_FAILURE() << name << "[" << i << "]: expected " << vals[i]
                          << " but got " << ptr[i];
            break;
        }
    }
};

auto checkIntArray = [&](const char* name, int* ptr, const std::vector<int>& vals) {
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
        { "portamentoTime",           &DelayLama::Core::DelayLamaAudio::portamentoTime           },
        { "curVowelValue",            &DelayLama::Core::DelayLamaAudio::curVowelValue            },
        { "prevVowelValue",           &DelayLama::Core::DelayLamaAudio::prevVowelValue           },
        { "delay",                    &DelayLama::Core::DelayLamaAudio::delay                    },
        { "monkSprite",               &DelayLama::Core::DelayLamaAudio::monkSprite               },
        { "headSize",                 &DelayLama::Core::DelayLamaAudio::headSize                 },
        { "vibratoDepthCurrent",      &DelayLama::Core::DelayLamaAudio::vibratoDepthCurrent      },
        { "vibratoAmount",            &DelayLama::Core::DelayLamaAudio::vibratoAmount            },
        { "pitchValue",               &DelayLama::Core::DelayLamaAudio::pitchValue               },
        { "outputGain",               &DelayLama::Core::DelayLamaAudio::outputGain               },
        { "vowelTargetValue",         &DelayLama::Core::DelayLamaAudio::vowelTargetValue         },
        { "formantMorphValue",        &DelayLama::Core::DelayLamaAudio::formantMorphValue        },
        { "currentFormantMorphValue", &DelayLama::Core::DelayLamaAudio::currentFormantMorphValue },
        { "lfoPhaseAccumulator",      &DelayLama::Core::DelayLamaAudio::lfoPhaseAccumulator      },
        { "lfoPhaseWrapValue",        &DelayLama::Core::DelayLamaAudio::lfoPhaseWrapValue        },
        { "lfoDepth",                 &DelayLama::Core::DelayLamaAudio::lfoDepth                 },
        { "lfoSampleValue",           &DelayLama::Core::DelayLamaAudio::lfoSampleValue           },
        { "lfoPhaseIncrement",        &DelayLama::Core::DelayLamaAudio::lfoPhaseIncrement        },
        { "vowelLookupIndex",         &DelayLama::Core::DelayLamaAudio::vowelLookupIndex         },
        { "formant1Bandwidth",        &DelayLama::Core::DelayLamaAudio::formant1Bandwidth        },
        { "formant2Bandwidth",        &DelayLama::Core::DelayLamaAudio::formant2Bandwidth        },
        { "formant3Bandwidth",        &DelayLama::Core::DelayLamaAudio::formant3Bandwidth        },
        { "vowelBlendFactor",         &DelayLama::Core::DelayLamaAudio::vowelBlendFactor         },
        { "vibratoCurrent",           &DelayLama::Core::DelayLamaAudio::vibratoCurrent           },
        { "pluginSampleRate",         &DelayLama::Core::DelayLamaAudio::pluginSampleRate         },
        { "prevSampleRate",           &DelayLama::Core::DelayLamaAudio::prevSampleRate           },
        { "delayFeedback",            &DelayLama::Core::DelayLamaAudio::delayFeedback            },
        { "glottalPhaseInc",          &DelayLama::Core::DelayLamaAudio::glottalPhaseInc          },
        { "unusedFloat",              &DelayLama::Core::DelayLamaAudio::unusedFloat              },
        { "frequencyValue",           &DelayLama::Core::DelayLamaAudio::frequencyValue           },
        { "delayTimeScaler",          &DelayLama::Core::DelayLamaAudio::delayTimeScaler          },
    };

    for (const auto& [name, member] : fields) {
        SCOPED_TRACE(name);
        EXPECT_NEAR(audio->*member, s[name].get<float>(), kEps) << "Field: " << name;
    }
}

TEST_F(DelayLamaAudioInitTest, Initialize_Scalars_Ints) {
    const auto& s = state["scalars"];

    const std::pair<const char*, int DelayLama::Core::DelayLamaAudio::*> fields[] = {
        { "isInterpActive",                  &DelayLama::Core::DelayLamaAudio::isInterpActive                  },
        { "interpEventCount",                &DelayLama::Core::DelayLamaAudio::interpEventCount                },
        { "interpSampleStep",                &DelayLama::Core::DelayLamaAudio::interpSampleStep                },
        { "interpCurrentIdx",                &DelayLama::Core::DelayLamaAudio::interpCurrentIdx                },
        { "synthesisFrameCounter",           &DelayLama::Core::DelayLamaAudio::synthesisFrameCounter           },
        { "pitchTargetValue",                &DelayLama::Core::DelayLamaAudio::pitchTargetValue                },
        { "formantMorphStep",                &DelayLama::Core::DelayLamaAudio::formantMorphStep                },
        { "lfoReseedIntervalSamples",        &DelayLama::Core::DelayLamaAudio::lfoReseedIntervalSamples        },
        { "sampleCounter",                   &DelayLama::Core::DelayLamaAudio::sampleCounter                   },
        { "writeIndex",                      &DelayLama::Core::DelayLamaAudio::writeIndex                      },
        { "frequencyIndex",                  &DelayLama::Core::DelayLamaAudio::frequencyIndex                  },
        { "excitationWriteIndex",            &DelayLama::Core::DelayLamaAudio::excitationWriteIndex            },
        { "attackSamples",                   &DelayLama::Core::DelayLamaAudio::attackSamples                   },
        { "sustainStart",                    &DelayLama::Core::DelayLamaAudio::sustainStart                    },
        { "releaseSamples",                  &DelayLama::Core::DelayLamaAudio::releaseSamples                  },
        { "totalSmoothingFrames",            &DelayLama::Core::DelayLamaAudio::totalSmoothingFrames            },
        { "smoothCounter",                   &DelayLama::Core::DelayLamaAudio::smoothCounter                   },
        { "smoothingFrames",                 &DelayLama::Core::DelayLamaAudio::smoothingFrames                 },
        { "smoothStep",                      &DelayLama::Core::DelayLamaAudio::smoothStep                      },
        { "pitchCurrent",                    &DelayLama::Core::DelayLamaAudio::pitchCurrent                    },
        { "pitchTarget",                     &DelayLama::Core::DelayLamaAudio::pitchTarget                     },
        { "pitchDelta",                      &DelayLama::Core::DelayLamaAudio::pitchDelta                      },
        { "pitchSmoothingFramesRemaining",   &DelayLama::Core::DelayLamaAudio::pitchSmoothingFramesRemaining   },
        { "pitchStep",                       &DelayLama::Core::DelayLamaAudio::pitchStep                       },
        { "vibratoTarget",                   &DelayLama::Core::DelayLamaAudio::vibratoTarget                   },
        { "vibratoDelta",                    &DelayLama::Core::DelayLamaAudio::vibratoDelta                    },
        { "vibratoStep",                     &DelayLama::Core::DelayLamaAudio::vibratoStep                     },
        { "vibratoSmoothingFramesRemaining", &DelayLama::Core::DelayLamaAudio::vibratoSmoothingFramesRemaining },
        { "pluginBlockSize",                 &DelayLama::Core::DelayLamaAudio::pluginBlockSize                 },
        { "rngState",                        &DelayLama::Core::DelayLamaAudio::rngState                        },
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
        { "excitationBufferSize",            &DelayLama::Core::DelayLamaAudio::excitationBufferSize            },
        { "excitationReadIndex",             &DelayLama::Core::DelayLamaAudio::excitationReadIndex             },
        { "sineTableSize",                   &DelayLama::Core::DelayLamaAudio::sineTableSize                   },
        { "formantTableSize",                &DelayLama::Core::DelayLamaAudio::formantTableSize                },
        { "glottalTableSize",                &DelayLama::Core::DelayLamaAudio::glottalTableSize                },
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
        { "vibratoDirty",             &DelayLama::Core::DelayLamaAudio::vibratoDirty             },
        { "pitchValueDirty",          &DelayLama::Core::DelayLamaAudio::pitchValueDirty          },
        { "pitchTargetDirty",         &DelayLama::Core::DelayLamaAudio::pitchTargetDirty         },
        { "isGlideActive",            &DelayLama::Core::DelayLamaAudio::isGlideActive            },
        { "formantTableNeedsUpdate",  &DelayLama::Core::DelayLamaAudio::formantTableNeedsUpdate  },
        { "unusedBool",               &DelayLama::Core::DelayLamaAudio::unusedBool               },
        { "isGateActive",             &DelayLama::Core::DelayLamaAudio::isGateActive             },
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

    auto checkFloatArray = [&](const char* name, float* ptr, const std::vector<float>& vals) {
        for (size_t i = 0; i < vals.size(); ++i) {
            if (std::abs(ptr[i] - vals[i]) > kEps) {
                ADD_FAILURE() << name << "[" << i << "]: expected " << vals[i]
                              << " but got " << ptr[i];
                break;
            }
        }
    };

    checkIntArray("pitchInterpData1",  audio->pitchInterpData1,  arrays["pitchInterpData1"].get<std::vector<int>>());
    checkIntArray("pitchInterpData2",  audio->pitchInterpData2,  arrays["pitchInterpData2"].get<std::vector<int>>());
    checkIntArray("noteStack",         audio->noteStack,         arrays["noteStack"].get<std::vector<int>>());
    checkFloatArray("monkIdleFrameTable", audio->monkIdleFrameTable, arrays["monkIdleFrameTable"].get<std::vector<float>>());
}

TEST_F(DelayLamaAudioInitTest, Initialize_PointerArrays_NotNull) {
    EXPECT_NE(audio->stereoDelayLBuffer, nullptr);
    EXPECT_NE(audio->stereoDelayRBuffer, nullptr);
    EXPECT_NE(audio->synthesisBuffer,    nullptr);
    EXPECT_NE(audio->excitationBuffer,   nullptr);
    EXPECT_NE(audio->sineTable,          nullptr);
    EXPECT_NE(audio->formantTable,       nullptr);
    EXPECT_NE(audio->vocalEnvelope,      nullptr);
    EXPECT_NE(audio->glottalSource,      nullptr);
    EXPECT_NE(audio->harmonicBuffer,     nullptr);
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
        { "synthesisBuffer",    audio->synthesisBuffer    },
        { "excitationBuffer",   audio->excitationBuffer   },
        { "sineTable",          audio->sineTable          },
        { "formantTable",       audio->formantTable       },
        { "vocalEnvelope",      audio->vocalEnvelope      },
        { "glottalSource",      audio->glottalSource      },
        { "harmonicBuffer",     audio->harmonicBuffer     },
        { "frequencyTable",     audio->frequencyTable     },
        { "formantTable1",      audio->formantTable1      },
        { "formantTable2",      audio->formantTable2      },
        { "formantTable3",      audio->formantTable3      },
    };

    for (const auto& [name, ptr] : buffers) {
        ASSERT_NE(ptr, nullptr) << name << " is null";
        const auto vals = arrays[name].get<std::vector<float>>();
        for (size_t i = 0; i < vals.size(); ++i) {
            if (std::abs(ptr[i] - vals[i]) > kEps) {
                ADD_FAILURE() << name << "[" << i << "]: expected " << vals[i]
                              << " but got " << ptr[i];
                break;
            }
        }
    }
}