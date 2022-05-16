#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

#include "WavHeader.h"

struct downSampledHeader {
	
	uint8_t RIFF[4] = { 'R', 'I', 'F', 'F' }; 
	uint32_t ChunkSize; 
	uint8_t WAVE[4] = { 'W', 'A', 'V', 'E' };
	
	uint8_t fmt[4] = { 'f', 'm', 't', ' ' }; 
	uint32_t Subchunk1Size = 16;    
	uint16_t AudioFormat = 1; 
					
	uint16_t NumOfChan = 8; 
	uint32_t SamplingFrequency = 48000 / 2;  
	uint32_t bytesPerSec = 1152000;
	uint16_t blockAlign = 24;          
	uint16_t bitsPerSample = 24;     
	/* "data" sub-chunk */

	uint8_t Subchunk2ID[4] = { 'd', 'a', 't', 'a' };
	uint32_t Subchunk2Size;
};

void ExtractSubChunk2Size(ifstream& in,uint32_t& SubChunk2Size);
void WriteDownSampledWav(ifstream& in,CWavHeader& WAVItem,ofstream& wavOutput);
void fillNewHeader(downSampledHeader& hdr, CWavHeader& hdrOG);
int computeFileSize(FILE* inFile);

int main(int argc, char* argv[])
{
	CWavHeader WAVItem;
	if (argc <= 1)
	{
		cout << "CMD argument not set\n";
	}

	string fileName = argv[1];
	FILE* inputWavFile = fopen(fileName.c_str(), "r");
	if (inputWavFile == nullptr)
	{
		cout << "WAV file didn't open\n";
		return 1;
	}

	//Reads Fixed Header 36 Bytes
	size_t ItemSize = sizeof(WAVItem);
	auto GetBytesRead = [&WAVItem](FILE* fileName, size_t wavHeaderSize)
	{
		return fread(&WAVItem, 1, wavHeaderSize, fileName);
	};
	GetBytesRead(inputWavFile, ItemSize);
	fclose(inputWavFile);

	string convertedFile = "DownSampledFile_" + fileName;
	//Creating a file for converted output
	ofstream wavOutput(convertedFile.c_str(), std::ios::binary);
	WAVItem.DownSample();

	//Extracting raw data from the original wav 
	//Exctracting SubChunk2Size
	ifstream in(fileName, ifstream::binary);
	uint32_t SubChunk2Size;
	ExtractSubChunk2Size(in, SubChunk2Size);
	
	//Writing modified Header
	downSampledHeader dataChunk;
	fillNewHeader(dataChunk, WAVItem);
	dataChunk.Subchunk2Size = SubChunk2Size / 2;
	dataChunk.ChunkSize = 36 + dataChunk.Subchunk2Size;
	wavOutput.write((char*)&dataChunk, sizeof(dataChunk));
	//Header writing complete.

	//Writing raw data
	WriteDownSampledWav(in,WAVItem,wavOutput);

	return 0;
}

void WriteDownSampledWav(ifstream& in, CWavHeader& WAVItem, ofstream& wavOutput)
{

	uint32_t fsize = in.tellg();
	int pos = in.tellg();
	in.seekg(0, std::ios::end);
	fsize = (uint32_t)in.tellg() - fsize;
	in.seekg(0, std::ios::beg);

	in.ignore(pos);


	//Reduces total frames by 2;
	int WORD = WAVItem.GetBlockAlign();
	uint8_t* d = new uint8_t[WORD];
	cout << sizeof(d) << "\n";
	for (int i = 0; i < fsize / WORD; ++i) {

		if (!(i & 1))
		{
			in.read(reinterpret_cast<char *>(d), WORD);
			wavOutput.write(reinterpret_cast<char *>(d), WORD);
		}
		else in.read(reinterpret_cast<char *>(d), WORD);
	}
	free(d);
	wavOutput.close();
}

void ExtractSubChunk2Size(ifstream& in, uint32_t& SubChunk2Size)
{
	string beyond36;
	getline(in, beyond36, 'd');
	
	char c;
	while (in >> c)
	{
		if (c == 'a')
			break;
	}
	in.ignore(2);
	in.read(reinterpret_cast<char*>(&SubChunk2Size), sizeof(uint32_t));
}

int computeFileSize(FILE* inFile)
{
	int fileSize = 0;
	fseek(inFile, 0, SEEK_END);

	fileSize = ftell(inFile);

	fseek(inFile, 0, SEEK_SET);
	return fileSize;
}

void fillNewHeader(downSampledHeader& hdr, CWavHeader& hdrOG)
{
	hdr.AudioFormat = 1;
	hdr.bitsPerSample = hdrOG.GetBitsPerSample();
	hdr.blockAlign = hdrOG.GetBlockAlign();
	hdr.bytesPerSec = hdrOG.GetByteRate();

	hdr.NumOfChan = hdrOG.GetNumChannels();
	hdr.SamplingFrequency = hdrOG.GetSamplingFrequency();
	hdr.Subchunk1Size = 16;
}

