#include <ultra64.h>
#include "structs.h"
/* The .bss (gMp3Decoder) comes after this file's own header, as in game/Audio/Mp3Decoder/mp3decoder.c. */
#include "conker/mp3.h"

/* The stream front end of the MPEG audio layer III decoder: the bit reservoir, frame sync and header,
 * mp3_open and mp3_decodeFrame. It points the decoder's function pointers at the decoder core
 * (mp3_decodeLayer3 and mp3_readSideInfo of game/Audio/Mp3Decoder/mp3decoder.c) and calls it through them. */

/* .bss, linked at 0x800E1880..0x800E9D00 */
static Mp3Decoder gMp3Decoder; /* the decoder */

/* Appends this frame's main data to the bit reservoir, first sliding the
 * buffer down by half when it would overflow. Returns the byte offset of the
 * new data. */
s32 mp3_appendReservoir(Mp3Decoder *dec) {
    s32 half;
    s32 got;

    half = 0x1000;
    if (dec->mainDataLen + dec->mainDataSize >= 0x1FFC) {
        bcopy(&dec->mainData[half], dec->mainData, half);
        dec->mainDataLen -= half;
        dec->mainDataBit -= half * 8;
    }
    got = dec->read(dec->handle, &dec->mainData[dec->mainDataLen], dec->mainDataSize, -1);
    if (got < dec->mainDataSize) {
        /* zero-fills from mainData[got], not from the read position */
        bzero(&dec->mainData[got], dec->mainDataSize - got);
    }
    dec->streamPos += dec->mainDataSize;
    dec->mainDataLen += dec->mainDataSize;
    return dec->mainDataLen - dec->mainDataSize;
}

/* Finds the next frame sync (optionally seeking to seekPos first, -1 to
 * continue), parses the 4-byte header and CRC, skips frames whose header is
 * invalid or differs from the first one, selects the layer's decoder and reads
 * the side info. Returns 0 at end of stream or on an unsupported layer. */
s32 mp3_syncFrame(Mp3Decoder *dec, s32 seekPos) {
    s32 pos;
    s32 syncLen;
    s32 got;
    u8 mask;

    if (seekPos != -1) {
        dec->streamPos = seekPos;
    }
    pos = seekPos;
    syncLen = 0;
    mask = 0xFF;
    while (1) {
        got = dec->read(dec->handle, &dec->headerBuf[syncLen], 1, pos);
        if (got <= 0) {
            return 0;
        }
        pos = -1;
        dec->streamPos++;
        if (dec->headerBuf[syncLen] != 0xFF && dec->headerBuf[syncLen] != 0xF3) {
            return 0;
        }
        if ((dec->headerBuf[syncLen] & mask) != mask) {
            mask = 0xFF;
            syncLen = 0;
            continue;
        }
        syncLen++;
        if (mask == 0xF0) {
            break;
        }
        mask = 0xF0;
    }
    got = dec->read(dec->handle, &dec->headerBuf[2], 2, -1);
    if (got <= 0) {
        return 0;
    }
    dec->streamPos += 2;
    dec->headerBit = 12; /* past the sync word */
    dec->id = GET_HEADER_BITS(dec, 1);
    dec->layer = GET_HEADER_BITS(dec, 2);
    dec->protectionBit = GET_HEADER_BITS(dec, 1);
    dec->bitrateIndex = GET_HEADER_BITS(dec, 4);
    dec->sampleRateIndex = GET_HEADER_BITS(dec, 2);
    dec->paddingBit = GET_HEADER_BITS(dec, 1);
    dec->privateBit = GET_HEADER_BITS(dec, 1);
    dec->mode = GET_HEADER_BITS(dec, 2);
    dec->modeExtension = GET_HEADER_BITS(dec, 2);
    dec->copyright = GET_HEADER_BITS(dec, 1);
    dec->original = GET_HEADER_BITS(dec, 1);
    dec->emphasis = GET_HEADER_BITS(dec, 2);
    if (dec->bitrateIndex == 15 || dec->sampleRateIndex == 3) {
        return mp3_syncFrame(dec, -1);
    }
    if (dec->haveFirstHeader == 0) {
        dec->haveFirstHeader = 1;
        dec->firstId = dec->id;
        dec->firstLayer = dec->layer;
        dec->firstProtectionBit = dec->protectionBit;
        dec->firstSampleRateIndex = dec->sampleRateIndex;
        dec->firstMode = dec->mode;
        dec->firstCopyright = dec->copyright;
        dec->firstOriginal = dec->original;
    } else if (dec->id != dec->firstId || dec->layer != dec->firstLayer ||
               dec->protectionBit != dec->firstProtectionBit ||
               dec->sampleRateIndex != dec->firstSampleRateIndex ||
               dec->mode != dec->firstMode || dec->original != dec->firstOriginal) {
        return mp3_syncFrame(dec, -1);
    }
    dec->headerLen = 4;
    if (dec->protectionBit == 0) {
        got = dec->read(dec->handle, &dec->headerBuf[4], 2, -1);
        if (got <= 0) {
            return 0;
        }
        dec->streamPos += 2;
        dec->headerBit += 16;
        dec->headerLen = 6;
    }
    if (dec->layer == 1) {
        dec->decodeFrame = mp3_decodeLayer3;
        dec->readSideInfo = mp3_readSideInfo;
    } else if (dec->layer == 2) {
        return 0;
    } else if (dec->layer == 3) {
        return 0;
    }
    if (dec->readSideInfo(dec) == 0) {
        return 0;
    }
    return 1;
}

