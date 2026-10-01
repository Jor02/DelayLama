#include "DelayLamaAudio.h"
#include "PluginConfig.h"
#include "core/PluginConfig.h"
#include <cmath>
#include "damsdk/utils/portable_stdint.h"
#include <cstdio>
#include <gui/controls/Monk.h>
#include "utils/Logger.h"

namespace DelayLama {
namespace Core {

    // FUNCTION: DELAYLAMA 0x10002810
    Preset::Preset() {}

    // FUNCTION: DELAYLAMA 0x100015b0 FOLDED
    Preset::~Preset() {}

    // FUNCTION: DELAYLAMA 0x10002820
    DelayLamaAudio::DelayLamaAudio(DamSDK::Api::dispatchFunc hostCallback) : DamSDK::Api::AudioBaseExtended(hostCallback, PRESET_COUNT, PARAMETER_COUNT) {
        Utils::log("DelayLamaAudio::ctor\n");
        this->grainBuffer = nullptr;
        this->voiceBuffer = nullptr;
        this->decayTable = nullptr;
        this->formantWave = nullptr;
        this->fixedFormantBuffer = nullptr;
        this->sineTable = nullptr;
        this->grainWindow = nullptr;
        this->frequencyTable = nullptr;
        this->stereoDelayLBuffer = nullptr;
        this->stereoDelayRBuffer = nullptr;
        this->formantTable1 = nullptr;
        this->formantTable2 = nullptr;
        this->formantTable3 = nullptr;
        this->isSinging = false;

        this->presets = new Preset[5];

        if (this->presets != nullptr) {
            initPresets();
            loadPresetByIndex(0);
        }

        if (this->hostCallback != nullptr) {
            this->plugin.inputChannelCount = 0;
            this->plugin.outputChannelCount = 2;
            this->setSupportsInPlaceProcessing(true);
            this->setReportsLoudnessToHost(false);
            this->setHasClip(false);
            this->setIsSynthesizer(true);
            this->plugin.id = 'AnDl';
        }

        this->prevSampleRate = 0.0f;
        this->pluginSampleRate = 0.0f;
        this->curVowelValue = 0.5f;
        this->padPitchSlider = 0.0f;
        this->monkSprite = 0.1667f;
    }

    // FUNCTION: DELAYLAMA 0x100029a0
    DelayLamaAudio::~DelayLamaAudio() {
        Utils::log("DelayLamaAudio::destroy\n");

        if (this->presets != nullptr) {
            delete[] this->presets;
            this->presets = nullptr;
        }

        // Delete all dynamically allocated float buffers
        if (this->grainBuffer != nullptr) {
            delete[] this->grainBuffer;
            this->grainBuffer = nullptr;
        }
        if (this->voiceBuffer != nullptr) {
            delete[] this->voiceBuffer;
            this->voiceBuffer = nullptr;
        }
        if (this->sineTable != nullptr) {
            delete[] this->sineTable;
            this->sineTable = nullptr;
        }
        if (this->decayTable != nullptr) {
            delete[] this->decayTable;
            this->decayTable = nullptr;
        }
        if (this->grainWindow != nullptr) {
            delete[] this->grainWindow;
            this->grainWindow = nullptr;
        }
        if (this->formantWave != nullptr) {
            delete[] this->formantWave;
            this->formantWave = nullptr;
        }
        if (this->fixedFormantBuffer != nullptr) {
            delete[] this->fixedFormantBuffer;
            this->fixedFormantBuffer = nullptr;
        }
        if (this->frequencyTable != nullptr) {
            delete[] this->frequencyTable;
            this->frequencyTable = nullptr;
        }
        if (this->stereoDelayLBuffer != nullptr) {
            delete[] this->stereoDelayLBuffer;
            this->stereoDelayLBuffer = nullptr;
        }
        if (this->stereoDelayRBuffer != nullptr) {
            delete[] this->stereoDelayRBuffer;
            this->stereoDelayRBuffer = nullptr;
        }
        if (this->formantTable1 != nullptr) {
            delete[] this->formantTable1;
            this->formantTable1 = nullptr;
        }
        if (this->formantTable2 != nullptr) {
            delete[] this->formantTable2;
            this->formantTable2 = nullptr;
        }
        if (this->formantTable3 != nullptr) {
            delete[] this->formantTable3;
            this->formantTable3 = nullptr;
        }

        // Call base class destructor
    }

    // FUNCTION: DELAYLAMA 0x10003110
    bool DelayLamaAudio::getPluginName(char *outText) {
        strcpy(outText, "Delay Lama");
        return true;
    }
    
    // FUNCTION: DELAYLAMA 0x10003140
    bool DelayLamaAudio::getCompanyName(char *outText) {
        strcpy(outText, "AudioNerdz");
        return true;
    }

    const int kPitchBendCenter = 8192; 
    const int kVoiceBufferSize = 10240;
// MIDI status bytes
#define kMidiNoteOff       0x80
#define kMidiNoteOn        0x90
#define kMidiControlChange 0xB0
#define kMidiPitchBend     0xE0

#define kGrainSeconds ((double)(0.02))  // grain length: 20 milliseconds
#define kPi ((double)(3.141592654f))  // pi
#define kPi2 ((double)(6.283185307f))  // 2.0 * pi
#define kPi50 ((double)(157.0796327))  // 50.0 * pi
#define kMidiNote0Frequency ((double)(8.175798916))
#define kAttackTime ((double)(0.0018))
#define kSustainTime ((double)(0.013))
#define kReleaseTime ((double)(0.007))
#define kPitchToFloatScale ((float)(1.0f / 16383.0f))  // as in the original

