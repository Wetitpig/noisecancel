#include <alsa/asoundlib.h>
#include <stdio.h>
	
int main()
{
	char *buffer;
	int err, i, j;
	snd_pcm_t *inhandle, *outhandle;
	
	snd_pcm_open(&inhandle, "default", SND_PCM_STREAM_CAPTURE, 0);
	snd_pcm_open(&outhandle, "default", SND_PCM_STREAM_PLAYBACK, 0);

	snd_pcm_set_params(inhandle, SND_PCM_FORMAT_S16_LE, SND_PCM_ACCESS_RW_INTERLEAVED, 1, 44100, 1, 0);
	snd_pcm_set_params(outhandle, SND_PCM_FORMAT_S16_LE, SND_PCM_ACCESS_RW_INTERLEAVED, 1, 44100, 1, 0);

	buffer = (char*)malloc(44100 * sizeof(char));
	j = sizeof(buffer);

	while (1) {
		if (snd_pcm_readi(inhandle, buffer, j) < 0)
			snd_pcm_prepare(inhandle);
		if ((err = snd_pcm_writei(outhandle, buffer, j)) < 0) {
			if (snd_pcm_recover(outhandle, err, 1) >= 0)
				snd_pcm_writei(outhandle, buffer, j);
		}
	}
}
