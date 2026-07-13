#pragma once

#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstdint>

class AudioDebugUtils {
public:
    AudioDebugUtils() : m_isRecording(false), m_sampleRate(44100) {}

    // Set the sample rate for the exported WAV files
    void setSampleRate(uint32_t sampleRate) {
        m_sampleRate = sampleRate;
    }

    // Pre-allocate memory to avoid audio thread dropouts (e.g., reserve 10 seconds)
    void reserve(size_t numSamples) {
        for (auto& pair : m_buffers) {
            pair.second.reserve(numSamples);
        }
    }

    // Call this when isSinging changes state
    void setRecording(bool isRecording) {
        if (isRecording && !m_isRecording) {
            // Just started singing: clear old buffers
            clear();
            std::cout << "[AudioDebug] Started accumulating audio..." << std::endl;
        } 
        else if (!isRecording && m_isRecording) {
            // Just stopped singing: export files
            std::cout << "[AudioDebug] Stopped. Exporting buffers..." << std::endl;
            exportAll("."); // Exports to current directory, change path as needed
        }
        m_isRecording = isRecording;
    }

    bool isRecording() const { return m_isRecording; }

    // Accumulate a single sample for a specific stage (e.g., "01_excitation")
    void accumulateSample(const std::string& stageName, float sample) {
        if (!m_isRecording) return;
        m_buffers[stageName].push_back(sample);
    }

    // Accumulate a whole block/buffer at once
    void accumulateBlock(const std::string& stageName, const float* data, size_t numSamples) {
        if (!m_isRecording) return;
        auto& buffer = m_buffers[stageName];
        buffer.insert(buffer.end(), data, data + numSamples);
    }

    // Clear all accumulated data
    void clear() {
        for (auto& pair : m_buffers) {
            pair.second.clear();
        }
    }

private:
    bool m_isRecording;
    uint32_t m_sampleRate;
    std::map<std::string, std::vector<float>> m_buffers;

    // Zero-dependency minimal 16-bit Mono WAV exporter
    void writeWav(const std::string& filename, const std::vector<float>& data) {
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "[AudioDebug] Failed to open " << filename << " for writing.\n";
            return;
        }

        uint32_t dataSize = static_cast<uint32_t>(data.size() * sizeof(int16_t));
        uint32_t chunkSize = 36 + dataSize;
        
        // RIFF Header
        file.write("RIFF", 4);
        file.write(reinterpret_cast<const char*>(&chunkSize), 4);
        file.write("WAVE", 4);

        // Subchunk 1: fmt 
        file.write("fmt ", 4);
        uint32_t subchunk1Size = 16; // PCM
        uint16_t audioFormat = 1;    // PCM
        uint16_t numChannels = 1;    // Mono
        uint32_t byteRate = m_sampleRate * numChannels * sizeof(int16_t);
        uint16_t blockAlign = numChannels * sizeof(int16_t);
        uint16_t bitsPerSample = 16;

        file.write(reinterpret_cast<const char*>(&subchunk1Size), 4);
        file.write(reinterpret_cast<const char*>(&audioFormat), 2);
        file.write(reinterpret_cast<const char*>(&numChannels), 2);
        file.write(reinterpret_cast<const char*>(&m_sampleRate), 4);
        file.write(reinterpret_cast<const char*>(&byteRate), 4);
        file.write(reinterpret_cast<const char*>(&blockAlign), 2);
        file.write(reinterpret_cast<const char*>(&bitsPerSample), 2);

        // Subchunk 2: data
        file.write("data", 4);
        file.write(reinterpret_cast<const char*>(&dataSize), 4);

        // Convert float (-1.0 to 1.0) to 16-bit PCM and write
        for (float sample : data) {
            // Clamp to avoid wrap-around distortion if the signal blows up
            float clamped = std::max(-1.0f, std::min(1.0f, sample));
            int16_t pcm = static_cast<int16_t>(clamped * 32767.0f);
            file.write(reinterpret_cast<const char*>(&pcm), 2);
        }

        file.close();
        std::cout << "[AudioDebug] Exported: " << filename << "\n";
    }

    void exportAll(const std::string& directory) {
        for (const auto& pair : m_buffers) {
            // Only export if we actually captured data
            if (!pair.second.empty()) {
                std::string path = directory + "/" + pair.first + ".wav";
                writeWav(path, pair.second);
            }
        }
    }
};