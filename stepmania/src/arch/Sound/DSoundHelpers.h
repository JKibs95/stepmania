#ifndef DSOUND_HELPERS
#define DSOUND_HELPERS 1

#include "SDL.h"
#include "windows.h"

struct IDirectSound;
struct IDirectSoundBuffer;

class DSound
{
	IDirectSound *ds;
	static BOOL CALLBACK EnumCallback( LPGUID lpGuid, LPCSTR lpcstrDescription, LPCSTR  lpcstrModule, LPVOID lpContext);

	void SetPrimaryBufferMode();
    
public:
	IDirectSound *GetDS() const { return ds; }
	bool IsEmulated() const;

	DSound();
	~DSound();
};

class DSoundBuf
{
	IDirectSoundBuffer *buf;

	int channels, samplerate, samplebits, writeahead;

	int buffersize;
	
	int buffersize_frames() const { return buffersize / samplesize(); }
	int samplesize() const { return channels*samplebits/8; }

	int write_cursor, buffer_bytes_filled; /* bytes */
	int last_cursor_pos; /* frames */
	mutable int LastPosition;

	bool buffer_locked;
	char *locked_buf;
	int locked_len;

public:
	enum hw { HW_HARDWARE, HW_SOFTWARE, HW_DONT_CARE };

	/* If samplerate is DYNAMIC_SAMPLERATE, then call SetSampleRate before
	 * you use the sample. */
	enum { DYNAMIC_SAMPLERATE = -1 };
	DSoundBuf(DSound &ds, hw hardware, 
		int channels, int samplerate, int samplebits, int writeahead);

	bool get_output_buf(char **buffer, unsigned *bufsiz, int chunksize);
	void release_output_buf(char *buffer, unsigned bufsiz);

	void Reset();
	void Play();
	void Stop();
	void SetVolume(float vol);
	void SetSampleRate(int hz);

	~DSoundBuf();
	int GetPosition() const;
	int GetOutputPosition() const { return last_cursor_pos; }
};

#endif

/*
 * Copyright (c) 2002 by the person(s) listed below.  All rights reserved.
 *
 * Glenn Maynard
 */