    // FUNCTION: DELAYLAMA 0x100048d0
    void DelayLamaAudio::initialize() {
        int i; // For loop index

        double currentSampleRate = this->getSampleRate();
        Utils::logf("DelayLamaAudio::initialize sampleRate=%f\n", currentSampleRate);
        this->pluginSampleRate = (float)currentSampleRate;
        if (currentSampleRate != this->prevSampleRate) {
            if (this->grainBuffer != nullptr) {
                delete[] this->grainBuffer;
            }
            if (this->voiceBuffer != nullptr) {
                delete[] this->voiceBuffer;
            }
            if (this->sineTable != nullptr) {
                delete[] this->sineTable;
            }
            if (this->decayTable != nullptr) {
                delete[] this->decayTable;
            }
            if (this->grainWindow != nullptr) {
                delete[] this->grainWindow;
            }
            if (this->formantWave != nullptr) {
                delete[] this->formantWave;
            }
            if (this->fixedFormantBuffer != nullptr) {
                delete[] this->fixedFormantBuffer;
            }
            if (this->frequencyTable != nullptr) {
                delete[] this->frequencyTable;
            }
            if (this->stereoDelayLBuffer != nullptr) {
                delete[] this->stereoDelayLBuffer;
            }
            if (this->stereoDelayRBuffer != nullptr) {
                delete[] this->stereoDelayRBuffer;
            }
            if (this->formantTable1 != nullptr) {
                delete[] this->formantTable1;
            }
            if (this->formantTable2 != nullptr) {
                delete[] this->formantTable2;
            }
            if (this->formantTable3 != nullptr) {
                delete[] this->formantTable3;
            }

            // Nullify all dangling pointers
            this->grainBuffer = nullptr;
            this->voiceBuffer = nullptr;
            this->decayTable = nullptr;
            this->formantWave = nullptr;
            this->fixedFormantBuffer = nullptr;
            this->sineTable = nullptr;
            this->grainWindow = nullptr;
            this->frequencyTable = nullptr;
            this->stereoDelayLBuffer = nullptr;
            this->stereoDelayRBuffer = nullptr;
            this->formantTable1 = nullptr;
            this->formantTable2 = nullptr;
            this->formantTable3 = nullptr;
        }
        this->prevSampleRate = this->pluginSampleRate;

        // Allocate and Initialize Synthesis Buffers
        this->numSamples = static_cast<long>(this->pluginSampleRate * kGrainSeconds);
        if (this->grainBuffer == nullptr) {
            this->grainBuffer = new float[this->numSamples];
        }

        this->voiceBufferSize = kVoiceBufferSize;
        if (this->voiceBuffer == nullptr) {
            this->voiceBuffer = new float[this->voiceBufferSize];
        }

        // Clear the voice buffer
        for (i = 0; i < this->voiceBufferSize; ++i) {
            this->voiceBuffer[i] = 0.0f;
        }

        // Exponential decay table, shared by the three formants
        this->decayTableSize = this->numSamples << 2;
        if (this->decayTable == nullptr) {
            this->decayTable = new float[this->decayTableSize];
        }

        float decayFactor = (float)(kPi50 / this->pluginSampleRate);
        int tableSize = this->decayTableSize;

        if (this->decayTable != nullptr) {
            for (i = 0; i < tableSize; ++i) {
                this->decayTable[i] = (float)::exp(-i * decayFactor);
            }
        }

        // Sine wavetable the three formants oscillate on
        this->formantWaveSize = 1024;
        if (this->formantWave == nullptr) {
            this->formantWave = new float[this->formantWaveSize];
        }

        for (i = 0; i < this->formantWaveSize; ++i) {
            double value = sin((i * kPi2) / (double)this->formantWaveSize);
            this->formantWave[i] = (float)value;
        }

        this->formantWaveStepsPerHz = (float)this->formantWaveSize / (float)this->pluginSampleRate;
        
        // Two fixed high formants (about 4950 Hz and 3800 Hz), added to every grain
        if (this->fixedFormantBuffer == nullptr) {
            this->fixedFormantBuffer = new float[this->numSamples];
        }

        // Two decaying sine components, each shaped by the formant decay table
        double envPhase2 = 0.0;
        double envPhase1 = 0.0;
        for (i = 0; i < this->numSamples; ++i) {
            double phase = i * 6.283185307;
            this->fixedFormantBuffer[i] = (float)sin(phase / (this->pluginSampleRate * 0.00020202021f)) * this->decayTable[(long)envPhase1];
            this->fixedFormantBuffer[i] += (float)sin(phase / (this->pluginSampleRate * 0.00026315788f)) * this->decayTable[(long)envPhase2];
            envPhase1 += 3.0f;
            envPhase2 += 3.6f;
        }

        // FO / Sine Table Initialization
        this->sineTableSize = 1024;

        if (this->sineTable == nullptr) {
            this->sineTable = new float[this->sineTableSize];
        }

        for (i = 0; i < this->sineTableSize; ++i) {
            this->sineTable[i] = sinf(((float)i * 6.283185307f) / (float)this->sineTableSize);
        }

        // Envelope Generator (ADSR style shaping)
         if (this->grainWindow == nullptr) {
            this->grainWindow = new float[this->numSamples];
        }

        this->attackSamples  = static_cast<long>(this->pluginSampleRate * kAttackTime);
        this->sustainStart   = static_cast<long>(this->pluginSampleRate * kSustainTime);
        this->releaseSamples = static_cast<long>(this->pluginSampleRate * kReleaseTime);

        // Fill Envelope with 1.0 (0x3f800000)
        for (i = 0; i < this->numSamples; ++i) {
            this->grainWindow[i] = 1.0f;
        }
        
        // Apply Attack Phase (Cosine shaping)
        const int attackSamples = this->attackSamples;
        if (attackSamples > 0) {
            for (i = 0; i < attackSamples; ++i) {
                double phase = (static_cast<double>(i) * kPi) / static_cast<double>(attackSamples);
                float value = static_cast<float>(0.5 * (1.0 - cos(phase)));
                this->grainWindow[i] = value;
            }
        }

        // Apply Release Phase (Cosine shaping)
        for (int idx = this->sustainStart; idx < this->numSamples; ++idx) {
            double phase = (static_cast<double>(idx) * kPi) / static_cast<double>(this->releaseSamples);
            float value = static_cast<float>(0.5 * (1.0 + cos(phase)));
            this->grainWindow[idx] = value;
        }
        
        // Pitch/Frequency Lookup Table
        this->frequencyTableSize = 4096;
        if (this->frequencyTable == nullptr) {
            this->frequencyTable = new float[this->frequencyTableSize];
        }

        // MIDI to frequency conversion: f = 8.175798916 * 2^(i / 12)
        for (i = 0; i < this->frequencyTableSize; ++i) {
            double exponent = static_cast<double>(i) / 12.0;
            this->frequencyTable[i] = static_cast<float>(kMidiNote0Frequency * pow(2.0, static_cast<double>(i) / 384.0));
        }

        // Formant Control Points Setup (Vowel filters)
        int32_t formantControlPoints [15];

        // First curve (indices 0-4)
        formantControlPoints[0] = 280;
        formantControlPoints[1] = 450;
        formantControlPoints[2] = 800;
        formantControlPoints[3] = 350;
        formantControlPoints[4] = 270;
        
        // Second curve (indices 5-9)
        formantControlPoints[5] = 600;
        formantControlPoints[6] = 800;
        formantControlPoints[7] = 1150;
        formantControlPoints[8] = 2000;
        formantControlPoints[9] = 2140;

        // Third curve (indices 10-14)
        formantControlPoints[10] = 2240;
        formantControlPoints[11] = 2830;
        formantControlPoints[12] = 2900;
        formantControlPoints[13] = 2800;
        formantControlPoints[14] = 2950;

        // Allocate and build three formant interpolation tables (each 1280 floats)
        if (this->formantTable1 == nullptr) {
            this->formantTable1 = new float[1280];
        }
        buildFormantCurveTable(formantControlPoints, this->formantTable1);

        if (this->formantTable2 == nullptr) {
            this->formantTable2 = new float[1280];
        }
        buildFormantCurveTable(formantControlPoints + 5, this->formantTable2);

        if (this->formantTable3 == nullptr) {
            this->formantTable3 = new float[1280];
        }
        buildFormantCurveTable(formantControlPoints + 10, this->formantTable3);

        this->formant1Bandwidth = 32.5f;
        this->formant2Bandwidth = 47.5f;
        this->formant3Bandwidth = 62.5f;
        
        this->formant1Bandwidth *= 0.02f;
        this->formant2Bandwidth *= 0.02f;
        this->formant3Bandwidth *= 0.02f;
        
        // Reset MIDI Events and Stack
        for (i = 0; i < 1024; ++i) {
            this->midiQueue[i].timestamp = 0;
            this->midiQueue[i].status = 0;
            this->midiQueue[i].data1 = 0;
            this->midiQueue[i].data2 = 0;
        }

        for (i = 0; i < 128; ++i) {
            this->noteStack[i] = 0;
        }

        // Vowel Preset Values
        this->monkIdleFrameTable[0]  = 0.1667f;
        this->monkIdleFrameTable[1]  = 0.1f;
        this->monkIdleFrameTable[2]  = 0.1333f;
        this->monkIdleFrameTable[3]  = 0.1f;
        this->monkIdleFrameTable[4]  = 0.0667f;
        this->monkIdleFrameTable[5]  = 0.0333f;
        this->monkIdleFrameTable[6]  = 0.0f;
        this->monkIdleFrameTable[7]  = 0.0333f;
        this->monkIdleFrameTable[8]  = 0.1667f;
        this->monkIdleFrameTable[9]  = 0.1f;
        this->monkIdleFrameTable[10] = 0.1333f;
        this->monkIdleFrameTable[11] = 0.1f;
        this->monkIdleFrameTable[12] = 0.1667f;
        this->monkIdleFrameTable[13] = 0.0333f;
        this->monkIdleFrameTable[14] = 0.0f;
        this->monkIdleFrameTable[15] = 0.0333f;
        this->monkIdleFrameTable[16] = 0.0667f;
        this->monkIdleFrameTable[17] = 0.1f;
        this->monkIdleFrameTable[18] = 0.1333f;
        this->monkIdleFrameTable[19] = 0.1f;
        this->monkIdleFrameTable[20] = 0.1667f;
        this->monkIdleFrameTable[21] = 0.0333f;
        this->monkIdleFrameTable[22] = 0.0f;
        this->monkIdleFrameTable[23] = 0.0333f;

        // Delay Effect Initialization
        this->delayBufferSize = 20000;

        if (this->stereoDelayLBuffer == nullptr) {
            this->stereoDelayLBuffer = new float[this->delayBufferSize];
        }

        if (this->stereoDelayRBuffer == nullptr) {
            this->stereoDelayRBuffer = new float[this->delayBufferSize];
        }

        // Clear both delay lines to zero
        for (i = 0; i < this->delayBufferSize; ++i) {
            this->stereoDelayLBuffer[i] = 0.0f;
            this->stereoDelayRBuffer[i] = 0.0f;
        }

        // General Synthesizer States & Smoothing Variables Reset
        this->synthesisFrameCounter = 0;
        this->vowelBendLsb = 64;
        this->vowelBendMsb = 64;
        this->vowelTargetValue = 0.5f;
        this->vowelBendDirty = false;
        this->isGlideActive = false;
        this->isSinging = false;
        this->vowelBufferNeedsUpdate = true;
        this->currentMidiEventData1 = 0;
        this->currentMidiEventData2 = 1;
        this->outputGain = 0.1f;
        this->padVowelDirty = false;
        this->padPitchDirty = false;

        synthesizeVowelBuffer(0.5f);

        this->prevVowelValue = 0.5;
        this->isLegato = false;
        this->voicePitch = 36.0;
        this->lfoRate = 4.0;
        this->vibratoDepth = 0.0;
        this->vibratoOffset = 0.0;
        this->lfoPhaseAccumulator = 0.0;
        this->lfoPhaseIncrement = this->pluginSampleRate / (float)this->sineTableSize;

        // Vowel timing/frame math
        this->lfoReseedIntervalSamples = (int)(this->sampleRate * 104.0 * 0.001); // 104 ms
        this->needsMonkAnimationRefresh = true;
        this->globalAnimationSampleCounter = 0;
        this->idleAnimationSampleCounter = 0;
        this->currentIdleFrame = 0;

        // Vowel Trigger Envelope/Sequence Steps
        int idleSamplesPerFrame = (int)(this->sampleRate * 0.208f);
        this->idleSamplesPerFrame = idleSamplesPerFrame;
        this->startBlink1 = idleSamplesPerFrame * 7;
        this->stopBlink1 = (int)(idleSamplesPerFrame * 8.5f);
        this->startBlink2 = idleSamplesPerFrame * 15;
        this->stopBlink2 = idleSamplesPerFrame * 17;
        this->startIdleAnimation = idleSamplesPerFrame * 23;

        this->pulseWriteIndex = 0;
        this->voiceReadIndex = 0;
        this->samplesSincePulse = 0;
        this->sampleCounter = 0;

        // Delay Line setup
        this->delayWriteIndex = 0;
        this->rngScale = (float)(1.0 / pow(2.0, 32.0)); 
        this->delayReadIndexL = (int)(this->sampleRate * -0.309592);
        this->delayReadIndexR = (int)(this->sampleRate * -0.398435);
        this->delayFeedback = 0.5;

        // Smoothing configuration
        this->totalSmoothingFrames = (int)(this->sampleRate * 0.001 * 100.0); // 100 ms
        this->smoothCounter = (int)(this->sampleRate * 0.0099999998);         // 10 ms
        this->smoothingFrames = this->totalSmoothingFrames / this->smoothCounter;

        this->vowelCurrent = kPitchBendCenter; // Assuming this is 8192 (0x2000) based on '00 20 00 00'
        this->padPitchCurrent = 36.0;
        this->smoothStep = 0;
        this->vowelSmoothingFramesRemaining = 0;
        this->padPitchSmoothingFramesRemaining = 0;
    }

#define kPitchScaleFactor ((float)(16384.0f))
#define closeEyes 0.06667f  // rounded literals, as in the original
#define openEyes 0.16667f

