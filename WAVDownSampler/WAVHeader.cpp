#include "WavHeader.h"

CWavHeader::CWavHeader()
{
	m_nAudioFormat = 0;
	m_nBitsPerSample = 0;
	m_nBlockAlign = 0;
	m_nByteRate = 0;
	m_nChunkSize = 0;
	m_nNumChannels = 0;
	m_nSampleRate = 0;
	m_nSubChunk1Size = 0;


	for (int i = 0; i < 4; i++)
		Format[i] = 0;
	for (int i = 0; i < 4; i++)
		SubChunk1ID[i] = 0;
	for (int i = 0; i < 4; i++)
		RIFF_ChunkID[i] = 0;

}

void CWavHeader::SetChunkSize(uint64_t val)
{
	m_nChunkSize = val;
}

uint32_t CWavHeader::GetByteRate() {
	return m_nByteRate;
}
uint32_t CWavHeader::GetSubChunk1Size()
{
	return m_nSubChunk1Size;
}

uint64_t CWavHeader::GetChunkSize()
{
	return m_nChunkSize;
}
std::string   CWavHeader::GetRIFFChunkID()
{
	std::string res;
	for (auto x : RIFF_ChunkID)res += x;
	return res;
}
std::string   CWavHeader::GetWAVFormat()
{
	std::string res;
	for (auto x : Format)res += x;
	return res;
}
std::string   CWavHeader::GetFMTChunk()
{
	std::string res;
	for (auto x : SubChunk1ID)res += x;
	return res;
}

uint32_t CWavHeader::GetSamplingFrequency()
{
	return m_nSampleRate;
}
uint16_t CWavHeader::GetBitsPerSample()
{
	return m_nBitsPerSample;
}
uint16_t CWavHeader::GetNumChannels()
{
	return m_nNumChannels;
}
uint32_t CWavHeader::GetBitRate()
{
	return 8 * m_nByteRate;
}
uint16_t CWavHeader::GetAudioFormat()
{
	return m_nAudioFormat;
}
uint16_t CWavHeader::GetBlockAlign()
{
	return m_nBlockAlign;
}

void CWavHeader::DownSample()
{
	m_nSampleRate /= 2;
	m_nByteRate /= 2;
}