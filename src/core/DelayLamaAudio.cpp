#include "DelayLamaAudio.h"
#include "PluginConfig.h"
#include "core/PluginConfig.h"
#include <cmath>
#include "utils/portable_stdint.h"

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

        Preset* presets = new Preset[5];

        if (presets != nullptr) {
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

    const int kExcitationBufferSize = 10240;
    const double kDelayTimeSeconds = 0.02;  // 20 milliseconds
    const double kPi2 = 6.283185307f;  // 2.0 * pi
    const double kPi50 = 157.0796327;  // 50.0 * pi

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

    // FUNCTION DELAYLAMA: 0x100054c0
    void DelayLamaAudio::processAudio(float** inputs, float** outputs, int32_t sampleFrames)
    {
        // int i;
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

    // FUNCTION DELAYLAMA: 0x100032c0
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

    // STUB: DELAYLAMA 0x10002110
    bool DelayLamaAudio::sendEventsToHost(void* eventsPtr) {
        // int32_t *piVar1;
        // HostCallback hostCallback;
        //
        // hostCallback = this->audioEffectXMembers.audioEffectMembers.hostCallback;
        // if (hostCallback != (HostCallback)0x0) {
        //   piVar1 = (*hostCallback)(&this->audioEffectXMembers.audioEffectMembers.plugin,
        //                            hostProcessDeferredEvents,0,0,eventsPtr,0.0);
        //   return (bool)('\x01' - (piVar1 != (int32_t *)0x1));
        // }
        return false;
    }

    // STUB: DELAYLAMA 0x100029a0
    void DelayLamaAudio::destroy() {
        // float *pfVar1;
        // Preset (*presetArray) [5];
        // float **tempBufferPtr;
        // int **tempRawPtr;
        //
        // this->vtable = &vftable_1000b460_1000b460;
        // presetArray = this->presets;
        // if (presetArray != (Preset (*) [5])0x0) {
        //   __vector_destructor_iterator
        //             ((int)presetArray,0x24,*(int *)(presetArray[-1][4].name + 0x14),EditorBase::generic18)
        //   ;
        //   operator_delete(presetArray[-1][4].name + 0x14);
        // }
        // tempBufferPtr = this->synthesisBuffer;
        // if (tempBufferPtr != (float **)0x0) {
        //   operator_delete(tempBufferPtr);
        // }
        // tempRawPtr = this->excitationBuffer;
        // if (tempRawPtr != (int **)0x0) {
        //   operator_delete(tempRawPtr);
        // }
        // tempBufferPtr = this->sineTable;
        // if (tempBufferPtr != (float **)0x0) {
        //   operator_delete(tempBufferPtr);
        // }
        // tempBufferPtr = this->formantTable;
        // if (tempBufferPtr != (float **)0x0) {
        //   operator_delete(tempBufferPtr);
        // }
        // tempBufferPtr = this->vocalEnvelope;
        // if (tempBufferPtr != (float **)0x0) {
        //   operator_delete(tempBufferPtr);
        // }
        // tempBufferPtr = this->glottalSource;
        // if (tempBufferPtr != (float **)0x0) {
        //   operator_delete(tempBufferPtr);
        // }
        // tempBufferPtr = this->harmonicBuffer;
        // if (tempBufferPtr != (float **)0x0) {
        //   operator_delete(tempBufferPtr);
        // }
        // pfVar1 = this->frequencyTable;
        // if (pfVar1 != (float *)0x0) {
        //   operator_delete(pfVar1);
        // }
        // tempRawPtr = this->stereoDelayLBuffer;
        // if (tempRawPtr != (int **)0x0) {
        //   operator_delete(tempRawPtr);
        // }
        // tempRawPtr = this->stereoDelayRBuffer;
        // if (tempRawPtr != (int **)0x0) {
        //   operator_delete(tempRawPtr);
        // }
        // pfVar1 = this->formantTable1;
        // if (pfVar1 != (float *)0x0) {
        //   operator_delete(pfVar1);
        // }
        // pfVar1 = this->formantTable2;
        // if (pfVar1 != (float *)0x0) {
        //   operator_delete(pfVar1);
        // }
        // pfVar1 = this->formantTable3;
        // if (pfVar1 != (float *)0x0) {
        //   operator_delete(pfVar1);
        // }
        // AudioBaseExtended::destroy((AudioBaseExtended *)this);
        // return;
    }

    // STUB: DELAYLAMA 0x10002b10
    void DelayLamaAudio::loadPresetByIndex(int currentProgram) {
        // float portTime;
        // Preset (*presets) [5];
        // DelayLamaAudioVTable *vtable;
        //
        // presets = this->presets;
        // this->audioEffectXMembers.audioEffectMembers.currentPreset = currentProgram;
        // portTime = (*presets)[currentProgram].portTime;
        // this->portamentoTime = portTime;
        // this->delay = (*presets)[currentProgram].delay;
        // vtable = this->vtable;
        // this->headSize = (*presets)[currentProgram].headSize;
        // (*(vtable->audioEffectX).audioEffect.setParameterValue)(0,portTime);
        // (*(this->vtable->audioEffectX).audioEffect.setParameterValue)(2,this->delay);
        // (*(this->vtable->audioEffectX).audioEffect.setParameterValue)(3,this->headSize);
        // return;
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
        //          [this->audioEffectXMembers.audioEffectMembers.currentPreset].name;
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
        //          [this->audioEffectXMembers.audioEffectMembers.currentPreset].name;
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

    // STUB: DELAYLAMA 0x10002c90
    void DelayLamaAudio::getParameterValueString(int parameterId, char* outText) {
        // *outText = '\0';
        // switch(parameterId) {
        // case 0:
        //                   /* Portamento Time */
        //   (*(this->vtable->audioEffectX).audioEffect.formatFloatToString)
        //             (this->portamentoTime * FLOAT_1000b680,outText);
        //   return;
        // case 1:
        //                   /* Vowel */
        //   (*(this->vtable->audioEffectX).audioEffect.formatFloatToString)
        //             (this->curVowelValue,outText);
        //   return;
        // case 2:
        //                   /* Delay */
        //   (*(this->vtable->audioEffectX).audioEffect.formatFloatAsDecibelString)
        //             (this->delay,outText);
        //   return;
        // case 3:
        //                   /* Head Size */
        //   (*(this->vtable->audioEffectX).audioEffect.formatFloatToString)
        //             (this->headSize * FLOAT_1000b67c,outText);
        // }
        // return;
    }

    // STUB: DELAYLAMA 0x10002d20
    void DelayLamaAudio::getParameterName(int parameterId, char* outBuffer) {
        // char cVar1;
        // uint uVar2;
        // uint uVar3;
        // char *sourceString;
        // char *pcVar4;
        //
        // switch(parameterId) {
        // case 0:
        //   sourceString = s_PortTime_1000d31c;
        //   goto LAB_10002d45;
        // case 1:
        //   sourceString = s_Vowel_1000d2f0;
        //   break;
        // case 2:
        //   sourceString = s_Delay_1000d314;
        // LAB_10002d45:
        //   uVar2 = 0xffffffff;
        //   do {
        //     pcVar4 = sourceString;
        //     if (uVar2 == 0) break;
        //     uVar2 = uVar2 - 1;
        //     pcVar4 = sourceString + 1;
        //     cVar1 = *sourceString;
        //     sourceString = pcVar4;
        //   } while (cVar1 != '\0');
        //   uVar2 = ~uVar2;
        //   sourceString = pcVar4 + -uVar2;
        //   for (uVar3 = uVar2 >> 2; uVar3 != 0; uVar3 = uVar3 - 1) {
        //     *(undefined4 *)outBuffer = *(undefined4 *)sourceString;
        //     sourceString = sourceString + 4;
        //     outBuffer = outBuffer + 4;
        //   }
        //   for (uVar2 = uVar2 & 3; uVar2 != 0; uVar2 = uVar2 - 1) {
        //     *outBuffer = *sourceString;
        //     sourceString = sourceString + 1;
        //     outBuffer = outBuffer + 1;
        //   }
        //   return;
        // case 3:
        //   sourceString = s_HeadSize_1000d308;
        //   break;
        // default:
        //   goto LAB_10002d8d;
        // }
        // uVar2 = 0xffffffff;
        // do {
        //   pcVar4 = sourceString;
        //   if (uVar2 == 0) break;
        //   uVar2 = uVar2 - 1;
        //   pcVar4 = sourceString + 1;
        //   cVar1 = *sourceString;
        //   sourceString = pcVar4;
        // } while (cVar1 != '\0');
        // uVar2 = ~uVar2;
        // sourceString = pcVar4 + -uVar2;
        // for (uVar3 = uVar2 >> 2; uVar3 != 0; uVar3 = uVar3 - 1) {
        //   *(undefined4 *)outBuffer = *(undefined4 *)sourceString;
        //   sourceString = sourceString + 4;
        //   outBuffer = outBuffer + 4;
        // }
        // for (uVar2 = uVar2 & 3; uVar2 != 0; uVar2 = uVar2 - 1) {
        //   *outBuffer = *sourceString;
        //   sourceString = sourceString + 1;
        //   outBuffer = outBuffer + 1;
        // }
        // LAB_10002d8d:
        // return;
    }

    // STUB: DELAYLAMA 0x10002fd0
    float DelayLamaAudio::getParameterValue(int parameter) {
        // float fVar1;
        //
        // fVar1 = FLOAT_1000b208;
        // switch(parameter) {
        // case 0:
        //   return (float10)this->portamentoTime;
        // case 1:
        //   return (float10)this->curVowelValue;
        // case 2:
        //   return (float10)this->delay;
        // case 3:
        //   return (float10)this->headSize;
        // case 5:
        //   fVar1 = this->vibratoDepthCurrent;
        //   break;
        // case 6:
        //   return (float10)this->scaledVowel;
        // }
        // return (float10)fVar1;
        return 0.0;
    }

    // STUB: DELAYLAMA 0x10003050
    uint32_t DelayLamaAudio::getOutputBusProperties(uint32_t index, char* properties) {
        // int label;
        //
        // if ((int)index < 2) {
        //   label = sprintf(properties,s_Vstx__1d_1000d328,index + 1);
        //   properties[0x40] = 3;
        //   properties[0x41] = '\0';
        //   properties[0x42] = '\0';
        //   properties[0x43] = '\0';
        //   return CONCAT31((int3)((uint)label >> 8),1);
        // }
        // return index & 0xffffff00;
        return 0;
    }

    // STUB: DELAYLAMA 0x10003080
    bool DelayLamaAudio::getPresetNameByIndex(int category, int index, char* outText) {
        // char cVar1;
        // uint uVar2;
        // uint uVar3;
        // char *pcVar4;
        // char *presetName;
        //
        // if (4 < index) {
        //   return false;
        // }
        // uVar2 = 0xffffffff;
        // presetName = (*this->presets)[index].name;
        // do {
        //   pcVar4 = presetName;
        //   if (uVar2 == 0) break;
        //   uVar2 = uVar2 - 1;
        //   pcVar4 = presetName + 1;
        //   cVar1 = *presetName;
        //   presetName = pcVar4;
        // } while (cVar1 != '\0');
        // uVar2 = ~uVar2;
        // presetName = pcVar4 + -uVar2;
        // for (uVar3 = uVar2 >> 2; uVar3 != 0; uVar3 = uVar3 - 1) {
        //   *(undefined4 *)outText = *(undefined4 *)presetName;
        //   presetName = presetName + 4;
        //   outText = outText + 4;
        // }
        // for (uVar2 = uVar2 & 3; uVar2 != 0; uVar2 = uVar2 - 1) {
        //   *outText = *presetName;
        //   presetName = presetName + 1;
        //   outText = outText + 1;
        // }
        return true;
    }

    // STUB: DELAYLAMA 0x100030d0
    bool DelayLamaAudio::copyPreset(int param_1) {
        // Preset (*paPVar1) [5];
        // int iVar2;
        // Preset *activePreset;
        // Preset *destinationPreset;
        //
        // if (param_1 < 5) {
        //   paPVar1 = this->presets;
        //   activePreset = *paPVar1 + this->audioEffectXMembers.audioEffectMembers.currentPreset;
        //   destinationPreset = *paPVar1 + param_1;
        //   for (iVar2 = 9; iVar2 != 0; iVar2 = iVar2 + -1) {
        //     destinationPreset->portTime = activePreset->portTime;
        //     activePreset = (Preset *)&activePreset->delay;
        //     destinationPreset = (Preset *)&destinationPreset->delay;
        //   }
        //   return true;
        // }
        return false;
    }

    // STUB: DELAYLAMA 0x10003170
    bool DelayLamaAudio::getProductName(char* outText) {
        const char* src = "Virtual Singing Monk"; 
        ::strcpy(outText, src);
        size_t len = ::strlen(src) + 1;
        return (static_cast<uint32_t>(len >> 8) << 8) | 1;
    }

    // STUB: DELAYLAMA 0x10004870
    void DelayLamaAudio::setSampleRate(uint32_t sampleRate) {
        // this->audioEffectXMembers.audioEffectMembers.sampleRate = sampleRate;
        // this->sampleRate = sampleRate;
        // return;
    }

    // STUB: DELAYLAMA 0x10004890
    void DelayLamaAudio::setMaxFramesPerProcess(uint32_t blocksize) {
        // this->audioEffectXMembers.audioEffectMembers.blockSize = blocksize;
        // this->blockSize = blocksize;
        // return;
    }

    // STUB: DELAYLAMA 0x100048a0
    void DelayLamaAudio::disableAudioProcessing() {
        // return;
    }

    // STUB: DELAYLAMA 0x100048b0
    void DelayLamaAudio::enableAudioProcessing() {
        // (*(this->vtable->audioEffectX).requestMidiSupport)(1);
        // initialize(this);
        // return;
    }

    // STUB: DELAYLAMA 0x10005350
    void DelayLamaAudio::buildPiecewiseCubicTable(int32_t* controlPoints, float* outSamples) {
        // float fVar1;
        // float fVar2;
        // float fVar3;
        // float *outPtr;
        // int x;
        // int *window;
        // int segmentEnd;
        // int segmentBase;
        // int32_t controlWindow [6];
        // int p0;
        // int p1;
        // int p2;
        // int pMinus1;
        // float t;
        //
        // controlWindow[0] = *controlPoints;
        // controlWindow[2] = controlPoints[2];
        // controlWindow[1] = controlPoints[1];
        // controlWindow[3] = controlPoints[3];
        // controlWindow[4] = controlPoints[4];
        // controlWindow[5] = controlPoints[4];
        // controlPoints = (int32_t *)0x1;
        // segmentEnd = 320;
        // window = controlWindow;
        // do {
        //   x = segmentEnd + -320;
        //   fVar1 = (float)DOUBLE_1000bb38;
        //   if (x < segmentEnd) {
        //                   /* next point */
        //     p2 = window[2];
        //                   /* current point */
        //     p1 = *window;
        //                   /* previous point */
        //     p0 = window[1];
        //                   /* point before previous */
        //     pMinus1 = window[-1];
        //     fVar2 = (float)(((p1 - p0) * 3 - pMinus1) + p2) * FLOAT_1000bb34;
        //     fVar3 = (float)(p0 - pMinus1) * FLOAT_1000bb34;
        //     outPtr = outSamples;
        //     do {
        //       t = (float)x;
        //       x = x + 1;
        //                   /* Normalize t to [0, 1] within segment */
        //       t = (t - (float)((int)controlPoints + -1) * fVar1) * FLOAT_1000bb30;
        //                   /* Cubic interpolation */
        //       *outPtr = ((t * fVar2 +
        //                  (((float)p0 + (float)p0 + (float)pMinus1) - (float)((p1 + p2 + p1 * 4) / 2))) * t
        //                 + fVar3) * t + (float)p1;
        //       outPtr = outPtr + 1;
        //     } while (x < segmentEnd);
        //   }
        //   segmentEnd = segmentEnd + 0x140;
        //                   /* segment index++ */
        //   controlPoints = (int32_t *)((int)controlPoints + 1);
        //                   /* slide control window */
        //   window = window + 1;
        //                   /* next segment (320 samples) */
        //   outSamples = outSamples + 320;
        // } while (segmentEnd < 1600);
        // return;
    }

    // STUB: DELAYLAMA 0x100054a0
    void DelayLamaAudio::invokeAudioProcess(float* * inputs, float* * outputs, int32_t sampleFrames) {
        // (*(this->vtable->audioEffectX).audioEffect.processAudio)(inputs,outputs,sampleFrames);
        // return;
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

    // STUB: DELAYLAMA 0x10005eb0
    void DelayLamaAudio::addSynthesisToExcitation(int offsetIncrement) {
        // int i;
        // int tmpIdx;
        // int bufSize;
        // int **dstPtr;
        //
        // i = this->writeIndex + offsetIncrement;
        // this->writeIndex = i;
        // bufSize = this->numSamples;
        // if ((this->excitationBufferSize < bufSize + i) || (i < 0)) {
        //   i = 0;
        //   if (0 < bufSize) {
        //     do {
        //       bufSize = this->excitationBufferSize;
        //       tmpIdx = this->writeIndex;
        //       while (bufSize <= tmpIdx) {
        //         tmpIdx = this->writeIndex - bufSize;
        //         this->writeIndex = tmpIdx;
        //       }
        //       tmpIdx = this->writeIndex;
        //       while (tmpIdx < 0) {
        //         tmpIdx = this->writeIndex + bufSize;
        //         this->writeIndex = tmpIdx;
        //       }
        //       dstPtr = this->excitationBuffer + this->writeIndex;
        //       *dstPtr = (int *)((float)this->synthesisBuffer[i] + (float)*dstPtr);
        //       i = i + 1;
        //       this->writeIndex = this->writeIndex + 1;
        //     } while (i < this->numSamples);
        //   }
        // }
        // else {
        //   i = 0;
        //   if (0 < bufSize) {
        //     do {
        //       dstPtr = this->excitationBuffer + this->writeIndex;
        //       *dstPtr = (int *)((float)this->synthesisBuffer[i] + (float)*dstPtr);
        //       i = i + 1;
        //       this->writeIndex = this->writeIndex + 1;
        //     } while (i < this->numSamples);
        //   }
        // }
        // this->writeIndex = this->writeIndex - this->numSamples;
        // return;
    }

    // STUB: DELAYLAMA 0x10005fb0
    void DelayLamaAudio::updateVowelFilter(float vowelX) {
        // float **ppfVar1;
        // long glottalIndex;
        // long formantIndex;
        // undefined4 unaff_EBX;
        // undefined4 unaff_EBP;
        // undefined4 unaff_ESI;
        // undefined4 unaff_EDI;
        // int sampleIndex;
        // float **glottalSourcePtr;
        // float **synthesisBufferPtr;
        //
        // this->vowelLookupIndex = vowelX * FLOAT_1000bb78;
        // this->resonanceGain =
        //      this->headSize * (float)DOUBLE_1000ba78 + (float)DOUBLE_1000bb70;
        // _ftol((double)CONCAT44(unaff_ESI,unaff_EDI));
        // sampleIndex = 0;
        // if (0 < this->numSamples) {
        //   do {
        //     glottalIndex = _ftol((double)CONCAT44(unaff_EBX,unaff_EBP));
        //     glottalSourcePtr = this->glottalSource;
        //     formantIndex = _ftol((double)CONCAT44(unaff_EBX,unaff_EBP));
        //     this->synthesisBuffer[sampleIndex] =
        //          (float *)((float)glottalSourcePtr[glottalIndex] *
        //                   (float)this->formantTable[formantIndex]);
        //     glottalSourcePtr = this->synthesisBuffer + sampleIndex;
        //     glottalIndex = _ftol((double)CONCAT44(unaff_EBX,unaff_EBP));
        //     ppfVar1 = this->glottalSource;
        //     formantIndex = _ftol((double)CONCAT44(unaff_EBX,unaff_EBP));
        //     *glottalSourcePtr =
        //          (float *)((float)ppfVar1[glottalIndex] *
        //                    (float)this->formantTable[formantIndex] + (float)*glottalSourcePtr);
        //     glottalSourcePtr = this->synthesisBuffer + sampleIndex;
        //     glottalIndex = _ftol((double)CONCAT44(unaff_EBX,unaff_EBP));
        //     ppfVar1 = this->glottalSource;
        //     formantIndex = _ftol((double)CONCAT44(unaff_EBX,unaff_EBP));
        //     *glottalSourcePtr =
        //          (float *)((float)ppfVar1[glottalIndex] *
        //                    (float)this->formantTable[formantIndex] + (float)*glottalSourcePtr);
        //     glottalSourcePtr = this->synthesisBuffer + sampleIndex;
        //     *glottalSourcePtr =
        //          (float *)((float)this->harmonicBuffer[sampleIndex] * (float)DOUBLE_1000ba78 +
        //                   (float)*glottalSourcePtr);
        //     synthesisBufferPtr = this->synthesisBuffer;
        //     glottalSourcePtr = this->vocalEnvelope + sampleIndex;
        //     ppfVar1 = synthesisBufferPtr + sampleIndex;
        //     synthesisBufferPtr = synthesisBufferPtr + sampleIndex;
        //     sampleIndex = sampleIndex + 1;
        //     *synthesisBufferPtr = (float *)((float)*glottalSourcePtr * (float)*ppfVar1);
        //   } while (sampleIndex < this->numSamples);
        // }
        // return;
    }

    // STUB: DELAYLAMA 0x100061e0
    void DelayLamaAudio::processEvents(DamSDK::Api::DamEventList* eventList) {
        // MidiEvent *outEvent;
        // MidiEvent *dstStatusPtrNext;
        // DamEvent **srcEvent;
        // int i;
        // DamEvent *evt;
        //
        // i = 0;
        // if (0 < eventList->count) {
        //   srcEvent = &eventList->events;
        //   outEvent = (MidiEvent *)&this->midiQueue[0].status;
        //   do {
        //     evt = *srcEvent;
        //     dstStatusPtrNext = outEvent;
        //     if (evt->type == 1) {
        //       outEvent[-1].data2 = evt->value;
        //       dstStatusPtrNext = outEvent + 1;
        //       outEvent->timestamp = (int)evt->a;
        //       outEvent->status = (int)evt->b;
        //       outEvent->data1 = (int)evt->c;
        //     }
        //     i = i + 1;
        //     srcEvent = srcEvent + 1;
        //     outEvent = dstStatusPtrNext;
        //   } while (i < eventList->count);
        //   return;
        // }
        // return;
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

    // STUB: DELAYLAMA 0x10006330
    void DelayLamaAudio::handleControlChange(int midiData1, int midiData2) {
        // float fVar1;
        // DelayLamaAudioVTable *vtable;
        //
        //                   /* Modulation Wheel */
        // if (midiData1 == 1) {
        //   this->lfoDepth = (float)midiData2 * FLOAT_1000bb7c;
        //   return;
        // }
        //                   /* Portamento Time */
        // if (midiData1 == 5) {
        //   vtable = this->vtable;
        //   fVar1 = (float)midiData2 * FLOAT_1000bb7c;
        //   this->portamentoTime = fVar1;
        //   (*(vtable->audioEffectX).audioEffect.setParameterValue)(PortamentoTimeParameterId,fVar1);
        //   return;
        // }
        //                   /* Main Volume */
        // if (midiData1 == 7) {
        //   this->outputGain = (float)midiData2 * FLOAT_1000ba84;
        //   return;
        // }
        //                   /* Expression */
        // if (midiData1 == 0xb) {
        //   this->vibratoDirty = true;
        //   this->vibratoAmount = (float)midiData2 * FLOAT_1000bb7c;
        //   return;
        // }
        //                   /* Delay Amount (Custom) */
        // if (midiData1 == 0xc) {
        //   vtable = this->vtable;
        //   fVar1 = (float)midiData2 * FLOAT_1000bb7c;
        //   this->delay = fVar1;
        //   (*(vtable->audioEffectX).audioEffect.setParameterValue)(DelayParameterId,fVar1);
        //   return;
        // }
        //                   /* Head Size (Custom) */
        // if (midiData1 == 0xd) {
        //   vtable = this->vtable;
        //   fVar1 = (float)midiData2 * FLOAT_1000bb7c;
        //   this->headSize = fVar1;
        //   (*(vtable->audioEffectX).audioEffect.setParameterValue)(HeadSizeParameterId,fVar1);
        // }
        // return;
    }

    // STUB: DELAYLAMA 0x10006400
    float DelayLamaAudio::getRandomFloat() {
        this->rngState = this->rngState * 1664525 + 1013904223;
        return (float)this->rngState * this->delayTimeScaler;
    }

    // STUB: DELAYLAMA 0x10006430
    void DelayLamaAudio::sendMidiToHost(uint8_t status, uint8_t data1, uint8_t data2) {
        // MidiEvent **midiEvent;
        // void **vstEventContainerPtr;
        //
        // midiEvent = &this->vstMidiEvent;
        // vstEventContainerPtr = &this->eventContainer;
        // this->midiEvent = (MidiEvent *)midiEvent;
        // *midiEvent = (MidiEvent *)0x1;
        // this->midiStatus = status;
        // this->midiData1 = data1;
        // *vstEventContainerPtr = (void *)0x1;
        // this->unknownMidi = 0;
        // this->vstMidiEventSize = 24;
        // this->unknownMidi2 = 0;
        // this->midiData2 = data2;
        // sendEventsToHost(this,vstEventContainerPtr);
        // return;
    }

    // STUB: DELAYLAMA 0x1000a77a
    void DelayLamaAudio::iteratePresetBlocks(Preset* context, int stride, int iterationCount, void* callback, void* extra) {
        // int unaff_EBX;
        // int unaff_ESI;
        // void *unaff_EDI;
        // undefined4 i;
        //
        // for (i = 0; i < iterationCount; i = i + 1) {
        //   (*callback)();
        // }
        // _conditionalRunCallback(unaff_EDI,unaff_ESI,unaff_EBX,(void *)0x1);
        // return;
    }

    // STUB: DELAYLAMA 0x1000a7e4
    void DelayLamaAudio::_conditionalRunCallback(void* context, int param2, int unknownLocal, void* extraParam) {
        // int unaff_EBP;
        //
        // if (*(int *)(unaff_EBP + -0x20) == 0) {
        //   _forEachPresetBlockReverse
        //             (*(void **)(unaff_EBP + 8),*(int *)(unaff_EBP + 0xc),*(int *)(unaff_EBP + -0x1c),
        //              *(void **)(unaff_EBP + 0x18));
        // }
        // return;
    }

    // STUB: DELAYLAMA 0x1000a808
    void DelayLamaAudio::__vector_destructor_iterator(int param_1, int param_2, int param_3, void* param_4) {
        // while( true ) {
        //   param_3 = param_3 + -1;
        //   if (param_3 < 0) break;
        //   (*param_4)();
        // }
        // _conditionalCleanup();
        // return;
    }

    // STUB: DELAYLAMA 0x1000a870
    void DelayLamaAudio::_conditionalCleanup() {
        // int unaff_EBP;
        //
        // if (*(int *)(unaff_EBP + -0x1c) == 0) {
        //   _forEachPresetBlockReverse
        //             (*(void **)(unaff_EBP + 8),*(int *)(unaff_EBP + 0xc),*(int *)(unaff_EBP + 0x10),
        //              *(void **)(unaff_EBP + 0x14));
        // }
        // return;
    }

    // STUB: DELAYLAMA 0x1000a888
    void DelayLamaAudio::_forEachPresetBlockReverse(void* startPtr, int step, int count, void* callback) {
        // while( true ) {
        //   count = count + -1;
        //   if (count < 0) break;
        //   (*callback)();
        // }
        // return;
    }
}
}