    // FUNCTION: DELAYLAMA 0x100054c0
    void DelayLamaAudio::processAudio(float** inputs, float** outputs, int32_t sampleFrames)
    {
        float* outLeft = outputs[0];
        float* outRight = outputs[1];
        int sampleIdx = 0;
        int frames;

        this->midiEventReadIndex = 0;

        // Keep the samples-since-pulse counter within the voice buffer
        if (this->samplesSincePulse >= this->voiceBufferSize)
            this->samplesSincePulse -= this->voiceBufferSize;
        if (this->samplesSincePulse < 0)
            this->samplesSincePulse += this->voiceBufferSize;

        // Control rate: MIDI, parameter smoothing, animation, LFO and voice synthesis
        frames = sampleFrames;
        while (--frames >= 0)
        {
            this->dispatchMidiEvents(sampleIdx, sampleFrames);

            // MIDI pitch bend controls the vowel
            if (this->vowelBendDirty == true)  // (== true: compiles like the original)
            {
                this->vowelBendDirty = false;
                this->vowelTarget = this->vowelBendMsb * 128 + this->vowelBendLsb;
                this->vowelDelta = this->vowelTarget - this->vowelCurrent;
                this->vowelStep = (float)(this->vowelDelta / this->smoothingFrames);
                this->vowelSmoothingFramesRemaining = this->smoothingFrames;
            }

            // Vowel from the singing pad
            if (this->padVowelDirty == true)  // (== true: compiles like the original)
            {
                this->padVowelDirty = false;
                this->vowelTarget = (long)(this->padVowel * kPitchScaleFactor);
                this->vowelDelta = this->vowelTarget - this->vowelCurrent;
                this->vowelStep = (float)(this->vowelDelta / this->smoothingFrames);
                this->vowelSmoothingFramesRemaining = this->smoothingFrames;
            }

            // Pitch from the singing pad: 36..48 (one octave)
            if (this->padPitchDirty == true)  // (== true: compiles like the original)
            {
                this->padPitchDirty = false;
                this->padPitchTarget = this->padPitch * 12.0f + 36.0f;
                this->padPitchDelta = this->padPitchTarget - this->padPitchCurrent;
                this->padPitchStep = this->padPitchDelta / this->smoothingFrames;
                this->padPitchSmoothingFramesRemaining = this->smoothingFrames;
            }

            // Parameter smoothing
            if (this->smoothStep >= this->smoothCounter)
            {
                this->smoothStep = 0;
                if (this->vowelSmoothingFramesRemaining > 0)
                {
                    this->vowelSmoothingFramesRemaining--;
                    this->vowelCurrent += (long)this->vowelStep;
                    this->curVowelValue = this->vowelCurrent * kPitchToFloatScale;
                    this->setParameterValue(SingingVerticalSliderParameterId, this->curVowelValue);
                }
                if (this->padPitchSmoothingFramesRemaining > 0)
                {
                    this->padPitchSmoothingFramesRemaining--;
                    this->padPitchCurrent = this->padPitchStep + this->padPitchCurrent;
                    this->padPitchSlider = (this->padPitchCurrent - 36.0f) * 0.083333336f;
                    this->setParameterValue(SingingHorizontalSliderParameterId, this->padPitchSlider);
                    this->notePitch = this->padPitchCurrent;
                }
            }

            if (this->isSinging)
            {
                this->idleAnimationSampleCounter = 0;
                if (this->needsMonkAnimationRefresh)
                {
                    this->monkSprite = (this->curVowelValue * 24.0f) * (1.0f / 30.0f) + 0.2f;
                    this->setParameterValue(MonkSpriteParameterId, this->monkSprite);
                    this->needsMonkAnimationRefresh = false;
                }

                // Portamento: glide the pitch towards its target
                if (this->isLegato)
                {
                    if (this->notePitch + 0.2f < this->glidePitch)
                        this->glideStep = -12.0f / ((this->portamentoTime + 0.01f) * this->pluginSampleRate);
                    else if (this->notePitch - 0.2f > this->glidePitch)
                        this->glideStep = 12.0f / ((this->portamentoTime + 0.01f) * this->pluginSampleRate);
                    else
                    {
                        this->glidePitch = this->notePitch;
                        this->glideStep = 0;
                    }
                    this->glidePitch = this->glidePitch + this->glideStep;
                }
                else
                {
                    this->glidePitch = this->notePitch;
                }

                // Vibrato (mod wheel): a sine LFO at a slightly random rate
                this->voicePitch = this->glidePitch;
                if (this->sineTableSize <= this->lfoPhaseAccumulator)
                    this->lfoPhaseAccumulator = this->lfoPhaseAccumulator - this->sineTableSize;
                if (this->sampleCounter >= this->lfoReseedIntervalSamples)
                {
                    this->sampleCounter = 0;
                    float random = this->getRandomFloat();
                    this->lfoRate = random + random + 5.0f;
                }
                this->vibratoOffset = (this->vibratoDepth + 0.2f) * this->sineTable[(long)this->lfoPhaseAccumulator];
                this->lfoPhaseAccumulator = (this->vibratoDepth * 0.2f + 1.0f) * this->lfoRate / this->lfoPhaseIncrement + this->lfoPhaseAccumulator;
                this->voicePitch = this->vibratoOffset + this->voicePitch;

                // Fundamental frequency and period of the current note
                this->voiceFrequency = this->frequencyTable[-(long)(this->voicePitch * -32.0f)];
                this->periodSamples = (long)(this->pluginSampleRate / this->voiceFrequency);

                // Start the next grain once a full pitch period has passed
                if (this->samplesSincePulse >= this->periodSamples || this->vowelBufferNeedsUpdate)
                {
                    if (this->vowelBufferNeedsUpdate)
                        this->synthesizeVowelBuffer(this->curVowelValue);
                    this->addGrainToVoice(this->samplesSincePulse);
                    this->samplesSincePulse = 0;
                    this->vowelBufferNeedsUpdate = false;
                }
            }
            else
            {
                // Idle: blink twice, then play the idle animation
                this->vowelBufferNeedsUpdate = true;
                if (this->idleAnimationSampleCounter == this->startBlink1)
                    this->setParameterValue(MonkSpriteParameterId, closeEyes);
                if (this->idleAnimationSampleCounter == this->stopBlink1)
                    this->setParameterValue(MonkSpriteParameterId, openEyes);
                if (this->idleAnimationSampleCounter == this->startBlink2)
                    this->setParameterValue(MonkSpriteParameterId, closeEyes);
                if (this->idleAnimationSampleCounter == this->stopBlink2)
                    this->setParameterValue(MonkSpriteParameterId, openEyes);

                if (this->globalAnimationSampleCounter >= this->idleSamplesPerFrame &&
                    this->idleAnimationSampleCounter >= this->startIdleAnimation)
                {
                    if (this->currentIdleFrame >= 24)
                        this->currentIdleFrame = 0;
                    this->globalAnimationSampleCounter = 0;
                    this->monkSprite = this->monkIdleFrameTable[this->currentIdleFrame];
                    this->setParameterValue(MonkSpriteParameterId, this->monkSprite);
                    this->idleAnimationSampleCounter = this->startIdleAnimation;
                    this->currentIdleFrame++;
                }
            }

            this->sampleCounter++;
            this->idleAnimationSampleCounter++;
            this->globalAnimationSampleCounter++;
            this->samplesSincePulse++;
            this->synthesisFrameCounter++;
            sampleIdx++;
            this->smoothStep++;
        }

        // Audio rate: stereo delay and output
        for (int i = 0; i < sampleFrames; i++)
        {
            while (this->voiceReadIndex >= this->voiceBufferSize)
                this->voiceReadIndex -= this->voiceBufferSize;

            int delaySize = this->delayBufferSize;
            while (this->delayWriteIndex >= delaySize)
                this->delayWriteIndex -= delaySize;
            while (this->delayWriteIndex < 0)
                this->delayWriteIndex += delaySize;
            while (this->delayReadIndexL >= delaySize)
                this->delayReadIndexL -= delaySize;
            while (this->delayReadIndexL < 0)
                this->delayReadIndexL += delaySize;
            while (this->delayReadIndexR >= delaySize)
                this->delayReadIndexR -= delaySize;
            while (this->delayReadIndexR < 0)
                this->delayReadIndexR += delaySize;

            this->stereoDelayLBuffer[this->delayWriteIndex] = (this->stereoDelayLBuffer[this->delayReadIndexL] * this->delayFeedback + this->voiceBuffer[this->voiceReadIndex]) * this->delay;
            this->stereoDelayRBuffer[this->delayWriteIndex] = (this->stereoDelayRBuffer[this->delayReadIndexR] * this->delayFeedback + this->voiceBuffer[this->voiceReadIndex]) * this->delay;
            this->delayWriteIndex++;

            // The output gain depends slightly on the pitch
            outLeft[i] = (this->voiceBuffer[this->voiceReadIndex] + this->stereoDelayLBuffer[this->delayReadIndexL]) * (((float)(this->glidePitch * -0.013888889f) + 2.0f) * this->outputGain);
            this->delayReadIndexL++;
            outRight[i] = (this->voiceBuffer[this->voiceReadIndex] + this->stereoDelayRBuffer[this->delayReadIndexR]) * (((float)(this->glidePitch * -0.013888889f) + 2.0f) * this->outputGain);
            this->delayReadIndexR++;

            this->voiceBuffer[this->voiceReadIndex] = 0;
            this->voiceReadIndex++;
        }
    }

