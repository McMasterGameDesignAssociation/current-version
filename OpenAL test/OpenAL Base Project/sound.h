#ifndef SOUND_H
#define SOUND_H

#include "CWaves.h"
#include "al.h"
#include "aldlist.h"
#include "alc.h"

class Sound
{

	private:

		//OAL important information
		ALuint		    *uiBuffers;
		ALuint		    uiSource;
		ALuint			uiBufferID;
		ALint			iState;
		CWaves *		pWaveLoader = NULL;
		WAVEID			WaveID;
		ALint			iLoop;
		ALint			iBuffersProcessed, 
						iTotalBuffersProcessed, 
						iQueuedBuffers, 
						iDataSize, iFrequency;
		ALuint			*iBuffers, *iSources;
		WAVEFORMATEX	wfex;
		int bufferNumber = 4;
		unsigned long	ulDataSize = 0;
		unsigned long	ulFrequency = 0;
		unsigned long	ulFormat = 0;
		unsigned long	ulBufferSize;
		unsigned long	ulBytesWritten;
		void *			pData = NULL;

		ALenum			eBufferFormat;

		//Device information
		ALDeviceList *pDeviceList = NULL;
		ALCcontext *pContext = NULL;
		ALCdevice *pDevice = NULL;

	public:
		Sound();
		~Sound();
		void loadWaveFile(string fileName);
		void setBufferToSound(void);

};

#endif