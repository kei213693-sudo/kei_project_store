#ifndef __IMA_ENC_H__
#define __IMA_ENC_H__

int16_t ima_adpcm_get_prevsample(void);
uint16_t ima_adpcm_get_previndex(void);
void ima_adpcm_encode_init(void);
int ima_adpcm_encode_proc(short *signal, unsigned char *serial, int len);
int ima_adpcm_encode_proc_big(short *signal, unsigned char *serial, int len);
int ima_adpcm_encode_proc_halfrate(short *signal, unsigned char *serial, int len);

#endif