    // FUNCTION: DELAYLAMA 0x10002db0
    void DelayLamaAudio::setParameterValue(int32_t parameterId, float value)
    {
        switch (parameterId)
        {
            case GlideKnobParameterId: // Portamento Time
            {
                this->portamentoTime = value;
                break;
            }
            case SingingHorizontalSliderParameterId: // Pitch indicator next to the pad
            {
                this->padPitchSlider = value;
                break;
            }
            case SingingVerticalSliderParameterId: // Vowel
            {
                this->curVowelValue = value;

                if (this->isSinging)
                {
                    const float monkSprite = (value * 24.0f) * (1.0f / 30.0f) + 0.2f;
                    this->monkSprite = monkSprite;

                    this->setParameterValue(MonkSpriteParameterId, monkSprite);

                    if (this->curVowelValue != this->prevVowelValue)
                    {
                        synthesizeVowelBuffer(this->curVowelValue);
                    }
                }

                this->prevVowelValue = this->curVowelValue;
                break;
            }
            case MonkSpriteParameterId: // Monk Sprite
            {
                if (!this->isSinging)
                {
                    this->monkSprite = value;
                }
                break;
            }
            case DelaySliderParameterId: // Delay
            {
                this->delay = value;
                break;
            }
            case VoiceKnobParameterId: // Head Size
            {
                this->headSize = value;

                if (this->isSinging)
                {
                    synthesizeVowelBuffer(this->curVowelValue);
                }
                break;
            }
            case SingingEnabledParameterId: // Note trigger
            {
                if (value != 0.0f)
                {
                    this->isSinging = true;
                    sendMidiToHost(kMidiNoteOn, 40, 64);
                }
                else
                {
                    if (this->noteStack[0] == 0)
                    {
                        this->isSinging = false;
                        this->setParameterValue(MonkSpriteParameterId, 0.1667f); // idle frame, as in monkIdleFrameTable

                        this->currentIdleFrame = 0;
                        this->needsMonkAnimationRefresh = true;

                        sendMidiToHost(kMidiNoteOff, 40, 64);
                    }
                }
                break;
            }
            case PadPitchParameterId: // MIDI CC
            {
                this->padPitchDirty = true;
                this->padPitch = value;

                const int midiValue = static_cast<int>(value * 127.0f);
                this->midiDataValue = midiValue;

                sendMidiToHost(kMidiControlChange, 11, midiValue);
                break;
            }
            case PadVowelParameterId: // Vowel from the pad, sent to the host as pitch bend
            {
                this->padVowelDirty = true;
                this->padVowel = value;

                const int midiValue = static_cast<int>(value * 127.0f);
                this->midiDataValue = midiValue;

                sendMidiToHost(kMidiPitchBend, 0, midiValue);
                break;
            }
            default:
                break;
        }
    }

