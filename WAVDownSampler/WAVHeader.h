#pragma once
#include <stdint.h>
#include <fstream>
#include <string>

class CWavHeader
{

	//RIFF Chunk descriptor
	uint8_t RIFF_ChunkID[4];
	uint32_t m_nChunkSize;
	uint8_t Format[4];

	//FMT sub-chunk

	uint8_t SubChunk1ID[4];
	uint32_t m_nSubChunk1Size;
	uint16_t m_nAudioFormat;
	uint16_t m_nNumChannels;
	uint32_t m_nSampleRate;
	uint32_t m_nByteRate;
	uint16_t m_nBlockAlign;
	uint16_t m_nBitsPerSample;


public:
	CWavHeader();


	//Getters:

	uint64_t GetChunkSize();
	std::string GetRIFFChunkID();
	std::string GetWAVFormat();
	std::string GetFMTChunk();
	uint32_t GetBitRate();
	uint32_t GetSubChunk1Size();

	uint16_t GetAudioFormat();
	uint16_t GetNumChannels();
	uint32_t GetSamplingFrequency();
	uint32_t GetByteRate();
	uint16_t GetBlockAlign();
	uint16_t GetBitsPerSample();

	//Setters
	void DownSample();
	void SetChunkSize(uint64_t val);

};