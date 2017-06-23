
/* Compile with gcc -o noisecancel noisecancel.c -lasound */

#include <alsa/asoundlib.h>

#define likely(x)       __builtin_expect((x),1)

int main()
{
	short *buffer = (short*)malloc(44100 * sizeof(short));;
	int err, i, j = sizeof(buffer);
	snd_pcm_t *inhandle, *outhandle;
	
	snd_pcm_open(&inhandle, "default", SND_PCM_STREAM_CAPTURE, 0);
	snd_pcm_open(&outhandle, "default", SND_PCM_STREAM_PLAYBACK, 0);

	snd_pcm_set_params(inhandle, SND_PCM_FORMAT_S16_LE, SND_PCM_ACCESS_RW_INTERLEAVED, 1, 44100, 1, 0);
	snd_pcm_set_params(outhandle, SND_PCM_FORMAT_S16_LE, SND_PCM_ACCESS_RW_INTERLEAVED, 1, 44100, 1, 0);

	while (likely(1)) {
		if (snd_pcm_readi(inhandle, buffer, j) < 0)
			snd_pcm_prepare(inhandle);
		for(i = 0; i < j; ++i)
			~buffer[i];
		if ((err = snd_pcm_writei(outhandle, buffer, j)) < 0) {
			if (snd_pcm_recover(outhandle, err, 1) >= 0)
				snd_pcm_writei(outhandle, buffer, j);
		}
	}
	return 0;
}