    // FUNCTION: DELAYLAMA 0x100032c0
    void DelayLamaAudio::initPresets() {
        // Preset 0
        this->presets[0].portTime = 0.5f;
        this->presets[0].delay     = 0.8f;
        this->presets[0].headSize  = 0.5f;
        strcpy(this->presets[0].name, "Rabten");

        // Preset 1
        this->presets[1].portTime = 0.4f;
        this->presets[1].delay     = 0.3f;
        this->presets[1].headSize  = 0.0f;
        strcpy(this->presets[1].name, "Dorje");

        // Preset 2
        this->presets[2].portTime = 0.8f;
        this->presets[2].delay     = 0.6f;
        this->presets[2].headSize  = 0.25f;
        strcpy(this->presets[2].name, "Ngawang");

        // Preset 3
        this->presets[3].portTime = 0.5f;
        this->presets[3].delay     = 0.0f;
        this->presets[3].headSize  = 0.75f;
        strcpy(this->presets[3].name, "Jamyang");

        // Preset 4
        this->presets[4].portTime = 1.0f;
        this->presets[4].delay     = 0.9f;
        this->presets[4].headSize  = 1.0f;
        strcpy(this->presets[4].name, "Tinley");
    }

    // FUNCTION: DELAYLAMA 0x10002110
    bool DelayLamaAudio::sendEventsToHost(DamSDK::Api::DamMidiEventList* eventsPtr) {
        if (this->hostCallback != nullptr) {
            int32_t resultInt = this->hostCallback(&this->plugin, DamSDK::Api::hostProcessDeferredEvents, 0, 0, eventsPtr, 0.0);
            return (resultInt == 1);
        }
        return false;
    }

