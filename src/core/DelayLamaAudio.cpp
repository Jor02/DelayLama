#include "DelayLamaAudio.h"
#include "PluginConfig.h"
#include "core/PluginConfig.h"
#include <cmath>

namespace DelayLama {
namespace Core {
    DelayLamaAudio::DelayLamaAudio(DamSDK::Api::dispatchFunc hostCallback) : DamSDK::Api::AudioBaseExtended(hostCallback, PRESET_COUNT, PARAMETER_COUNT) {
        this->plugin.inputChannelCount = 0;
        this->plugin.outputChannelCount = 2;
        this->setSupportsInPlaceProcessing(true);
        this->setReportsLoudnessToHost(false);
        this->setHasClip(false);
        this->setIsSynthesizer(true);
        this->plugin.id = 'AnDl';
    }

    DelayLamaAudio::~DelayLamaAudio() {}

    bool DelayLamaAudio::getPluginName(char *outText) {
        strcpy(outText, "Delay Lama");
        return true;
    }
    
    bool DelayLamaAudio::getCompanyName(char *outText) {
        strcpy(outText, "AudioNerdz");
        return true;
    }

    const int kExcitationBufferSize = 10240;
    const double kDelayTimeSeconds = 0.02;  // 20 milliseconds
    const double kPi2 = 6.283185307f;  // 2.0 * pi
    const double kPi50 = 157.0796327;  // 50.0 * pi

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
        if (this->harmonicBuffer == NULL) {
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

        if (this->sineTable == NULL) {
            this->sineTable = new float[this->sineTableSize];
        }

        for (i = 0; i < this->sineTableSize; ++i) {
            this->sineTable[i] = sinf(((float)i * 6.283185307f) / (float)this->sineTableSize);
        }
    }

    void DelayLamaAudio::processAudio(float** inputs, float** outputs, int32_t sampleFrames)
    {
        // int i;
    }

    void DelayLamaAudio::setParameterValue(int parameterId, float value)
    {
        switch (parameterId)
        {
            case LeftVoiceKnobParameterId: // Portamento Time
            {
                this->portamentoTime = value;
                break;
            }

            // case SingingVerticalSliderParameterId: // Vowel
            // {
            //     this->curVowelValue = value;

            //     if (this->isSinging)
            //     {
            //         // Simplifies: value * 24 * (1/30) + 0.2  => value * 0.8 + 0.2
            //         const float monkSprite = value * 0.8f + 0.2f;
            //         this->monkSprite = monkSprite;

            //         this->setParameterValue(ScaledVowelParameterId, monkSprite);

            //         if (this->curVowelValue != this->prevVowelValue)
            //         {
            //             updateVowelFilter(value);
            //         }
            //     }

            //     this->prevVowelValue = this->curVowelValue;
            //     break;
            // }

            case ReverbSliderParameterId: // Delay
            {
                this->delay = value;
                break;
            }

            // case RightGlideKnobParameterId: // Head Size
            // {
            //     this->headSize = value;

            //     if (this->isSinging)
            //     {
            //         updateVowelFilter(this->curVowelValue);
            //     }
            //     break;
            // }

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

            // case SingingEnabledParameterId: // Note trigger
            // {
            //     if (value != 0.0f)
            //     {
            //         this->isSinging = true;
            //         sendMidiToHost(0x90, 40, 64); // Note On
            //     }
            //     else
            //     {
            //         if (this->noteStack[0] == 0)
            //         {
            //             this->isSinging = false;

            //             // Magic constant preserved from original
            //             this->setParameterValue(6, 0.16670001f);

            //             this->vowelPresetIndex = 0;
            //             this->initialVowelNeedsUpdate = true;

            //             sendMidiToHost(0x80, 40, 64); // Note Off
            //         }
            //     }
            //     break;
            // }

            // case PitchValueParameterId: // Pitch Bend
            // {
            //     this->pitchValueDirty = true;
            //     this->pitchValue = value;

            //     // ftol() reconstruction: float [0,1] → MIDI [0,127]
            //     const int midiValue = static_cast<int>(value * 127.0f);
            //     this->midiDataValue = midiValue;

            //     sendMidiToHost(0xE0, 0, midiValue);
            //     break;
            // }

            // case VibratoAmountParameterId: // MIDI CC
            // {
            //     this->vibratoDirty = true;
            //     this->vibratoAmount = value;

            //     const int midiValue = static_cast<int>(value * 127.0f);
            //     this->midiDataValue = midiValue;

            //     sendMidiToHost(0xB0, 0x0B, midiValue);
            //     break;
            // }

            default:
                break;
        }
    }
}
}