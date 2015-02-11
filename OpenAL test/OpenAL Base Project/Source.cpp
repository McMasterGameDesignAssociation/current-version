#ifndef OAL_BASE_PROJECT
#define OAL_BASE_PROJECT

#include "freeglut.h"
#include "al.h"

#include "aldlist.h"
#include "Framework.h"
#include "CWaves.h"
#include <iostream>
#include "sound.h"

using namespace std;
#define	SERVICE_UPDATE_PERIOD	(20)
#define NUMBUFFERS              (4)

#define	TEST_WAVE_FILE		"stereo.wav"

void specialFunc(int key, int x, int y){ }

void keyboard(unsigned char key, int x, int y)
{
	if(key == 27) exit(0xAAAA);
}

int test(const char* waveFile, ALfloat * position)
{
	ALuint		    uiBuffers[NUMBUFFERS];
	ALuint		    uiSource;
	ALuint			uiBuffer;
	ALint			iState;
	CWaves *		pWaveLoader = NULL;
	WAVEID			WaveID;
	ALint			iLoop;
	ALint			iBuffersProcessed, iTotalBuffersProcessed, iQueuedBuffers;
	WAVEFORMATEX	wfex;
	unsigned long	ulDataSize = 0;
	unsigned long	ulFrequency = 0;
	unsigned long	ulFormat = 0;
	unsigned long	ulBufferSize;
	unsigned long	ulBytesWritten;
	void *			pData = NULL;

	ALDeviceList *pDeviceList = NULL;
	ALCcontext *pContext = NULL;
	ALCdevice *pDevice = NULL;
	ALint i;
	ALboolean bReturn = AL_FALSE;
	
	pDeviceList = new ALDeviceList();
	if((pDeviceList) && (pDeviceList->GetNumDevices()))
	{
	
		pDevice = alcOpenDevice(pDeviceList->GetDeviceName(0));
		if(pDevice)
		{
			pContext = alcCreateContext(pDevice, NULL);
			if(pContext)
			{
				alcMakeContextCurrent(pContext);
				bReturn = AL_TRUE;
			}
			else
			{
				alcCloseDevice(pDevice);
			}
		}

		delete pDeviceList;
	}

	//Generate some AL Buffers for streaming
	alGenBuffers(NUMBUFFERS, uiBuffers);

	//Generate a Source to playback the Buffers
	alGenSources(1, &uiSource);
	alSourcei(uiSource, AL_SOURCE_RELATIVE , AL_TRUE);

	alSourcefv(uiSource, AL_POSITION, position);
	// Create instance of WaveLoader class
	pWaveLoader = new CWaves();
	if((pWaveLoader) && (SUCCEEDED(pWaveLoader->OpenWaveFile(waveFile, &WaveID))))
	{
		pWaveLoader->GetWaveSize(WaveID, &ulDataSize);
		pWaveLoader->GetWaveFrequency(WaveID, &ulFrequency);
		pWaveLoader->GetWaveALBufferFormat(WaveID, &alGetEnumValue, &ulFormat);

		// Queue 250ms of audio data
		pWaveLoader->GetWaveFormatExHeader(WaveID, &wfex);
		ulBufferSize = wfex.nAvgBytesPerSec >> 2;

		// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
		ulBufferSize -= (ulBufferSize % wfex.nBlockAlign);

		if(ulFormat != 0)
		{
			pData = malloc(ulBufferSize);
			if(pData)
			{
				// Set read position to start of audio data
				pWaveLoader->SetWaveDataOffset(WaveID, 0);

				// Fill all the Buffers with audio data from the wavefile
				for(iLoop = 0; iLoop < 4; iLoop++)
				{
					if(SUCCEEDED(pWaveLoader->ReadWaveData(WaveID, pData, ulBufferSize, &ulBytesWritten)))
					{
						alBufferData(uiBuffers[iLoop], ulFormat, pData, ulBytesWritten, ulFrequency);
						alSourceQueueBuffers(uiSource, 1, &uiBuffers[iLoop]);
					}
				}				
				// Start playing source
				alSourcePlay(uiSource);
				
				iTotalBuffersProcessed = 0;

				while(!ALFWKeyPress())
				{
					Sleep(SERVICE_UPDATE_PERIOD);

					// Request the number of OpenAL Buffers have been processed (played) on the Source
					iBuffersProcessed = 0;
					alGetSourcei(uiSource, AL_BUFFERS_PROCESSED, &iBuffersProcessed);

					// Keep a running count of number of buffers processed (for logging purposes only)
					iTotalBuffersProcessed += iBuffersProcessed;

					// For each processed buffer, remove it from the Source Queue, read next chunk of audio
					// data from disk, fill buffer with new data, and add it to the Source Queue
					while(iBuffersProcessed)
					{
						// Remove the Buffer from the Queue.  (uiBuffer contains the Buffer ID for the unqueued Buffer)
						uiBuffer = 0;
						alSourceUnqueueBuffers(uiSource, 1, &uiBuffer);

						// Read more audio data (if there is any)
						pWaveLoader->ReadWaveData(WaveID, pData, ulBufferSize, &ulBytesWritten);
						if(ulBytesWritten)
						{
							// Copy audio data to Buffer
							alBufferData(uiBuffer, ulFormat, pData, ulBytesWritten, ulFrequency);
							// Queue Buffer on the Source
							alSourceQueueBuffers(uiSource, 1, &uiBuffer);
						}

						iBuffersProcessed--;
					}

					// Check the status of the Source.  If it is not playing, then playback was completed,
					// or the Source was starved of audio data, and needs to be restarted.
					alGetSourcei(uiSource, AL_SOURCE_STATE, &iState);
					if(iState != AL_PLAYING)
					{
						// If there are Buffers in the Source Queue then the Source was starved of audio
						// data, so needs to be restarted (because there is more audio data to play)
						alGetSourcei(uiSource, AL_BUFFERS_QUEUED, &iQueuedBuffers);
						if(iQueuedBuffers)
						{
							alSourcePlay(uiSource);
						}
						else
						{
							// Finished playing
							break;
						}
					}
				}

				// Stop the Source and clear the Queue
				alSourceStop(uiSource);
				alSourcei(uiSource, AL_BUFFER, 0);

				// Release temporary storage
				free(pData);
				pData = NULL;
			}
			else
			{
				ALFWprintf("Out of memory\n");
			}
		}
		else
		{
			ALFWprintf("Unknown Audio Buffer format\n");
		}

		// Close Wave Handle
		pWaveLoader->DeleteWaveFile(WaveID);
	}
	else
	{
		ALFWprintf("Failed to load %s\n", waveFile);
	}

	// Clean up buffers and sources
	alDeleteSources(1, &uiSource);
	alDeleteBuffers(NUMBUFFERS, uiBuffers);

	if(pWaveLoader)
		delete pWaveLoader;
	ALFWShutdownOpenAL();
	return 0;
}