    // FUNCTION: DELAYLAMA 0x10002b10
    void DelayLamaAudio::loadPresetByIndex(int32_t currentProgram) {
        Utils::logf("DelayLamaAudio::loadPresetByIndex %d\n", currentProgram);
        Preset* presets = this->presets;
        this->currentPreset = currentProgram;
        
        float portTime = presets[currentProgram].portTime;
        this->portamentoTime = portTime;
        this->delay = presets[currentProgram].delay;
        this->headSize = presets[currentProgram].headSize;

        this->setParameterValue(GlideKnobParameterId, portTime);
        this->setParameterValue(DelaySliderParameterId, this->delay);
        this->setParameterValue(VoiceKnobParameterId, this->headSize);
    }

    // FUNCTION: DELAYLAMA 0x10002b80
    void DelayLamaAudio::setCurrentPresetName(char* newName) {
        strcpy(this->presets[this->currentPreset].name, newName);
    }

    // FUNCTION: DELAYLAMA 0x10002bc0
    void DelayLamaAudio::getCurrentPresetName(char* outText) {
        strcpy(outText, this->presets[this->currentPreset].name);
    }

    // FUNCTION: DELAYLAMA 0x10002c00
    void DelayLamaAudio::getParameterUnitLabel(int32_t parameterId, char* label) {
        switch (parameterId) {
            case 0: strcpy(label, " Hours  "); break;
            case 1: strcpy(label, " Vowel  "); break;
            case 2: strcpy(label, "   dB   "); break;
            case 3: strcpy(label, "   cm   "); break;
        }
    }

    // FUNCTION: DELAYLAMA 0x10002c90
    void DelayLamaAudio::getParameterValueString(int32_t parameterId, char* outText) {
        *outText = '\0';
        switch(parameterId) {
        case GlideKnobParameterId:
          this->formatFloatToString(this->portamentoTime * 1000.0f, outText);
          return;
        case SingingVerticalSliderParameterId:
          this->formatFloatToString(this->curVowelValue, outText);
          return;
        case DelaySliderParameterId:
            this->formatFloatAsDecibelString(this->delay, outText);
            return;
        case VoiceKnobParameterId:
            this->formatFloatToString(this->headSize * 30.0f, outText);
        }
    }

    // FUNCTION: DELAYLAMA 0x10002d20
    void DelayLamaAudio::getParameterName(int32_t parameterId, char* outBuffer) {
        const char* sourceString = nullptr;

        switch (parameterId) {
            case 0: strcpy(outBuffer, "PortTime"); return;
            case 1: strcpy(outBuffer, " Vowel  "); return;
            case 2: strcpy(outBuffer, " Delay "); return;
            case 3: strcpy(outBuffer, "HeadSize"); return;
            default: return;
        }
    }

    // FUNCTION: DELAYLAMA 0x10002fd0
    float DelayLamaAudio::getParameterValue(int32_t parameter) {
        float value = 0;
        switch(parameter) {
            case GlideKnobParameterId:
                value = this->portamentoTime;
                break;
            case SingingVerticalSliderParameterId:
                value = this->curVowelValue;
                break;
            case DelaySliderParameterId:
                value = this->delay;
                break;
            case VoiceKnobParameterId:
                value = this->headSize;
                break;
            case MonkSpriteParameterId:
                value = this->monkSprite;
                break;
            case SingingHorizontalSliderParameterId:
                value = this->padPitchSlider;
                break;
        }
        return value;
    }

    // FUNCTION: DELAYLAMA 0x10003050
    bool DelayLamaAudio::getOutputBusProperties(int32_t index, char* properties) {
        if ((int)index < 2) {
          int label = sprintf(properties, "Vstx %1d", index + 1);
          properties[0x40] = 3;
          properties[0x41] = '\0';
          properties[0x42] = '\0';
          properties[0x43] = '\0';
          return true;
        }
        return false;
    }

    // FUNCTION: DELAYLAMA 0x10003080
    bool DelayLamaAudio::getPresetNameByIndex(int32_t category, int32_t index, char* outText) {
        // Only valid for the 5 preset slots
        if (index >= 5) {
            return false;
        }

        // Copy the preset name string to the output buffer
        const char* presetName = this->presets[index].name;
        strcpy(outText, presetName);

        return true;
    }

    // FUNCTION: DELAYLAMA 0x100030d0
    bool DelayLamaAudio::copyPreset(int32_t destinationIndex) {
        // Only valid for the 5 preset slots
        if (destinationIndex >= 5) {
            return false;
        }

        // Copy all fields of the current preset to the destination slot
        this->presets[destinationIndex] = this->presets[this->currentPreset];

        return true;
    }

    // FUNCTION: DELAYLAMA 0x10003170
    bool DelayLamaAudio::getProductName(char* outText) {
        const char* src = "Virtual Singing Monk"; 
        ::strcpy(outText, src);
        return true;
    }

    // FUNCTION: DELAYLAMA 0x100031a0
    int32_t DelayLamaAudio::pluginSupports(char* target) {
        // 1: yes, -1: no (VST canDo)
        if (!strcmp(target, "receiveVstEvents"))
            return 1;
        if (!strcmp(target, "receiveVstMidiEvent"))
            return 1;
        if (!strcmp(target, "sendVstMidiEvent"))
            return 1;
        if (!strcmp(target, "sendVstEvents"))
            return 1;
        return -1;
    }

    // FUNCTION: DELAYLAMA 0x10004870
    void DelayLamaAudio::setSampleRate(float sampleRate) {
        this->sampleRate = sampleRate;
        this->pluginSampleRate = sampleRate;
    }

    // FUNCTION: DELAYLAMA 0x10004890
    void DelayLamaAudio::setMaxFramesPerProcess(int32_t blocksize) {
        this->blockSize = blocksize;
        this->pluginBlockSize = blocksize;
    }

    // FUNCTION: DELAYLAMA 0x100048a0
    void DelayLamaAudio::disableAudioProcessing() {
        return;
    }

    // FUNCTION: DELAYLAMA 0x100048b0
    void DelayLamaAudio::enableAudioProcessing() {
        Utils::log("DelayLamaAudio::enableAudioProcessing\n");
        this->requestMidiSupport(1);
        initialize();
    }

    // FUNCTION: DELAYLAMA 0x10005350
    void DelayLamaAudio::buildFormantCurveTable(int32_t* controlPoints, float* outSamples) {
        // Catmull-Rom spline through the 5 control points, 320 samples per segment.
        // The first and last points are repeated so every segment has neighbours.
        int32_t points[7];
        points[0] = controlPoints[0];
        points[1] = controlPoints[0];
        points[3] = controlPoints[2];
        points[2] = controlPoints[1];
        points[4] = controlPoints[3];
        points[5] = controlPoints[4];
        points[6] = controlPoints[4];

        int32_t* p = &points[1];
        int segment = 1;
        for (int end = 320; end < 1600; end += 320) {
            float start = (float)((segment - 1) * 320.0);
            int x = end - 320;
            if (x < end) {
                int p3 = p[2];
                int p1 = p[0];
                int p2 = p[1];
                int p0 = p[-1];
                float a = (float)((p1 - p2) * 3 - p0 + p3) * 0.5f;
                float b = (float)p2 + (float)p2 + p0 - (float)((p1 + p3 + p1 * 4) / 2);
                float c = (float)(p2 - p0) * 0.5f;
                float d = (float)p1;
                float* out = outSamples;
                do {
                    float t = ((float)x - start) * 0.003125f;
                    x++;
                    *out++ = ((t * a + b) * t + c) * t + d;
                } while (x < end);
            }
            segment++;
            p++;
            outSamples += 320;
        }
    }

