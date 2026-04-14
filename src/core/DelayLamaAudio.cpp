#include "DelayLamaAudio.h"
#include "PluginConfig.h"
#include "core/PluginConfig.h"
#include <cmath>
#include "damsdk/utils/portable_stdint.h"
#include <cstdio>

namespace DelayLama {
namespace Core {

    // FUNCTION DELAYLAMA: 0x10002820
    DelayLamaAudio::DelayLamaAudio(DamSDK::Api::dispatchFunc hostCallback) : DamSDK::Api::AudioBaseExtended(hostCallback, PRESET_COUNT, PARAMETER_COUNT) {
        this->synthesisBuffer = nullptr;
        this->excitationBuffer = nullptr;
        this->formantTable = nullptr;
        this->glottalSource = nullptr;
        this->harmonicBuffer = nullptr;
        this->sineTable = nullptr;
        this->vocalEnvelope = nullptr;
        this->frequencyTable = nullptr;
        this->stereoDelayLBuffer = nullptr;
        this->stereoDelayRBuffer = nullptr;
        this->formantTable1 = nullptr;
        this->formantTable2 = nullptr;
        this->formantTable3 = nullptr;
        this->isSinging = false;

        this->presets = new Preset[5];

        if (this->presets != nullptr) {
            this->initPresets();
            this->loadPresetByIndex(0);
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
    }

    // FUNCTION DELAYLAMA: 0x10002980
    DelayLamaAudio::~DelayLamaAudio() {}

    // FUNCTION DELAYLAMA: 0x10003110
    bool DelayLamaAudio::getPluginName(char *outText) {
        strcpy(outText, "Delay Lama");
        return true;
    }
    
    // FUNCTION DELAYLAMA: 0x10003140
    bool DelayLamaAudio::getCompanyName(char *outText) {
        strcpy(outText, "AudioNerdz");
        return true;
    }

    const int kPitchBendCenter = 8192; 
    const int kExcitationBufferSize = 10240;
    const double kDelayTimeSeconds = 0.02;  // 20 milliseconds
    const double kPi = 3.141592654f;  // pi
    const double kPi2 = 6.283185307f;  // 2.0 * pi
    const double kPi50 = 157.0796327;  // 50.0 * pi
    const double kMidiNote0Frequency = 8.175798916; 
    const double kAttackTime  = 0.0018;
    const double kSustainTime = 0.013;
    const double kReleaseTime = 0.007;
    const float kPitchToFloatScale = 1.0f / 16384.0f;

    // FUNCTION: DELAYLAMA 0x100048d0
    void DelayLamaAudio::initialize() {
        int i; // For loop index

        double currentSampleRate = this->getSampleRate();
        this->pluginSampleRate = (float)currentSampleRate;
        if (currentSampleRate != this->prevSampleRate) {
            if (this->synthesisBuffer != nullptr) {
                delete this->synthesisBuffer;
            }
            if (this->excitationBuffer != nullptr) {
                delete this->excitationBuffer;
            }
            if (this->sineTable != nullptr) {
                delete this->sineTable;
            }
            if (this->formantTable != nullptr) {
                delete this->formantTable;
            }
            if (this->vocalEnvelope != nullptr) {
                delete this->vocalEnvelope;
            }
            if (this->glottalSource != nullptr) {
                delete this->glottalSource;
            }
            if (this->harmonicBuffer != nullptr) {
                delete this->harmonicBuffer;
            }
            if (this->frequencyTable != nullptr) {
                delete this->frequencyTable;
            }
            if (this->stereoDelayLBuffer != nullptr) {
                delete this->stereoDelayLBuffer;
            }
            if (this->stereoDelayRBuffer != nullptr) {
                delete this->stereoDelayRBuffer;
            }
            if (this->formantTable1 != nullptr) {
                delete this->formantTable1;
            }
            if (this->formantTable2 != nullptr) {
                delete this->formantTable2;
            }
            if (this->formantTable3 != nullptr) {
                delete this->formantTable3;
            }

            // Nullify all dangling pointers
            this->synthesisBuffer = nullptr;
            this->excitationBuffer = nullptr;
            this->formantTable = nullptr;
            this->glottalSource = nullptr;
            this->harmonicBuffer = nullptr;
            this->sineTable = nullptr;
            this->vocalEnvelope = nullptr;
            this->frequencyTable = nullptr;
            this->stereoDelayLBuffer = nullptr;
            this->stereoDelayRBuffer = nullptr;
            this->formantTable1 = nullptr;
            this->formantTable2 = nullptr;
            this->formantTable3 = nullptr;
        }
        this->prevSampleRate = this->pluginSampleRate;

        // Allocate and Initialize Synthesis Buffers
        this->numSamples = static_cast<long>(this->pluginSampleRate * kDelayTimeSeconds);
        if (this->synthesisBuffer == nullptr) {
            this->synthesisBuffer = new float[this->numSamples];
        }

        this->excitationBufferSize = kExcitationBufferSize;
        if (this->excitationBuffer == nullptr) {
            this->excitationBuffer = new float[this->excitationBufferSize];
        }

        // Clear excitation buffer
        for (i = 0; i < this->excitationBufferSize; ++i) {
            this->excitationBuffer[i] = 0.0f;
        }

        // Generate Formant Table
        this->formantTableSize = this->numSamples << 2;
        if (this->formantTable == nullptr) {
            this->formantTable = new float[this->formantTableSize];
        }

        float decayFactor = (float)(kPi50 / this->pluginSampleRate);
        int tableSize = this->formantTableSize;

        if (this->formantTable != nullptr) {
            for (i = 0; i < tableSize; ++i) {
                this->formantTable[i] = (float)::exp(-i * decayFactor);
            }
        }

        // Generate Glottal Source Table (Sine math)
        this->glottalTableSize = 1024;
        if (this->glottalSource == nullptr) {
            this->glottalSource = new float[this->glottalTableSize];
        }

        for (i = 0; i < this->glottalTableSize; ++i) {
            double value = sin((i * kPi2) / (double)this->glottalTableSize);
            this->glottalSource[i] = (float)value;
        }

        this->glottalPhaseInc = (float)this->glottalTableSize / (float)this->pluginSampleRate;
        
        // Generate Harmonic Buffer
        if (this->harmonicBuffer == nullptr) {
            this->harmonicBuffer = new float[this->numSamples];
        }

        for (i = 0; i < this->numSamples; ++i) {
            // Frequency 1: 4950 Hz
            float phase1 = (float)(i * kPi2 * 4950.0 / (double)this->pluginSampleRate);
            float s1 = (float)sin(phase1);
            
            // Frequency 2: 3800 Hz
            float phase2 = (float)(i * kPi2 * 3800.0f / (double)this->pluginSampleRate);
            float s2 = (float)sin(phase2);
            
            float sample = s1 * this->formantTable[i];
            sample += s2 * this->formantTable[i];

            this->harmonicBuffer[i] = sample;
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
         if (this->vocalEnvelope == nullptr) {
            this->vocalEnvelope = new float[this->numSamples];
        }

        this->attackSamples  = static_cast<long>(this->pluginSampleRate * kAttackTime);
        this->sustainStart   = static_cast<long>(this->pluginSampleRate * kSustainTime);
        this->releaseSamples = static_cast<long>(this->pluginSampleRate * kReleaseTime);

        // Fill Envelope with 1.0 (0x3f800000)
        for (i = 0; i < this->numSamples; ++i) {
            this->vocalEnvelope[i] = 1.0f;
        }
        
        // Apply Attack Phase (Cosine shaping)
        const int attackSamples = this->attackSamples;
        if (attackSamples > 0) {
            for (i = 0; i < attackSamples; ++i) {
                double phase = (static_cast<double>(i) * kPi) / static_cast<double>(attackSamples);
                float value = static_cast<float>(0.5 * (1.0 - cos(phase)));
                this->vocalEnvelope[i] = value;
            }
        }
        // Apply Release Phase (Cosine shaping)
        int relLoopIdx = this->sustainStart;
        int nextIndex = 0;
        if (relLoopIdx < this->numSamples) {
            do {
                int tempRelSamples = this->releaseSamples;
                nextIndex = relLoopIdx + 1;
                double phase = (static_cast<double>(i) * kPi) / static_cast<double>(releaseSamples);
                float value = static_cast<float>(0.5 * (1.0 + cos(phase)));
                this->vocalEnvelope[relLoopIdx] = value;
                relLoopIdx = nextIndex;
            } while (nextIndex < this->numSamples);
        }
        
        // Pitch/Frequency Lookup Table
        this->frequencyTableSize = 4096;
        if (this->frequencyTable == nullptr) {
            this->frequencyTable = new float[this->frequencyTableSize];
        }

        // MIDI to frequency conversion: f = 8.175798916 * 2^(i / 12)
        for (i = 0; i < this->frequencyTableSize; ++i) {
            double exponent = static_cast<double>(i) / 12.0;
            this->frequencyTable[i] = static_cast<float>(kMidiNote0Frequency * pow(2.0, exponent));
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
        buildPiecewiseCubicTable(formantControlPoints, this->formantTable1);

        if (this->formantTable2 == nullptr) {
            this->formantTable2 = new float[1280];
        }
        buildPiecewiseCubicTable(formantControlPoints + 5, this->formantTable2);

        if (this->formantTable3 == nullptr) {
            this->formantTable3 = new float[1280];
        }
        buildPiecewiseCubicTable(formantControlPoints + 10, this->formantTable3);

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
        this->vowelPresetTable[0] = 0.1667f;
        this->vowelPresetTable[1] = 0.1f;
        this->vowelPresetTable[2] = 0.1333f;
        this->vowelPresetTable[3] = 0.1f;
        this->vowelPresetTable[4] = 0.0667f;
        this->vowelPresetTable[5] = 0.0333f;
        this->vowelPresetTable[6] = 0.0f;
        this->vowelPresetTable[7] = 0.0333f;
        this->vowelPresetTable[8] = 0.1667f;
        this->vowelPresetTable[9] = 0.1f;
        this->vowelPresetTable[10] = 0.1333f;
        this->vowelPresetTable[11] = 0.1f;
        this->vowelPresetTable[12] = 0.1667f;
        this->vowelPresetTable[13] = 0.0333f;
        this->vowelPresetTable[14] = 0.0f;
        this->vowelPresetTable[15] = 0.0333f;
        this->vowelPresetTable[16] = 0.0667f;
        this->vowelPresetTable[17] = 0.1f;
        this->vowelPresetTable[18] = 0.1333f;
        this->vowelPresetTable[19] = 0.1f;
        this->vowelPresetTable[20] = 0.1667f;
        this->vowelPresetTable[21] = 0.0333f;
        this->vowelPresetTable[22] = 0.0f;
        this->vowelPresetTable[23] = 0.0333f;

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
        this->pitchBase = 64;
        this->pitchTargetRaw = 64;
        this->vowelTargetValue = 0.5f;
        this->pitchTargetDirty = false;
        this->isGlideActive = false;
        this->isSinging = false;
        this->vowelFilterDirty = true;
        this->currentMidiEventData2 = 0;
        this->currentMidiEventData1 = 1;
        this->outputGain = 0.1f;
        this->pitchValueDirty = false;
        this->vibratoDirty = false;

        updateVowelFilter(0.5f);

        this->prevVowelValue = 0.5;
        this->isGateActive = false;
        this->vowelMorphCurrent = 36.0;
        this->lfoPhaseWrapValue = 4.0;
        this->lfoDepth = 0.0;
        this->lfoSampleValue = 0.0;
        this->lfoPhaseAccumulator = 0.0;

        // Vowel timing/frame math
        this->vowelFrameCounter = (int)(this->sampleRate * 104.0 * 0.001); // Effectively sampleRate * 0.104
        this->initialVowelNeedsUpdate = true;
        this->anotherVowelIndex = 0;
        this->vowelStepIndex = 0;
        this->vowelPresetIndex = 0;

        // Vowel Trigger Envelope/Sequence Steps
        int vowelTriggerBase = (int)(this->sampleRate * 0.208);
        this->vowelTriggerBase = vowelTriggerBase;
        this->vowelTriggerA = vowelTriggerBase * 7;
        this->vowelTriggerB = (int)(vowelTriggerBase * 8.5);
        this->vowelTriggerC = vowelTriggerBase * 15;
        this->vowelTriggerD = vowelTriggerBase * 17;
        this->vowelTriggerE = vowelTriggerBase * 23;

        this->writeIndex = 0;
        this->excitationReadIndex = 0;
        this->excitationWriteIndex = 0;
        this->sampleCounter = 0;

        // Delay Line setup
        this->delayWriteIndex = 0;
        this->delayTimeScaler = (float)(1.0 / pow(2.0, 32.0)); 
        this->delayReadIndexL = (int)(this->sampleRate * -0.309592);
        this->delayReadIndexR = (int)(this->sampleRate * -0.398435);
        this->delayFeedback = 0.5;

        // Smoothing configuration
        this->totalSmoothingFrames = (int)(this->sampleRate * 0.001 * 100.0); // Effectively sampleRate * 0.1
        this->smoothCounter = (int)(this->sampleRate * 0.0099999998);         // Effectively sampleRate * 0.01
        this->smoothingFrames = this->totalSmoothingFrames / this->smoothCounter;

        this->pitchCurrent = kPitchBendCenter; // Assuming this is 8192 (0x2000) based on '00 20 00 00'
        this->vibratoCurrent = 36.0;
        this->smoothStep = 0;
        this->pitchSmoothingFramesRemaining = 0;
        this->vibratoSmoothingFramesRemaining = 0;
    }

    const float kPitchScaleFactor = 16384.0f;

    // FUNCTION DELAYLAMA: 0x100054c0
    void DelayLamaAudio::processAudio(float** inputs, float** outputs, int32_t sampleFrames)
    {
        float *outRight = outputs[1];
        float *outLeft = outputs[0];
        this->midiEventReadIndex = 0;

        // Ensure the write index for the internal excitation buffer stays within bounds
        int bufferWriteIndex = this->excitationWriteIndex;
        int excitationBufferSizeCheck = this->excitationBufferSize;
        int sampleIdx = 0;

        // Wrap index down if it exceeds the buffer size
        if (excitationBufferSizeCheck <= bufferWriteIndex)
        {
            this->excitationWriteIndex = bufferWriteIndex - excitationBufferSizeCheck;
        }

        // Wrap index up if it goes below zero
        int bufferWriteIndexCheck = this->excitationWriteIndex;
        if (bufferWriteIndexCheck < 0)
        {
            this->excitationWriteIndex =
                bufferWriteIndexCheck + this->excitationBufferSize;
        }

        // Control Rate Loop (MIDI, Envelopes, LFOs, Vowel Morphing)
        int frame;
        if (-1 < sampleFrames + -1)
        {
            frame = sampleFrames;
            do
            {
                // Check for MIDI Note On/Off/CC at this specific sample offset
                this->dispatchMidiEvents(sampleIdx, sampleFrames);

                // Pitch Smoothing Trigger
                if (this->pitchTargetDirty == true)
                {
                    this->pitchTargetDirty = false;
                    
                    int targetPitch = this->pitchTargetRaw * 128 + this->pitchBase;
                    this->pitchTarget = targetPitch;
                    
                    int pitchDeltaVal = targetPitch - this->pitchCurrent;
                    this->pitchDelta = pitchDeltaVal;
                    this->pitchStep = (int)(float)(pitchDeltaVal / this->smoothingFrames);
                    this->pitchSmoothingFramesRemaining = this->smoothingFrames;
                }

                // Alternative Pitch Smoothing (likely from Pitch Bend)
                if (this->pitchValueDirty == true)
                {
                    this->pitchValueDirty = false;
                    int tempPitchTarget = static_cast<int>(this->pitchTargetRaw * 16384.0f);
                    this->pitchTarget = tempPitchTarget;
                    int tempPitchDelta = tempPitchTarget - this->pitchCurrent;
                    this->pitchDelta = tempPitchDelta;
                    this->pitchStep = static_cast<int>(static_cast<float>(tempPitchDelta) / static_cast<float>(this->smoothingFrames));
                    this->pitchSmoothingFramesRemaining = this->smoothingFrames;
                }

                // Vibrato Smoothing Trigger
                if (this->vibratoDirty == true)
                {
                    this->vibratoDirty = false;
                    float tempVowelVal = this->vibratoAmount * 12.0f + 36.0f;
                    this->vibratoTarget = (int)tempVowelVal;
                    tempVowelVal = tempVowelVal - this->vibratoCurrent;
                    this->vibratoDelta = (int)tempVowelVal;
                    this->vibratoStep = (int)(tempVowelVal / (float)this->smoothingFrames);
                    this->vibratoSmoothingFramesRemaining = this->smoothingFrames;
                }
                // Apply Parameter Smoothing
                if (this->smoothCounter <= this->smoothStep)
                {
                    this->smoothStep = 0;
                    int pitchStepsRemaining = this->pitchSmoothingFramesRemaining;
                    if (pitchStepsRemaining > 0) {
                        --this->pitchSmoothingFramesRemaining;

                        // Add the per‑sample pitch step (converted from float to integer)
                        long step = static_cast<long>(this->pitchStep);
                        this->pitchCurrent += step;

                        // Convert the internal fixed‑point pitch back to a float (1.0 / 16384.0)
                        float currentPitchFloat = static_cast<float>(this->pitchCurrent) * kPitchToFloatScale;
                        this->pitchCurrent = static_cast<int>(currentPitchFloat);

                        // Notify host of parameter change (likely via VST's setParameterAutomated)
                        this->setParameterValue(SingingVerticalSliderParameterId, currentPitchFloat);
                    }

                    int vibratoStepsRemaining = this->vibratoSmoothingFramesRemaining;
                    if (0 < vibratoStepsRemaining)
                    {
                        this->vibratoSmoothingFramesRemaining = vibratoStepsRemaining + -1;
                        
                        float nextVibratoValue = (float)this->vibratoStep + this->vibratoCurrent;
                        this->vibratoCurrent = nextVibratoValue;
                        
                        float normalizedVibrato = (nextVibratoValue - 36.0f) * 0.083333336f;
                        this->vibratoDepthCurrent = normalizedVibrato;
                        this->setParameterValue(SingingHorizontalSliderParameterId, normalizedVibrato);
                        this->pitchTargetValue = (int)this->vibratoCurrent;
                    }
                }

                // Idle Vowel Logic
                if (this->isSinging == false)
                {
                    this->vowelFilterDirty = true;
                    if (this->vowelStepIndex == this->vowelTriggerA)
                    {
                        this->setParameterValue(MonkParameterId, 0.06667f);
                    }
                    if (this->vowelStepIndex == this->vowelTriggerB)
                    {
                        this->setParameterValue(MonkParameterId, 0.16667f);
                    }
                    if (this->vowelStepIndex == this->vowelTriggerC)
                    {
                        this->setParameterValue(MonkParameterId, 0.06667f);
                    }
                    if (this->vowelStepIndex == this->vowelTriggerD)
                    {
                        this->setParameterValue(MonkParameterId, 0.16667f);
                    }

                    // cycle through vowels?
                    if ((this->vowelTriggerBase <= this->anotherVowelIndex) && (this->vowelTriggerE <= this->vowelStepIndex))
                    {
                        if (23 < this->vowelPresetIndex)
                        {
                            this->vowelPresetIndex = 0;
                        }
                        this->anotherVowelIndex = 0;
                        float targetMonkSprite = this->vowelPresetTable[this->vowelPresetIndex];
                        this->monkSprite = targetMonkSprite;
                        this->setParameterValue(MonkParameterId, targetMonkSprite);
                        this->vowelStepIndex = this->vowelTriggerE;
                        this->vowelPresetIndex = this->vowelPresetIndex + 1;
                    }
                }
                else
                {
                    // Singing Vowel Logic
                    this->vowelStepIndex = 0;
                    if (this->initialVowelNeedsUpdate != false)
                    {
                        float tempVowelVal = this->curVowelValue * 24.0f * 0.033333335f + 0.2f;
                        this->monkSprite = tempVowelVal;
                        this->setParameterValue(MonkParameterId, tempVowelVal);
                        this->initialVowelNeedsUpdate = false;
                    }
                    // Portamento/Glide for the Vowel Filter
                    if (this->isGateActive == false)
                    {
                        this->vowelMorph = (float)this->pitchTargetValue;
                    }
                    else
                    {
                        // Calculate glide delta based on Portamento Time and Sample Rate
                        if (this->vowelMorph <= (float)this->pitchTargetValue + 0.2)
                        {
                            if ((float)this->pitchTargetValue - 0.2 <= this->vowelMorph)
                            {
                                this->vowelMorph = (float)this->pitchTargetValue;
                                this->vowelMorphDelta = 0;
                            }
                            else
                            {
                                this->vowelMorphDelta = (int)(12.0 / ((this->portamentoTime + 0.01) * this->pluginSampleRate));
                            }
                        }
                        else
                        {
                            this->vowelMorphDelta = (int)(-12.0 / ((this->portamentoTime + 0.01) * this->pluginSampleRate));
                        }
                        this->vowelMorph =
                            this->vowelMorph + (float)this->vowelMorphDelta;
                    }

                    // LFO Calculation (Frequency Wobble)
                    this->vowelMorphCurrent = this->vowelMorph;
                    int sineSize = this->sineTableSize;
                    if (sineSize <= this->lfoPhaseAccumulator)
                    {
                        this->lfoPhaseAccumulator = this->lfoPhaseAccumulator - sineSize;
                    }
                    if (this->vowelFrameCounter <= this->sampleCounter)
                    {
                        this->sampleCounter = 0;
                        float random = getRandomFloat();
                        this->lfoPhaseWrapValue = random + random + 5.0f;
                    }

                    int lfoSineIndex = static_cast<long>(this->lfoPhaseAccumulator);
                    this->lfoSampleValue = (this->lfoDepth + 0.2f) * (float)this->sineTable[lfoSineIndex];
                    this->lfoPhaseAccumulator = ((this->lfoDepth * 0.2f + 1.0f) * this->lfoPhaseWrapValue) / this->lfoPhaseIncrement + this->lfoPhaseAccumulator;
                    this->vowelMorphCurrent = this->lfoSampleValue + this->vowelMorphCurrent;
                    
                    // Fetch fundamental frequency for synthesis
                    int freqTableIndex =  static_cast<long>(this->vowelMorphCurrent * -32.0f);
                    float fundamentalFreq = this->frequencyTable[freqTableIndex];
                    this->frequencyValue = fundamentalFreq;
                    
                    int writeBoundary = static_cast<long>(this->pluginSampleRate / fundamentalFreq);
                    this->frequencyIndex = writeBoundary;
                    
                    // Perform the actual synthesis if filter is dirty or boundary reached
                    if ((writeBoundary <= this->excitationWriteIndex) || (this->vowelFilterDirty != false))
                    {
                        if (this->vowelFilterDirty != false)
                        {
                            updateVowelFilter(this->curVowelValue);
                        }
                        addSynthesisToExcitation(this->excitationWriteIndex);
                        this->excitationWriteIndex = 0;
                        this->vowelFilterDirty = false;
                    }
                }

                // Increment per-sample counters
                this->sampleCounter = this->sampleCounter + 1;
                this->vowelStepIndex = this->vowelStepIndex + 1;
                this->excitationWriteIndex = this->excitationWriteIndex + 1;
                this->synthesisFrameCounter = this->synthesisFrameCounter + 1;
                sampleIdx += 1;
                frame--;
                this->anotherVowelIndex = this->anotherVowelIndex + 1;
                this->smoothStep = this->smoothStep + 1;

            } while (frame != 0);
        }

        // Audio Rate Loop (Final Mix, Delay, and Output)
        if (0 < sampleFrames)
        {
            int frame = sampleFrames;
            float* outPtr = outRight;
            do
            {
                // Circular Buffer Wrapping for Excitation and Delay
                excitationBufferSize = this->excitationBufferSize;
                int wrappedExcitReadIdx = this->excitationReadIndex;
                while (excitationBufferSize <= wrappedExcitReadIdx)
                {
                    wrappedExcitReadIdx = this->excitationReadIndex - excitationBufferSize;
                    this->excitationReadIndex = wrappedExcitReadIdx;
                }
                
                delayBufferSize = this->delayBufferSize;
                int wrappedDelayWriteIdx = this->delayWriteIndex;
                while (delayBufferSize <= wrappedDelayWriteIdx)
                {
                    wrappedDelayWriteIdx = this->delayWriteIndex - delayBufferSize;
                    this->delayWriteIndex = wrappedDelayWriteIdx;
                }
                
                int wrappedDelayWriteIdxNeg = this->delayWriteIndex;
                while (wrappedDelayWriteIdxNeg < 0)
                {
                    wrappedDelayWriteIdxNeg = this->delayWriteIndex + delayBufferSize;
                    this->delayWriteIndex = wrappedDelayWriteIdxNeg;
                }
                
                int wrappedDelayReadLIdx = this->delayReadIndexL;
                while (delayBufferSize <= wrappedDelayReadLIdx)
                {
                    wrappedDelayReadLIdx = this->delayReadIndexL - delayBufferSize;
                    this->delayReadIndexL = wrappedDelayReadLIdx;
                }
                
                int wrappedDelayReadLIdxNeg = this->delayReadIndexL;
                while (wrappedDelayReadLIdxNeg < 0)
                {
                    wrappedDelayReadLIdxNeg = this->delayReadIndexL + delayBufferSize;
                    this->delayReadIndexL = wrappedDelayReadLIdxNeg;
                }
                
                int wrappedDelayReadRIdx = this->delayReadIndexR;
                while (delayBufferSize <= wrappedDelayReadRIdx)
                {
                    wrappedDelayReadRIdx = this->delayReadIndexR - delayBufferSize;
                    this->delayReadIndexR = wrappedDelayReadRIdx;
                }
                
                int wrappedDelayReadRIdxNeg = this->delayReadIndexR;
                while (wrappedDelayReadRIdxNeg < 0)
                {
                    wrappedDelayReadRIdxNeg = this->delayReadIndexR + delayBufferSize;
                    this->delayReadIndexR = wrappedDelayReadRIdxNeg;
                }

                // Read current excitation value and clear it (prep for next accumulation)
                float excitation = this->excitationBuffer[this->excitationReadIndex];

                // Stereo Delay Line (Feedback Loop)
                float* stereoDelayLBuffer = this->stereoDelayLBuffer;
                stereoDelayLBuffer[this->delayWriteIndex] = (stereoDelayLBuffer[this->delayReadIndexL] * this->delayFeedback + excitation) * this->delay;
                
                float* stereoDelayRBuffer = this->stereoDelayRBuffer;
                stereoDelayRBuffer[this->delayWriteIndex] = (stereoDelayRBuffer[this->delayReadIndexR] * this->delayFeedback + excitation) * this->delay;
                
                this->delayWriteIndex = this->delayWriteIndex + 1;
                
                // Final Output Mix & Volume Normalization
                // Volume is adjusted slightly depending on the mouth position/vowel.
                float morphScale = (this->vowelMorph * -0.013888889f + 2.0f) * this->outputGain;
                
                // Left Channel: Dry Excitation + Delay Line L
                outLeft[frame] = morphScale * (excitation + this->stereoDelayLBuffer[this->delayReadIndexL]);
                
                // Right Channel: Dry Excitation + Delay Line R
                outRight[frame] = morphScale * (excitation + this->stereoDelayRBuffer[this->delayReadIndexR]);
                
                this->excitationBuffer[this->excitationReadIndex] = 0.0f;
                this->excitationReadIndex = this->excitationReadIndex + 1;
                
                outPtr = outPtr + 1;
                frame--;
            } while (frame != 0);
        }
    }

    // FUNCTION DELAYLAMA: 0x10002db0
    void DelayLamaAudio::setParameterValue(int parameterId, float value)
    {
        switch (parameterId)
        {
            case LeftVoiceKnobParameterId: // Portamento Time
            {
                this->portamentoTime = value;
                break;
            }
            case SingingVerticalSliderParameterId: // Vowel
            {
                this->curVowelValue = value;

                if (this->isSinging)
                {
                    const float monkSprite = value * 24.0f * 0.033333335f + 0.2f;
                    this->monkSprite = monkSprite;

                    this->setParameterValue(MonkParameterId, monkSprite);

                    if (this->curVowelValue != this->prevVowelValue)
                    {
                        updateVowelFilter(value);
                    }
                }

                this->prevVowelValue = this->curVowelValue;
                break;
            }
            case ReverbSliderParameterId: // Delay
            {
                this->delay = value;
                break;
            }
            case RightGlideKnobParameterId: // Head Size
            {
                this->headSize = value;

                if (this->isSinging)
                {
                    updateVowelFilter(this->curVowelValue);
                }
                break;
            }
            case SingingHorizontalSliderParameterId: // Vibrato Depth
            {
                this->vibratoDepthCurrent = value;
                break;
            }
            case MonkParameterId: // Scaled Vowel
            {
                if (!this->isSinging)
                {
                    this->monkSprite = value;
                }
                break;
            }
            case SingingEnabledParameterId: // Note trigger
            {
                if (value != 0.0f)
                {
                    this->isSinging = true;
                    sendMidiToHost(0x90, 40, 64); // Note On
                }
                else
                {
                    if (this->noteStack[0] == 0)
                    {
                        this->isSinging = false;

                        // Magic constant preserved from original
                        this->setParameterValue(6, 0.16670001f);

                        this->vowelPresetIndex = 0;
                        this->initialVowelNeedsUpdate = true;

                        sendMidiToHost(0x80, 40, 64); // Note Off
                    }
                }
                break;
            }
            case PitchValueParameterId: // Pitch Bend
            {
                this->pitchValueDirty = true;
                this->pitchValue = value;

                const int midiValue = static_cast<int>(value * 127.0f);
                this->midiDataValue = midiValue;

                sendMidiToHost(0xE0, 0, midiValue);
                break;
            }
            case VibratoAmountParameterId: // MIDI CC
            {
                this->vibratoDirty = true;
                this->vibratoAmount = value;

                const int midiValue = static_cast<int>(value * 127.0f);
                this->midiDataValue = midiValue;

                sendMidiToHost(0xB0, 0x0B, midiValue);
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

    // STUB: DELAYLAMA 0x100029a0
    void DelayLamaAudio::destroy() {

        if (this->presets != nullptr) {
            delete[] this->presets;
            this->presets = nullptr;
        }

        // Delete all dynamically allocated float buffers
        if (this->synthesisBuffer != nullptr) {
            delete[] this->synthesisBuffer;
            this->synthesisBuffer = nullptr;
        }
        if (this->excitationBuffer != nullptr) {
            delete[] this->excitationBuffer;
            this->excitationBuffer = nullptr;
        }
        if (this->sineTable != nullptr) {
            delete[] this->sineTable;
            this->sineTable = nullptr;
        }
        if (this->formantTable != nullptr) {
            delete[] this->formantTable;
            this->formantTable = nullptr;
        }
        if (this->vocalEnvelope != nullptr) {
            delete[] this->vocalEnvelope;
            this->vocalEnvelope = nullptr;
        }
        if (this->glottalSource != nullptr) {
            delete[] this->glottalSource;
            this->glottalSource = nullptr;
        }
        if (this->harmonicBuffer != nullptr) {
            delete[] this->harmonicBuffer;
            this->harmonicBuffer = nullptr;
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
        AudioBaseExtended::destroy();
    }

    // FUNCTION: DELAYLAMA 0x10002b10
    void DelayLamaAudio::loadPresetByIndex(int currentProgram) {
        Preset* presets = this->presets;
        this->currentPreset = currentProgram;
        
        float portTime = presets[currentProgram].portTime;
        this->portamentoTime = portTime;
        this->delay = presets[currentProgram].delay;
        this->headSize = presets[currentProgram].headSize;

        this->setParameterValue(LeftVoiceKnobParameterId, portTime);
        this->setParameterValue(ReverbSliderParameterId, this->delay);
        this->setParameterValue(RightGlideKnobParameterId, this->headSize);
    }

    // STUB: DELAYLAMA 0x10002b80
    void DelayLamaAudio::setCurrentPresetName(char* newName) {
        // char cVar1;
        // uint uVar2;
        // uint uVar3;
        // char *pcVar4;
        // char *pcVar5;
        //
        // uVar2 = 0xffffffff;
        // do {
        //   pcVar4 = newName;
        //   if (uVar2 == 0) break;
        //   uVar2 = uVar2 - 1;
        //   pcVar4 = newName + 1;
        //   cVar1 = *newName;
        //   newName = pcVar4;
        // } while (cVar1 != '\0');
        // uVar2 = ~uVar2;
        // pcVar4 = pcVar4 + -uVar2;
        // pcVar5 = (*this->presets)
        //          [this->currentPreset].name;
        // for (uVar3 = uVar2 >> 2; uVar3 != 0; uVar3 = uVar3 - 1) {
        //   *(undefined4 *)pcVar5 = *(undefined4 *)pcVar4;
        //   pcVar4 = pcVar4 + 4;
        //   pcVar5 = pcVar5 + 4;
        // }
        // for (uVar2 = uVar2 & 3; uVar2 != 0; uVar2 = uVar2 - 1) {
        //   *pcVar5 = *pcVar4;
        //   pcVar4 = pcVar4 + 1;
        //   pcVar5 = pcVar5 + 1;
        // }
        // return;
    }

    // STUB: DELAYLAMA 0x10002bc0
    void DelayLamaAudio::getCurrentPresetName(char* outText) {
        // char cVar1;
        // uint uVar2;
        // uint uVar3;
        // char *pcVar4;
        // char *pcVar5;
        //
        // uVar2 = 0xffffffff;
        // pcVar4 = (*this->presets)
        //          [this->currentPreset].name;
        // do {
        //   pcVar5 = pcVar4;
        //   if (uVar2 == 0) break;
        //   uVar2 = uVar2 - 1;
        //   pcVar5 = pcVar4 + 1;
        //   cVar1 = *pcVar4;
        //   pcVar4 = pcVar5;
        // } while (cVar1 != '\0');
        // uVar2 = ~uVar2;
        // pcVar4 = pcVar5 + -uVar2;
        // for (uVar3 = uVar2 >> 2; uVar3 != 0; uVar3 = uVar3 - 1) {
        //   *(undefined4 *)outText = *(undefined4 *)pcVar4;
        //   pcVar4 = pcVar4 + 4;
        //   outText = outText + 4;
        // }
        // for (uVar2 = uVar2 & 3; uVar2 != 0; uVar2 = uVar2 - 1) {
        //   *outText = *pcVar4;
        //   pcVar4 = pcVar4 + 1;
        //   outText = outText + 1;
        // }
        // return;
    }

    // STUB: DELAYLAMA 0x10002c00
    void DelayLamaAudio::getParameterUnitLabel(char* label) {
        // char cVar1;
        // uint uVar2;
        // uint uVar3;
        // char *pcVar4;
        // char *pcVar5;
        //
        // switch(this) {
        // case (DelayLamaAudio *)0x0:
        //   pcVar4 = s_Hours_1000d2fc;
        //   goto LAB_10002c25;
        // case (DelayLamaAudio *)0x1:
        //   pcVar4 = s_Vowel_1000d2f0;
        //   break;
        // case (DelayLamaAudio *)0x2:
        //   pcVar4 = &DAT_1000d2e4;
        // LAB_10002c25:
        //   uVar2 = 0xffffffff;
        //   do {
        //     pcVar5 = pcVar4;
        //     if (uVar2 == 0) break;
        //     uVar2 = uVar2 - 1;
        //     pcVar5 = pcVar4 + 1;
        //     cVar1 = *pcVar4;
        //     pcVar4 = pcVar5;
        //   } while (cVar1 != '\0');
        //   uVar2 = ~uVar2;
        //   pcVar4 = pcVar5 + -uVar2;
        //   for (uVar3 = uVar2 >> 2; uVar3 != 0; uVar3 = uVar3 - 1) {
        //     *(undefined4 *)label = *(undefined4 *)pcVar4;
        //     pcVar4 = pcVar4 + 4;
        //     label = label + 4;
        //   }
        //   for (uVar2 = uVar2 & 3; uVar2 != 0; uVar2 = uVar2 - 1) {
        //     *label = *pcVar4;
        //     pcVar4 = pcVar4 + 1;
        //     label = label + 1;
        //   }
        //   return;
        // case (DelayLamaAudio *)0x3:
        //   pcVar4 = &DAT_1000d2d8;
        //   break;
        // default:
        //   goto LAB_10002c6d;
        // }
        // uVar2 = 0xffffffff;
        // do {
        //   pcVar5 = pcVar4;
        //   if (uVar2 == 0) break;
        //   uVar2 = uVar2 - 1;
        //   pcVar5 = pcVar4 + 1;
        //   cVar1 = *pcVar4;
        //   pcVar4 = pcVar5;
        // } while (cVar1 != '\0');
        // uVar2 = ~uVar2;
        // pcVar4 = pcVar5 + -uVar2;
        // for (uVar3 = uVar2 >> 2; uVar3 != 0; uVar3 = uVar3 - 1) {
        //   *(undefined4 *)label = *(undefined4 *)pcVar4;
        //   pcVar4 = pcVar4 + 4;
        //   label = label + 4;
        // }
        // for (uVar2 = uVar2 & 3; uVar2 != 0; uVar2 = uVar2 - 1) {
        //   *label = *pcVar4;
        //   pcVar4 = pcVar4 + 1;
        //   label = label + 1;
        // }
        // LAB_10002c6d:
        // return;
    }

    // FUNCTION: DELAYLAMA 0x10002c90
    void DelayLamaAudio::getParameterValueString(int parameterId, char* outText) {
        *outText = '\0';
        switch(parameterId) {
        case LeftVoiceKnobParameterId:
          this->formatFloatToString(this->portamentoTime * 1000.0f, outText);
          return;
        case SingingVerticalSliderParameterId:
          this->formatFloatToString(this->curVowelValue, outText);
          return;
        case ReverbSliderParameterId:
            this->formatFloatAsDecibelString(this->delay, outText);
            return;
        case RightGlideKnobParameterId:
            this->formatFloatToString(this->headSize * 30.0f, outText);
        }
    }

    // FUNCTION: DELAYLAMA 0x10002d20
    void DelayLamaAudio::getParameterName(int parameterId, char* outBuffer) {
        const char* sourceString = nullptr;

        switch (parameterId) {
            case 0: strcpy(outBuffer, "PortTime"); return;
            case 1: strcpy(outBuffer, "Vowel"); return;
            case 2: strcpy(outBuffer, "Delay"); return;
            case 3: strcpy(outBuffer, "HeadSize"); return;
            default: return;
        }
    }

    // FUNCTION: DELAYLAMA 0x10002fd0
    float DelayLamaAudio::getParameterValue(int parameter) {
        switch(parameter) {
            case LeftVoiceKnobParameterId:
                return this->portamentoTime;
            case SingingVerticalSliderParameterId:
                return this->curVowelValue;
            case ReverbSliderParameterId:
                return this->delay;
            case RightGlideKnobParameterId:
                return this->headSize;
            case SingingHorizontalSliderParameterId:
                return this->vibratoDepthCurrent;
            case MonkParameterId:
                return this->monkSprite;
            default:
                return 0.0;
        }
    }

    // FUNCTION: DELAYLAMA 0x10003050
    bool DelayLamaAudio::getOutputBusProperties(uint32_t index, char* properties) {
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
    bool DelayLamaAudio::getPresetNameByIndex(int category, int index, char* outText) {
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
    bool DelayLamaAudio::copyPreset(int destinationIndex) {
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

    // FUNCTION: DELAYLAMA 0x10004870
    void DelayLamaAudio::setSampleRate(float sampleRate) {
        this->sampleRate = sampleRate;
        this->pluginSampleRate = sampleRate;
    }

    // FUNCTION: DELAYLAMA 0x10004890
    void DelayLamaAudio::setMaxFramesPerProcess(uint32_t blocksize) {
        this->blockSize = blocksize;
        this->pluginBlockSize = blocksize;
    }

    // FUNCTION: DELAYLAMA 0x100048a0
    void DelayLamaAudio::disableAudioProcessing() {
        return;
    }

    // FUNCTION: DELAYLAMA 0x100048b0
    void DelayLamaAudio::enableAudioProcessing() {
        this->requestMidiSupport(1);
        initialize();
    }

    // FUNCTION: DELAYLAMA 0x10005350
    void DelayLamaAudio::buildPiecewiseCubicTable(int32_t *controlPoints,float *outSamples)
    {
        float *outPtr;
        int x;
        int32_t *window;
        int segmentEnd;
        int32_t controlWindow [6];
        int p0;
        int p1;
        int p2;
        int pMinus1;
        float t;
        
        controlWindow[0] = *controlPoints;
        controlWindow[2] = controlPoints[2];
        controlWindow[1] = controlPoints[1];
        controlWindow[3] = controlPoints[3];
        controlWindow[4] = controlPoints[4];
        controlWindow[5] = controlPoints[4];
        controlPoints = (int32_t *)0x1;
        segmentEnd = 320;
        window = controlWindow;
        do {
            x = segmentEnd + -320;
            if (x < segmentEnd) {
                // next point
                p2 = window[2];
                // current point
                p1 = *window;
                // previous point
                p0 = window[1];
                // point before previous
                pMinus1 = window[-1];
                outPtr = outSamples;
                do {
                    t = (float)x;
                    x += 1;
                // Normalize t to [0, 1] within segment
                    t = (t - (float)((int)controlPoints + -1) * 320.0f) * 0.003125f;
                // Cubic interpolation
                    *outPtr = ((t * (float)(((p1 - p0) * 3 - pMinus1) + p2) * 0.5f +
                            (((float)p0 + (float)p0 + (float)pMinus1) - (float)((p1 + p2 + p1 * 4) / 2))) * t
                            + (float)(p0 - pMinus1) * 0.5f) * t + (float)p1;
                    outPtr = outPtr + 1;
                } while (x < segmentEnd);
            }
            segmentEnd += 320;
            // segment index++
            controlPoints = (int32_t *)((int)controlPoints + 1);
            // slide control window
            window = window + 1;
            // next segment (320 samples)
            outSamples = outSamples + 320;
        } while (segmentEnd < 1600);
        return;
    }

    // FUNCTION: DELAYLAMA 0x100054a0
    void DelayLamaAudio::invokeAudioProcess(float* * inputs, float* * outputs, int32_t sampleFrames) {
        this->processAudio(inputs,outputs,sampleFrames);
    }

    // STUB: DELAYLAMA 0x10005ca0
    void DelayLamaAudio::dispatchMidiEvents(int sampleIdx, int sampleFrame) {
        // int midiData2_;
        // uint midiData2;
        // int midiData1;
        // uint midiCommand;
        // int pitchInterpCount;
        // int *pitchInterpQueue;
        // uint statusByte;
        // uint bendValue;
        // int currentReadPtr;
        // int nextInterpSample;
        // int currentInterpIdx;
        // int currentMidiEvent;
        //
        // pitchInterpCount = 0;
        // if (this->midiQueue[this->midiEventReadIndex].timestamp == sampleIdx) {
        //   pitchInterpQueue = this->pitchInterpData2;
        //   do {
        //     currentMidiEvent = this->midiEventReadIndex;
        //     statusByte = this->midiQueue[currentMidiEvent].status;
        //     if (statusByte == 0) break;
        //     midiCommand = statusByte & 0xf0;
        //                   /* Handle note on (0x90) / Note off (0x80) */
        //     if ((midiCommand == 0x90) || (midiCommand == 0x80)) {
        //       midiData2 = this->midiQueue[currentMidiEvent].data2 & 0x7f;
        //       if (midiCommand == 0x80) {
        //         midiData2 = 0;
        //       }
        //       handleNoteEvent(this,this->midiQueue[currentMidiEvent].data1 & 0x7f,midiData2);
        //     }
        //     else {
        //                   /* Handle control change (0xB0) */
        //       if (midiCommand == 0xb0) {
        //         midiData1 = this->midiQueue[currentMidiEvent].data1 & 0x7f;
        //         this->currentMidiEventData1 = midiData1;
        //         midiData2_ = this->midiQueue[currentMidiEvent].data2 & 0x7f;
        //         this->currentMidiEventData2 = midiData2_;
        //         handleControlChange(this,midiData1,midiData2_);
        //       }
        //       else {
        //                   /* Handle pitch bend (0xE0) */
        //         if (midiCommand == 0xe0) {
        //                   /* If the bend happens at the very start of the buffer (sample 0), the plugin
        //                      sets up an interpolation routine to smooth the pitch change. */
        //           if (sampleIdx == 0) {
        //                   /* Store data in a temporary "interpolation queue" to be spread across the
        //                      buffer */
        //             pitchInterpQueue[-0x400] = this->midiQueue[currentMidiEvent].data1 & 0x7f;
        //             pitchInterpCount = pitchInterpCount + 1;
        //             *pitchInterpQueue =
        //                  this->midiQueue[this->midiEventReadIndex].data2 & 0x7f;
        //             pitchInterpQueue = pitchInterpQueue + 1;
        //           }
        //           else {
        //             this->pitchBase = this->midiQueue[currentMidiEvent].data1 & 0x7f;
        //             bendValue = this->midiQueue[currentMidiEvent].data2;
        //             this->pitchTargetDirty = true;
        //             this->pitchTargetRaw = bendValue & 0x7f;
        //           }
        //         }
        //       }
        //     }
        //                   /* Wipe the queue slot so it isn't processed twice */
        //     this->midiQueue[this->midiEventReadIndex].timestamp = 0;
        //     this->midiQueue[this->midiEventReadIndex].status = 0;
        //     this->midiQueue[this->midiEventReadIndex].data1 = 0;
        //     this->midiQueue[this->midiEventReadIndex].data2 = 0;
        //                   /* Advance the read pointer */
        //     currentReadPtr = this->midiEventReadIndex;
        //     this->midiEventReadIndex = currentReadPtr + 1;
        //   } while (this->midiQueue[currentReadPtr + 1].timestamp == sampleIdx);
        //                   /* If multiple pitch bend events occurred at sample 0, calculate how many
        //                      samples to wait between each update to spread them evenly across the buffer
        //                      (sampleFrameCount). */
        //   if (pitchInterpCount != 0) {
        //     this->isInterpActive = 1;
        //     this->interpEventCount = pitchInterpCount;
        //     this->interpCurrentIdx = 0;
        //     this->interpSampleStep = (sampleFrame + -2) / pitchInterpCount;
        //   }
        // }
        //                   /* If the interpolation logic was triggered above, this block executes at
        //                      specific intervals throughout the buffer processing to update the pitch
        //                      incrementally. */
        // nextInterpSample = this->isInterpActive;
        // if (((sampleIdx == nextInterpSample) && (sampleIdx != 0)) &&
        //    (currentInterpIdx = this->interpCurrentIdx,
        //    currentInterpIdx < this->interpEventCount)) {
        //                   /* Apply the next piece of pitch data from the interpolation queue */
        //   this->pitchBase = this->pitchInterpData1[currentInterpIdx];
        //   pitchInterpCount = this->pitchInterpData2[currentInterpIdx];
        //   this->interpCurrentIdx = currentInterpIdx + 1;
        //   this->pitchTargetRaw = pitchInterpCount;
        //   this->pitchTargetDirty = true;
        //                   /* Set the timestamp for the next interpolation step */
        //   this->isInterpActive = this->interpSampleStep + nextInterpSample;
        // }
        // return;
    }

    // FUNCTION: DELAYLAMA 0x10005eb0
    void DelayLamaAudio::addSynthesisToExcitation(int offsetIncrement) {
        int i = this->writeIndex + offsetIncrement;
        this->writeIndex = i;
        int bufSize = this->numSamples;
        if ((this->excitationBufferSize < bufSize + i) || (i < 0)) {
          i = 0;
          if (0 < bufSize) {
            do {
              bufSize = this->excitationBufferSize;
              int tmpIdx = this->writeIndex;
              while (bufSize <= tmpIdx) {
                tmpIdx = this->writeIndex - bufSize;
                this->writeIndex = tmpIdx;
              }
              tmpIdx = this->writeIndex;
              while (tmpIdx < 0) {
                tmpIdx = this->writeIndex + bufSize;
                this->writeIndex = tmpIdx;
              }
              float* dstPtr = &this->excitationBuffer[this->writeIndex];
              *dstPtr += this->synthesisBuffer[i];
              i++;
              this->writeIndex++;
            } while (i < this->numSamples);
          }
        }
        else {
          i = 0;
          if (0 < bufSize) {
            do {
              float* dstPtr = &this->excitationBuffer[this->writeIndex];
              *dstPtr += this->synthesisBuffer[i];
              i = i + 1;
              this->writeIndex = this->writeIndex + 1;
            } while (i < this->numSamples);
          }
        }
        this->writeIndex = this->writeIndex - this->numSamples;
        return;
    }

    const float kTableIndexMax = 1279.0f;
    
    // FUNCTION: DELAYLAMA 0x10005fb0
    void DelayLamaAudio::updateVowelFilter(float vowelX) {
        // Scale vowel position to table index range (0.0 .. 1279.0)
        float vowelIndex = vowelX * kTableIndexMax;
        this->vowelLookupIndex = vowelIndex;

        // Compute a blend factor from vowelMorphCurrent (offset 0xc4)
        // blend = vowelMorphCurrent * 0.5 + 0.75
        float blendFactor = static_cast<float>(this->vowelMorphCurrent * 0.5 + 0.75);
        this->vowelBlendFactor = blendFactor;  // offset 0x63b0

        // Convert vowel index to integer for table lookup
        long tableIndex = static_cast<long>(vowelIndex);

        // Retrieve formant gains from the three tables (each scaled by formantGain)
        float phaseInc1 = blendFactor * this->formantTable1[tableIndex] * this->glottalPhaseInc;
        float phaseInc2 = blendFactor * this->formantTable2[tableIndex] * this->glottalPhaseInc;
        float phaseInc3 = blendFactor * this->formantTable3[tableIndex] * this->glottalPhaseInc;

        // Processing Loop
        if (this->numSamples > 0) {

            // The 6 initialized 0.0 floats pushed to the FPU stack at the start
            float phase1 = 0.0f;
            float phase2 = 0.0f;
            float phase3 = 0.0f;
            float glotPhase1 = 0.0f;
            float glotPhase2 = 0.0f;
            float glotPhase3 = 0.0f;
            
            float limit = static_cast<float>(this->glottalTableSize);

            for (int i = 0; i < this->numSamples; ++i) {
                
                // Formant 1
                this->synthesisBuffer[i] = this->formantTable[static_cast<int>(phase1)] * this->glottalSource[static_cast<int>(glotPhase1)];
                glotPhase1 += this->formant1Bandwidth;
                phase1 += phaseInc1;
                if (phase1 >= limit) {
                    phase1 -= limit;
                }

                // Formant 2
                this->synthesisBuffer[i] += this->formantTable[static_cast<int>(phase2)] * this->glottalSource[static_cast<int>(glotPhase2)];
                glotPhase2 += this->formant2Bandwidth;
                phase2 += phaseInc2;
                if (phase2 >= limit) {
                    phase2 -= limit;
                }

                // Formant 3
                this->synthesisBuffer[i] += this->formantTable[static_cast<int>(phase3)] * this->glottalSource[static_cast<int>(glotPhase3)];
                glotPhase3 += this->formant3Bandwidth;
                phase3 += phaseInc3;
                if (phase3 >= limit) {
                    phase3 -= limit;
                }

                // Final Mixing & Enveloping
                this->synthesisBuffer[i] += this->harmonicBuffer[i] * 0.5f;
                this->synthesisBuffer[i] *= this->vocalEnvelope[i];
            }
        }
    }

    // FUNCTION: DELAYLAMA 0x100061e0
    void DelayLamaAudio::processEvents(DamSDK::Api::DamEventList* eventList) {
        if (eventList == nullptr || eventList->count <= 0) {
            return;
        }

        int writeIndex = 0;

        for (int i = 0; i < eventList->count; ++i) {
            const DamSDK::Api::DamEvent& evt = eventList->events[i];

            // Only process MIDI events
            if (evt.eventType == 1) {
                DamSDK::Api::MidiEvent& outEvent = this->midiQueue[writeIndex];

                outEvent.timestamp = evt.frames;
                outEvent.status   = (evt.flags >> 16) & 0xFF;
                outEvent.data1    = (evt.flags >> 8) & 0xFF;
                outEvent.data2    = evt.eventSize;

                ++writeIndex;
            }
        }
    }

    // STUB: DELAYLAMA 0x10006240
    void DelayLamaAudio::handleNoteEvent(int midiData1, int midiData2) {
        // int *piVar1;
        // int *nextNote;
        // int i;
        // int noteWithOffset;
        // int *noteStackPtr;
        // int activeNote;
        // int currentNote;
        // DelayLamaAudioVTable *vtable;
        //
        //                   /* Apply a -12 offset (one octave) to incoming MIDI notes */
        // noteWithOffset = midiData1 + -0xc;
        //                   /* Note off */
        // if (midiData2 == 0) {
        //   if ((noteWithOffset < 0x49) && (3 < noteWithOffset)) {
        //     i = 0;
        //     do {
        //       currentNote = this->noteStack[i];
        //       nextNote = this->noteStack + i;
        //       if (currentNote == noteWithOffset) {
        //         while (currentNote != 0) {
        //           i = i + 1;
        //           *nextNote = nextNote[1];
        //           piVar1 = nextNote + 1;
        //           nextNote = nextNote + 1;
        //           currentNote = *piVar1;
        //         }
        //       }
        //       i = i + 1;
        //     } while (i < 128);
        //   }
        // }
        // else {
        //                   /* Note on */
        //   if ((noteWithOffset < 73) && (3 < noteWithOffset)) {
        //     noteStackPtr = this->noteStack + 127;
        //     i = 127;
        //     nextNote = noteStackPtr;
        //     do {
        //       if (*noteStackPtr != 0) break;
        //       if (*nextNote != 0) {
        //         nextNote[1] = *nextNote;
        //       }
        //       i = i + -1;
        //       nextNote = nextNote + -1;
        //     } while (-1 < i);
        //     this->noteStack[0] = noteWithOffset;
        //   }
        // }
        // activeNote = this->noteStack[0];
        // this->pitchTargetValue = (int)(float)activeNote;
        // this->isSinging = activeNote != 0;
        // if (activeNote == 0) {
        //   vtable = this->vtable;
        //   this->isGateActive = false;
        //   (*(vtable->audioEffectX).audioEffect.setParameterValue)(6,1042985832);
        //   this->vowelPresetIndex = 0;
        //   this->initialVowelNeedsUpdate = true;
        // }
        // if ((this->noteStack[1] != 0) && (this->isGateActive == false)) {
        //   this->isGateActive = true;
        // }
        // return;
    }

    // FUNCTION: DELAYLAMA 0x10006330
    void DelayLamaAudio::handleControlChange(int midiData1, int midiData2) {
        // Modulation Wheel
        if (midiData1 == 1) {
          this->lfoDepth = (float)midiData2 * 0.007874016f;
          return;
        }

        // Portamento Time
        if (midiData1 == 5) {
          float portamento = (float)midiData2 * 0.007874016f;
          this->portamentoTime = portamento;
          this->setParameterValue(LeftVoiceKnobParameterId,portamento);
          return;
        }

        // Main Volume
        if (midiData1 == 7) {
          this->outputGain = (float)midiData2 * 0.001f;
          return;
        }

        // Expression
        if (midiData1 == 0xb) {
          this->vibratoDirty = true;
          this->vibratoAmount = (float)midiData2 * 0.007874016f;
          return;
        }

        // Delay Amount (Custom)
        if (midiData1 == 0xc) {
          float delay = (float)midiData2 * 0.007874016f;
          this->delay = delay;
          this->setParameterValue(ReverbSliderParameterId, delay);
          return;
        }

        // Head Size (Custom)
        if (midiData1 == 0xd) {
          float headSize = (float)midiData2 * 0.007874016f;
          this->headSize = headSize;
          this->setParameterValue(RightGlideKnobParameterId, headSize);
        }
    }

    // FUNCTION: DELAYLAMA 0x10006400
    float DelayLamaAudio::getRandomFloat() {
        this->rngState = this->rngState * 1664525 + 1013904223;
        return (float)this->rngState * this->delayTimeScaler;
    }

    // FUNCTION: DELAYLAMA 0x10006430
    void DelayLamaAudio::sendMidiToHost(uint8_t status, uint8_t data1, uint8_t data2) {        
        
        DamSDK::Api::DamMidiEvent* damMidiEvent = &this->midiEvent;
        DamSDK::Api::DamMidiEventList* eventList = &this->midiEventList;

        this->midiEventList.events[0] = &damMidiEvent->event;

        damMidiEvent->event.eventType = 1;
        this->midiEvent.midiData[0] = status;
        this->midiEvent.midiData[1] = data1;
        eventList->listSize = 1;
        this->unknownMidi = 0;
        this->midiEvent.event.eventSize = 24;
        this->midiEvent.event.frames = 0;
        this->midiEvent.midiData[2] = data2;

        sendEventsToHost(eventList);
    }
}
}