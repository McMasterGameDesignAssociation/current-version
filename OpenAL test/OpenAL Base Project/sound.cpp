#ifndef SOUND_M
#define SOUND_M

#include "sound.h"

Sound::Sound()
{
	//Set up the wave file loader
	pWaveLoader = new CWaves();
	//populate the device lists
	pDeviceList = new ALDeviceList();
	/*
	next choose defualt device, i.e.
	audio device 0, this can be changed
	by the user later in the settings

	first step is to make sure the device list isn't
	NULL and atleast 1 device exists in the list
	*/
	if((pDeviceList) && (pDeviceList->GetNumDevices()))
	{
		//next get the default device
		pDevice = alcOpenDevice(pDeviceList->GetDeviceName(pDeviceList->GetDefaultDevice()));

		//If the device exists, generate a context for the 
		//device to operate in, else close the device and
		//set the bReturn to false
		if(pDevice)
		{		
			alcMakeContextCurrent(pContext);
		}
		else
		{
			alcCloseDevice(pDevice);
			return;
		}
	}

}

void Sound::loadWaveFile(string fileName)
{
		pWaveLoader->LoadWaveFile(&fileName[0], &WaveID);
		pWaveLoader->GetWaveSize(WaveID, (unsigned long*)&iDataSize);
		pWaveLoader->GetWaveData(WaveID, (void**)&pData);
		pWaveLoader->GetWaveFrequency(WaveID, (unsigned long*)&iFrequency);
		pWaveLoader->GetWaveALBufferFormat(WaveID, &alGetEnumValue, (unsigned long*)&eBufferFormat);
		pWaveLoader->GetWaveFormatExHeader(WaveID, &wfex);
}

//Currently this set up only works with 1 source
void Sound::setBufferToSound(void)
{
	DWORD ulBufferSize = (wfex.nAvgBytesPerSec >> 2);
	ulBufferSize -= (ulBufferSize % wfex.nBlockAlign);
	//We need to generate the openAL buffers
	alGenBuffers(bufferNumber, iBuffers);
	//next we need to set the sources of the sound
	pData = malloc(ulBufferSize);
	alGenSources(1, iSources);
	alBufferData(uiBufferID, eBufferFormat, pData, iDataSize, iFrequency);
	pWaveLoader->SetWaveDataOffset(WaveID, 0);
	for(int i = 0; i < 4; i++)
	{

	}
}

Sound::~Sound()
{
	pWaveLoader->DeleteWaveFile(WaveID);
}
#endif