    // FUNCTION: DELAYLAMA 0x100054a0
    void DelayLamaAudio::invokeAudioProcess(float* * inputs, float* * outputs, int32_t sampleFrames) {
        this->processAudio(inputs,outputs,sampleFrames);
    }

    // FUNCTION: DELAYLAMA 0x10005ca0
    void DelayLamaAudio::dispatchMidiEvents(int sampleIdx, int sampleFrame) {
        int pitchInterpCount = 0;

        if (this->midiQueue[this->midiEventReadIndex].timestamp == sampleIdx) {
            do {
                int status = this->midiQueue[this->midiEventReadIndex].status;
                if (status == 0)
                    break;
                status &= 0xf0;

                if (status == kMidiNoteOn || status == kMidiNoteOff) {
                    // Note on / note off
                    int note = this->midiQueue[this->midiEventReadIndex].data1 & 0x7f;
                    int velocity = this->midiQueue[this->midiEventReadIndex].data2 & 0x7f;
                    if (status == kMidiNoteOff)
                        velocity = 0;
                    this->handleNoteEvent(note, velocity);
                }
                else if (status == kMidiControlChange) {
                    // Control change
                    this->currentMidiEventData1 = this->midiQueue[this->midiEventReadIndex].data1 & 0x7f;
                    this->currentMidiEventData2 = this->midiQueue[this->midiEventReadIndex].data2 & 0x7f;
                    this->handleControlChange(this->currentMidiEventData1, this->currentMidiEventData2);
                }
                else if (status == kMidiPitchBend) {
                    // Pitch bend (controls the vowel)
                    if (sampleIdx != 0) {
                        this->vowelBendLsb = this->midiQueue[this->midiEventReadIndex].data1 & 0x7f;
                        this->vowelBendDirty = true;
                        this->vowelBendMsb = this->midiQueue[this->midiEventReadIndex].data2 & 0x7f;
                    }
                    else {
                        // Bends at the start of the buffer are spread across it below.
                        this->bendQueueLsb[pitchInterpCount] = this->midiQueue[this->midiEventReadIndex].data1 & 0x7f;
                        this->bendQueueMsb[pitchInterpCount] = this->midiQueue[this->midiEventReadIndex].data2 & 0x7f;
                        pitchInterpCount++;
                    }
                }

                // Clear the slot and move on
                this->midiQueue[this->midiEventReadIndex].timestamp = 0;
                this->midiQueue[this->midiEventReadIndex].status = 0;
                this->midiQueue[this->midiEventReadIndex].data1 = 0;
                this->midiQueue[this->midiEventReadIndex].data2 = 0;
                this->midiEventReadIndex++;
            } while (this->midiQueue[this->midiEventReadIndex].timestamp == sampleIdx);

            if (pitchInterpCount != 0) {
                this->nextBendSample = 1;
                this->bendQueueCount = pitchInterpCount;
                this->bendQueueIndex = 0;
                this->bendQueueSpacing = (sampleFrame - 2) / pitchInterpCount;
            }
        }

        // Pitch bends from the start of the buffer are applied one by one, spread
        // evenly across it (nextBendSample holds the sample of the next one)
        if (sampleIdx == this->nextBendSample && sampleIdx != 0 && this->bendQueueIndex < this->bendQueueCount) {
            this->vowelBendLsb = this->bendQueueLsb[this->bendQueueIndex];
            this->vowelBendMsb = this->bendQueueMsb[this->bendQueueIndex];
            this->bendQueueIndex++;
            this->vowelBendDirty = true;
            this->nextBendSample += this->bendQueueSpacing;
        }
    }

    // FUNCTION: DELAYLAMA 0x10005eb0
    void DelayLamaAudio::addGrainToVoice(int offsetIncrement) {
        // Overlap-add the grain into the voice ring buffer at the write position
        this->pulseWriteIndex += offsetIncrement;
        if (this->pulseWriteIndex + this->numSamples > this->voiceBufferSize || this->pulseWriteIndex < 0) {
            for (int i = 0; i < this->numSamples; i++) {
                while (this->pulseWriteIndex >= this->voiceBufferSize)
                    this->pulseWriteIndex -= this->voiceBufferSize;
                while (this->pulseWriteIndex < 0)
                    this->pulseWriteIndex += this->voiceBufferSize;
                this->voiceBuffer[this->pulseWriteIndex] += this->grainBuffer[i];
                this->pulseWriteIndex++;
            }
        } else {
            for (int i = 0; i < this->numSamples; i++) {
                this->voiceBuffer[this->pulseWriteIndex] += this->grainBuffer[i];
                this->pulseWriteIndex++;
            }
        }
        this->pulseWriteIndex -= this->numSamples;
    }

#define kTableIndexMax ((float)(1279.0f))
    
