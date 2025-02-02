#pragma once
#include "shiftbuffer.h"
#include <stdint.h>

typedef uint16_t mux_length_t;
typedef uint16_t cid_t;

typedef struct __attribute__((__packed__))
{
    mux_length_t length;
    cid_t        cid;
    uint8_t      flags;

    char data[];

} mux_frame_t;

enum
{
    kMuxFlagOpen  = 0,
    kMuxFlagClose = 1,
    kMuxFlagFlow  = 2,
    kMuxFlagData  = 3,
    kMuxMinFrameLength = (sizeof(mux_frame_t) - sizeof(mux_length_t)),
    kMuxMaxFrameLength = (1U << (8*sizeof(mux_length_t))) - (1+kMuxMinFrameLength)
};





static void makeOpenFrame(sbuf_t *buf, cid_t cid)
{
    sbufShiftLeft(buf, sizeof(mux_frame_t));
    mux_frame_t frame = {.length = sbufGetBufLength(buf) - sizeof(frame.length), .cid = cid, .flags = kMuxFlagOpen};
    sbufWrite(buf, &frame, sizeof(mux_frame_t));
}

static void makeCloseFrame(sbuf_t *buf, cid_t cid)
{
    sbufShiftLeft(buf, sizeof(mux_frame_t));
    mux_frame_t frame = {.length = sizeof(mux_frame_t) - sizeof(frame.length), .cid = cid, .flags = kMuxFlagClose};
    sbufWrite(buf, &frame, sizeof(mux_frame_t));
}

static void makeDataFrame(sbuf_t *buf, cid_t cid)
{
    sbufShiftLeft(buf, sizeof(mux_frame_t));
    mux_frame_t frame = {.length = sbufGetBufLength(buf) - sizeof(frame.length), .cid = cid, .flags = kMuxFlagData};
    sbufWrite(buf, &frame, sizeof(mux_frame_t));
}