/* Opens the (single, static) decoder on a stream: sync to the first frame and
 * clear the IMDCT overlap state. Returns NULL if no frame is found. */
Mp3Decoder *mp3_open(s32 handle, Mp3ReadFn read, s32 arg2) {
    Mp3Decoder *dec;

    dec = &gMp3Decoder;
    if (dec == NULL) {
        return NULL;
    }
    dec->unk000C = -1;
    dec->unk0010 = -1;
    dec->unk0014 = -1;
    dec->handle = handle;
    dec->read = read;
    dec->unk0008 = arg2;
    dec->mainDataLen = 0;
    dec->mainDataBit = 0;
    dec->outIndex = 0;
    if (mp3_syncFrame(dec, 0) == 0) {
        return NULL;
    }
    dec->seekPos = 0;
    bzero(dec->overlap, sizeof(dec->overlap[0]));
    return dec;
}

/* Decodes the next frame into the next outBuf slot and returns it through
 * outBlock / outChannels. A frame with the copyright bit set is followed by a
 * NUL-terminated tag string, handed to the stream's tag callback (D_800E0D80.tagCallback). */
s32 mp3_decodeFrame(Mp3Decoder *decoder, u8 **outBlock, s32 *outChannels) {
    Mp3Decoder *dec;
    s32 ret;
    u8 tag[0x100];
    s32 i;

    // matching: the parameter is copied into a local
    dec = decoder;
    dec->outIndex++;
    if (dec->outIndex >= 6) {
        dec->outIndex = 0;
    }
    if (mp3_syncFrame(dec, dec->seekPos) == 0) {
        D_800E0D80.unk84 = 3;
        return 0;
    }
    dec->seekPos = -1;
    ret = dec->decodeFrame(dec);
    if (ret == 0) {
        // matching: a goto to the shared return; an if-block or early return is a jump shorter
        goto end;
    }
    *outBlock = dec->outBuf[dec->outIndex];
    *outChannels = dec->nch;
    if (dec->copyright != 0) {
        i = 0;
        do {
            if (dec->read(dec->handle, &tag[i], 1, -1) == 0) {
                break;
            }
        } while (tag[i++]);
        if (D_800E0D80.tagCallback != NULL) {
            D_800E0D80.tagCallback(0, tag, strlen(tag) + 1);
        }
    }
end:
    return ret;
}