    // FUNCTION: DELAYLAMA 0x10005fb0
    void DelayLamaAudio::synthesizeVowelBuffer(float vowelX) {
        Utils::logf("DelayLamaAudio::synthesizeVowelBuffer vowelX=%f\n", vowelX);

        // Scale vowel position to table index range (0.0 .. 1279.0)
        float vowelIndex = vowelX * kTableIndexMax;
        this->vowelLookupIndex = vowelIndex;

        // resonanceGain widens/narrows all 3 formant rates together based on head size
        float resonanceGain = (float)(this->headSize * 0.5 + 0.75);
        this->headSizeScale = resonanceGain;

        // Convert vowel index to integer for the formant frequency table lookup
        int tableIndex = static_cast<int>(vowelIndex);

        // Per-formant frequency (Hz-ish) at this vowel position, converted to
        // formantWave-table steps per sample via formantWaveStepsPerHz (steps per Hz).
        // The original multiplies at x87 precision and rounds once to float; double
        // does the same with SSE2 compilers.
        float glotStep1 = (float)((double)resonanceGain * this->formantTable1[tableIndex] * this->formantWaveStepsPerHz);
        float glotStep2 = (float)((double)resonanceGain * this->formantTable2[tableIndex] * this->formantWaveStepsPerHz);
        float glotStep3 = (float)((double)resonanceGain * this->formantTable3[tableIndex] * this->formantWaveStepsPerHz);

        if (this->numSamples <= 0) {
            return;
        }

        // Wavetable phases: one per formant, wrap at formantWaveSize.
        // The original keeps all six phases in x87 registers for the whole loop; double
        // keeps the (int) lookups the same as the original with any compiler.
        double glotPhase1 = 0.0;
        double glotPhase2 = 0.0;
        double glotPhase3 = 0.0;

        // Formant envelope phases (index into the shared exponential decay/bandwidth table):
        double envPhase1 = 0.0;
        double envPhase2 = 0.0;
        double envPhase3 = 0.0;

        for (int i = 0; i < this->numSamples; ++i) {
            // Formant 1 (F1)
            this->grainBuffer[i] = this->formantWave[static_cast<int>(glotPhase1)] *
                                        this->decayTable[static_cast<int>(envPhase1)];
            envPhase1 += this->formant1Bandwidth;
            glotPhase1 += glotStep1;
            if (glotPhase1 >= this->formantWaveSize) {
                glotPhase1 -= this->formantWaveSize;
            }

            // Formant 2 (F2)
            this->grainBuffer[i] += this->formantWave[static_cast<int>(glotPhase2)] *
                                         this->decayTable[static_cast<int>(envPhase2)];
            envPhase2 += this->formant2Bandwidth;
            glotPhase2 += glotStep2;
            if (glotPhase2 >= this->formantWaveSize) {
                glotPhase2 -= this->formantWaveSize;
            }

            // Formant 3 (F3)
            this->grainBuffer[i] += this->formantWave[static_cast<int>(glotPhase3)] *
                                         this->decayTable[static_cast<int>(envPhase3)];
            envPhase3 += this->formant3Bandwidth;
            glotPhase3 += glotStep3;
            if (glotPhase3 >= this->formantWaveSize) {
                glotPhase3 -= this->formantWaveSize;
            }

            // Add the fixed formants (at half level) and apply the grain window
            this->grainBuffer[i] += this->fixedFormantBuffer[i] * 0.5;
            this->grainBuffer[i] *= this->grainWindow[i];
        }
    }

    // FUNCTION: DELAYLAMA 0x100061e0
    int32_t DelayLamaAudio::processEvents(void* events) {
        DamSDK::Api::DamEventList* eventList = (DamSDK::Api::DamEventList*)events;
        int queued = 0;

        // Queue the MIDI events; processAudio dispatches them at their sample offset.
        for (int i = 0; i < eventList->count; i++) {
            DamSDK::Api::DamMidiEvent* event = (DamSDK::Api::DamMidiEvent*)eventList->events[i];
            if (event->event.eventType == 1) {
                this->midiQueue[queued].timestamp = event->event.frames;
                this->midiQueue[queued].status = (char)event->midiData[0];
                this->midiQueue[queued].data1 = (char)event->midiData[1];
                this->midiQueue[queued].data2 = (char)event->midiData[2];
                queued++;
            }
        }
        return 1;
    }

    // FUNCTION: DELAYLAMA 0x10006240
    void DelayLamaAudio::handleNoteEvent(int midiData1, int midiData2) {
        // Notes are played one octave lower than received.
        int note;
        int i;

        midiData1 -= 12;
        note = midiData1;

        if (midiData2 != 0) {
            // Note on: push the note onto the front of the stack.
            if (note <= 72 && note > 3) {
                for (i = 127; i >= 0; i--) {
                    if (this->noteStack[127] != 0)
                        break;
                    if (this->noteStack[i] != 0)
                        this->noteStack[i + 1] = this->noteStack[i];
                }
                this->noteStack[0] = note;
            }
        }
        else {
            // Note off: remove the note from the stack.
            if (note <= 72 && note > 3) {
                for (i = 0; i <= 127; i++) {
                    if (this->noteStack[i] == note) {
                        while (this->noteStack[i] != 0) {
                            this->noteStack[i] = this->noteStack[i + 1];
                            i++;
                        }
                    }
                }
            }
        }

        int activeNote = this->noteStack[0];
        this->notePitch = (float)activeNote;
        this->isSinging = activeNote != 0;

        if (activeNote == 0) {
            this->isLegato = false;
            this->setParameterValue(MonkSpriteParameterId, 0.1667f);  // mouth closed
            this->currentIdleFrame = 0;
            this->needsMonkAnimationRefresh = true;
        }
        if (this->noteStack[1] != 0 && !this->isLegato)
            this->isLegato = true;
    }

    // FUNCTION: DELAYLAMA 0x10006330
    void DelayLamaAudio::handleControlChange(int midiData1, int midiData2) {
        Utils::logf("DelayLamaAudio::handleControlChange cc=%d value=%d\n", midiData1, midiData2);
        // Modulation Wheel
        if (midiData1 == 1) {
          this->vibratoDepth = (float)midiData2 * 0.007874016f;
          return;
        }

        // Portamento Time
        if (midiData1 == 5) {
          float portamento = (float)midiData2 * 0.007874016f;
          this->portamentoTime = portamento;
          this->setParameterValue(GlideKnobParameterId,portamento);
          return;
        }

        // Main Volume
        if (midiData1 == 7) {
          this->outputGain = (float)midiData2 * 0.001f;
          return;
        }

        // Pitch from the pad (the plugin sends it as controller 11)
        if (midiData1 == 0xb) {
          this->padPitchDirty = true;
          this->padPitch = (float)midiData2 * 0.007874016f;
          return;
        }

        // Delay Amount (Custom)
        if (midiData1 == 0xc) {
          float delay = (float)midiData2 * 0.007874016f;
          this->delay = delay;
          this->setParameterValue(DelaySliderParameterId, delay);
          return;
        }

        // Head Size (Custom)
        if (midiData1 == 0xd) {
          float headSize = (float)midiData2 * 0.007874016f;
          this->headSize = headSize;
          this->setParameterValue(VoiceKnobParameterId, headSize);
        }
        
    }

    // FUNCTION: DELAYLAMA 0x10006400
    float DelayLamaAudio::getRandomFloat() {
        this->rngState = this->rngState * 1664525 + 1013904223;
        return (float)this->rngState * this->rngScale;
    }

    // FUNCTION: DELAYLAMA 0x10006430
    void DelayLamaAudio::sendMidiToHost(uint8_t status, uint8_t data1, uint8_t data2) {
        Utils::logf("DelayLamaAudio::sendMidiToHost status=0x%02x data1=%d data2=%d\n", status, data1, data2);
        DamSDK::Api::DamMidiEventList* eventList = &this->midiEventList;
        DamSDK::Api::DamMidiEvent* damMidiEvent = &this->midiEvent;

        this->midiEventList.events[0] = &damMidiEvent->event;

        damMidiEvent->event.eventType = 1;
        this->midiEvent.midiData[0] = status;
        this->midiEvent.midiData[1] = data1;
        eventList->listSize = 1;
        this->midiEventList.events[1] = 0;
        this->midiEvent.event.eventSize = 24;
        this->midiEvent.event.frames = 0;
        this->midiEvent.midiData[2] = data2;

        sendEventsToHost(eventList);
    }
}
}