/******************************************************************************
 * FILE NAME :      ima_enc.c
 *
 * DESIGNER :       Honda Hsu
 *
 * COPYRIGHT :      Copyright (c) Airoha Tek. Cop.
 *
 * DESCRIPTION :    4-bit IMA ADPCM encoder
 *
 * REVISION HISTORY :
 *  MAINTAINER  DATE        DESCRIPTION
 *  Honda Hsu     20160926    Creation
 *
 ******************************************************************************/

#include <stdint.h>

#define FILT_LEN 40
const int16_t filt_coef[] = {
    174,   478,   477,   -17,  -393,   -21,   465,    83,  -600,  -182,
    780,   346, -1037,  -614,  1436,  1119, -2221, -2393,  4929, 13573,
};

int16_t filt_state[FILT_LEN * 2];
int16_t idx;

const int8_t IndexTable[16] = { -1, -1, -1, -1, 2, 4, 6, 8, -1, -1, -1, -1, 2, 4, 6, 8};

const int16_t StepSizeTable[89] = {
    7, 8, 9, 10, 11, 12, 13, 14,
    16, 17, 19, 21, 23, 25, 28, 31,
    34, 37, 41, 45, 50, 55, 60, 66,
    73, 80, 88, 97, 107, 118, 130, 143,
    157, 173, 190, 209, 230, 253, 279, 307,
    337, 371, 408, 449, 494, 544, 598, 658,
    724, 796, 876, 963, 1060, 1166, 1282, 1411,
    1552, 1707, 1878, 2066, 2272, 2499, 2749, 3024,
    3327, 3660, 4026, 4428, 4871, 5358, 5894, 6484,
    7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899,
    15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767
};

int16_t prevsample;
uint16_t previndex;


int16_t ima_adpcm_get_prevsample(void)
{
    return prevsample;
}

uint16_t ima_adpcm_get_previndex(void)
{
    return previndex;
}

void ima_adpcm_encode_init(void)
{
    int i;
    prevsample = 0;
    previndex = 0;
    for (i = 0; i < FILT_LEN * 2; i++) {
        filt_state[i] = 0;
    }
    idx = FILT_LEN - 2;
}

int ima_32_encoder(int raw_y)
{
    int predsample, index, step, diff, diffq, code = 0;
    int i;

    predsample = prevsample;
    index = previndex;
    step = StepSizeTable[index];

    diff = raw_y - predsample;
    if (diff < 0) {
        code = 1;
        diff = -diff;
    }

    diffq = 0;
    i = 3;
    diff <<= 3;
    step <<= 3;


    do {
        code <<= 1;
        if (diff >= step) {
            code += 1;
            diff -= step;
            diffq += step;
        }
        step >>= 1;
        i--;
    } while (i > 0);
    diffq += step;
    diffq >>= 3;

    if (code & 8) {
        predsample -= diffq;
    } else {
        predsample += diffq;
    }

    if (predsample > 32767) {
        predsample = 32767;
    } else if (predsample < -32768) {
        predsample = -32768;
    }

    index = index + IndexTable[code];

    if (index < 0) {
        index = 0;
    } else if (index > 88) {
        index = 88;
    }

    prevsample = (int16_t)predsample;
    previndex = (uint16_t)index;

    return code;
}

int ima_adpcm_encode_proc(short *signal, unsigned char *serial, int len)
{
    int i, tmp;
    int enc_len = 0;
    unsigned char enc;
    for (i = 0; i < len; i += 2) {
        tmp = ima_32_encoder(signal[i]);
        enc = (unsigned char)tmp; //low nibble
        tmp = ima_32_encoder(signal[i + 1]);
        enc |= (unsigned char)(tmp << 4); //high nibble
        serial[enc_len++] = enc;
    }
    return enc_len;
}

int ima_adpcm_encode_proc_big(short *signal, unsigned char *serial, int len)
{
    int i, tmp;
    int enc_len = 0;
    unsigned char enc;
    for (i = 0; i < len; i += 2) {
        tmp = ima_32_encoder(signal[i]);
        enc = (unsigned char)(tmp << 4); //high nibble
        tmp = ima_32_encoder(signal[i + 1]);
        enc |= (unsigned char)tmp; //low nibble
        serial[enc_len++] = enc;
    }
    return enc_len;
}

static short D2_FILT(short in0, short in1)
{
    long out = 0;
    short *ph, *pe;
    int i;
    filt_state[idx] = filt_state[idx + FILT_LEN] = (in1 >> 2);
    filt_state[idx + 1] = filt_state[idx + FILT_LEN + 1] = (in0 >> 2);
    ph = filt_state + idx;
    pe = ph + FILT_LEN - 1;
    for (i = 0; i < FILT_LEN / 2; i++) {
        out += (*ph + *pe) * filt_coef[i];
        ph++;
        pe--;
    }
    idx -= 2;
    if (idx < 0) {
        idx = FILT_LEN - 2;
    }
    out >>= 15;

    return (short)out;
}

int ima_adpcm_encode_proc_halfrate(short *signal, unsigned char *serial, int len)
{
    int i, tmp;
    int enc_len = 0;
    unsigned char enc;
    short sig0, sig1;
    for (i = 0; i < len; i += 4) {
        sig0 = signal[i];
        sig1 = signal[i + 1];
        sig0 = D2_FILT(sig0, sig1);
        tmp = ima_32_encoder(sig0);
        enc = (unsigned char)tmp; //low nibble
        sig0 = signal[i + 2];
        sig1 = signal[i + 3];
        sig0 = D2_FILT(sig0, sig1);
        tmp = ima_32_encoder(sig0);
        enc |= (unsigned char)(tmp << 4); //high nibble
        serial[enc_len++] = enc;
    }
    return enc_len;
}