void display(void)
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluOrtho2D(0, 500, 0, 500);
	glViewport(0, 0, 500, 500);
	
	glPointSize(50);
	glBegin(GL_POINTS);
	glColor3f(1, 1, 1);
	glVertex2i(25, 250);
	glColor3f(1, 1, 0);
	glVertex2i(475, 250);
	glColor3f(1, 0, 0);
	glVertex2i(250, 250);
	glColor3f(0, 1, 1);
	glVertex2i(250, 25);
	glColor3f(0, 0, 1);
	glVertex2i(250, 475);
	glEnd();
	
	glutSwapBuffers();
	glFlush();
}

void mouse(int button, int state, int x, int y)
{
	//right side
	if(state == GLUT_UP) return;
	if(((x < 50) && (x > 0)) && ((y < 275) && (y > 225)))
	{
		float position[] = {-5, 0, 0};
		Sleep(10);
		test("wave1.wav", position);
	}
	//center
	else if(((x < 275) && (x > 225)) && ((y < 275) && (y > 225)))
	{
		float position[] = { 0, 0, 0 };
		Sleep(10);
		test("wave2.wav", position);
	}
	//left side
	else if(((x < 500) && (x > 450)) && ((y < 275) && (y > 225)))
	{
		float position[] = { 5, 0, 0 };
		Sleep(10);
		test("wave3.wav", position);
	}
	//bottom
	else if(((x < 275) && (x > 225)) && ((y < 50) && (y > 0)))
	{
		float position[] = { 0, 0, -5 };
		Sleep(10);
		test("wave2.wav", position);
	}
	//top
	else if(((x < 275) && (x > 225)) && ((y < 500) && (y > 450)))
	{
		float position[] = { 0, 0, 5 };
		Sleep(10);
		test("wave2.wav", position);
	}
	cout << "clicked: " << x << " " << y << endl;
}

void main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitWindowSize(500, 500);
	glutCreateWindow("OAL Test");
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutSpecialFunc(specialFunc);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
}

#endif