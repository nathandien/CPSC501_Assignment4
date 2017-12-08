#include <iostream>
#include <stdio.h>
#include <stdlib.h> 
using namespace std;

//https://stackoverflow.com/questions/13660777/c-reading-the-data-part-of-a-wav-file

struct WAV_HEADER {
	
	unsigned char 	chunkID[4]; 	// "RIFF"
	unsigned int 	chunkSize; 		// Size of remaining file in bytes (36 + Subchunk2Size)
	unsigned char 	fmt[4]; 		// "WAVE"
	unsigned char 	subChunk1ID[4];	// "fmt "
	unsigned int 	subChunk1Size; 	// 16
	unsigned int	audioFmt; 		// 1 = PCM
	unsigned int	numChannels;	// 1 = mono, 2 = stereo, etc
	unsigned int	sampleRate;		
	unsigned int	byteRate;		// sampleRate * numChannels * bitsPerSample / 8
	unsigned int	blockAlign;		// Frame size in bytes (numChannels * bitsPerSample / 8)
	unsigned int	bitsPerSample;	// Sample size in bits (e.g 16)
	unsigned char	subChunk2ID[4];	// "data"
	unsigned int	subChunk2Size;	// Size of sound sample data in bytes
	//unsigned int	data;			// Actual sound samples

};

unsigned int convertBig(unsigned char buffer[]) {
	return (buffer[0] | (buffer[1]<<8) | (buffer[2]<<16) | (buffer[3]<<24));
}

unsigned int convertLit(unsigned char buffer[]) {
	return (buffer[0] | (buffer[1]<<8));
}

int main(int argc, char* argv[]) {
	
	const char* inFileName;
	const char* IRFileName;
	const char* outFileName;
	
	struct WAV_HEADER wav_header;
	int bytesRead;
	
	if(argc != 4) {
		printf("Usage: %s <inputfile> <IRfile> <outputfile>\n", argv[0]);
		exit(1);
	}
	else {
		inFileName = argv[1];
		IRFileName = argv[2];
		outFileName = argv[3];
	}
	
	FILE* wavFile = fopen(inFileName, "rb");
	// Check file
	if(wavFile == NULL) {
		printf("Error: Failed to open file %s", inFileName);
		exit(1);
	}
	
	int headerSize = sizeof(WAV_HEADER);
 
	// Reads chunk ID
	bytesRead = fread(wav_header.chunkID, sizeof(wav_header.chunkID), 1, wavFile);
	cout << wav_header.chunkID << "\n";
	// Reads chunk size
	unsigned char buffer[4];
	unsigned char litBuffer[2];
	bytesRead = fread(buffer, sizeof(buffer), 1, wavFile);
	printf("%u %u %u %u\n", buffer[0], buffer[1]<<8, buffer[2]<<16, buffer[3]<<24);
	wav_header.chunkSize  = (convertBig(buffer));

	bytesRead = fread(wav_header.fmt, sizeof(wav_header.fmt), 1, wavFile);
	bytesRead = fread(wav_header.subChunk1ID, sizeof(wav_header.subChunk1ID), 1, wavFile);
	bytesRead = fread(buffer, sizeof(buffer), 1, wavFile);
	wav_header.subChunk1Size = convertBig(buffer);
	bytesRead = fread(litBuffer, sizeof(litBuffer), 1, wavFile);
	wav_header.audioFmt = convertLit(buffer);
	bytesRead = fread(litBuffer, sizeof(litBuffer), 1, wavFile);
	wav_header.numChannels = convertLit(buffer);
	bytesRead = fread(buffer, sizeof(buffer), 1, wavFile);
	wav_header.sampleRate = convertBig(buffer);
	bytesRead = fread(buffer, sizeof(buffer), 1, wavFile);
	wav_header.byteRate = convertBig(buffer);
	bytesRead = fread(litBuffer, sizeof(litBuffer), 1, wavFile);
	wav_header.blockAlign = convertLit(buffer);
	bytesRead = fread(litBuffer, sizeof(litBuffer), 1, wavFile);
	wav_header.bitsPerSample = convertLit(buffer);
	bytesRead = fread(wav_header.subChunk2ID, sizeof(wav_header.subChunk2ID), 1, wavFile);
	bytesRead = fread(buffer, sizeof(buffer), 1, wavFile);
	wav_header.subChunk2Size = convertBig(buffer);
	
	cout << wav_header.chunkID << "\n";
	cout << wav_header.chunkSize << "\n";
	cout << wav_header.subChunk1ID << "\n";
	cout << wav_header.subChunk1Size << "\n";
	cout << wav_header.audioFmt << "\n";
	cout << wav_header.numChannels << "\n";
	/*
	cout << wav_header.sampleRate << "\n";
	cout << wav_header.byteRate << "\n";
	cout << wav_header.blockAlign << "\n";
	cout << wav_header.bitsPerSample << "\n";
	cout << wav_header.subChunk2ID << "\n";
	cout << wav_header.subChunk2Size << "\n";
	*/
}

void convolve(float x[], int N, float h[], int M, float y[], int P) {
	int n, m;
	
	/* Clear output buffer y[] */
	for(n = 0; n < P; n++)
		y[n] = 0.0;
		
	/* Outer loop: process each input value x[n] in turn */
	for(n = 0; n < N; n++) {
		/* Inner loop: process x[n] with each sample of h[n] */
		for(m = 0; m < M; m++)
			y[n+m] += x[n] * h[m];
	}
}


