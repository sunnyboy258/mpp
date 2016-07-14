/*------------------------------------------------------------------------------
--                                                                            --
--       This software is confidential and proprietary and may be used        --
--        only as expressly authorized by a licensing agreement from          --
--                                                                            --
--                            Rockchip Products .                             --
--                                                                            --
--                   (C) COPYRIGHT 2014 ROCKCHIP PRODUCTS                     --
--                            ALL RIGHTS RESERVED                             --
--                                                                            --
--                 The entire notice above must be reproduced                 --
--                  on all copies and should not be removed.                  --
--                                                                            --
--------------------------------------------------------------------------------
--
--  Abstract  :   Encoder instance
--
--------------------------------------------------------------------------------
--
--
--
------------------------------------------------------------------------------*/

#ifndef __H264_INSTANCE_H__
#define __H264_INSTANCE_H__

/*------------------------------------------------------------------------------
    1. Include headers
------------------------------------------------------------------------------*/
#include "enccommon.h"
#include "encpreprocess.h"
#include "encasiccontroller.h"

#include "H264SequenceParameterSet.h"
#include "H264PictureParameterSet.h"
#include "H264Slice.h"
#include "H264RateControl.h"
#include "H264Mad.h"

#ifdef VIDEOSTAB_ENABLED
#include "vidstabcommon.h"
#endif

/*------------------------------------------------------------------------------
    2. External compiler flags
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
    3. Module defines
------------------------------------------------------------------------------*/
enum H264EncStatus {
    H264ENCSTAT_INIT = 0xA1,
    H264ENCSTAT_START_STREAM,
    H264ENCSTAT_START_FRAME,
    H264ENCSTAT_ERROR
};

/* Picture type for encoding */
typedef enum {
    H264ENC_INTRA_FRAME = 0,
    H264ENC_PREDICTED_FRAME = 1,
    H264ENC_NOTCODED_FRAME  /* Used just as a return value */
} H264EncPictureCodingType;

/* Encoder input structure */
typedef struct {
    u32 busLuma;         /* Bus address for input picture
                              * planar format: luminance component
                              * semiplanar format: luminance component
                              * interleaved format: whole picture
                              */
    u32 busChromaU;      /* Bus address for input chrominance
                              * planar format: cb component
                              * semiplanar format: both chrominance
                              * interleaved format: not used
                              */
    u32 busChromaV;      /* Bus address for input chrominance
                              * planar format: cr component
                              * semiplanar format: not used
                              * interleaved format: not used
                              */
    u32 timeIncrement;   /* The previous picture duration in units
                              * of frameRateDenom/frameRateNum.
                              * 0 for the very first picture.
                              */
    u32 *pOutBuf;        /* Pointer to output stream buffer */
    u32 busOutBuf;       /* Bus address of output stream buffer */
    u32 outBufSize;      /* Size of output stream buffer in bytes */
    u32 *pNaluSizeBuf;   /* Output buffer for NAL unit sizes
                              * pNaluSizeBuf[0] = NALU 0 size in bytes
                              * pNaluSizeBuf[1] = NALU 1 size in bytes
                              * etc
                              */
    u32 naluSizeBufSize; /* Size of pNaluSizeBuf in bytes */
    H264EncPictureCodingType codingType;    /* Proposed picture coding type,
                                                 * INTRA/PREDICTED
                                                 */
    u32 busLumaStab;     /* bus address of next picture to stabilize (luminance) */
} H264EncIn;

/* Encoder output structure */
typedef struct {
    H264EncPictureCodingType codingType;    /* Realized picture coding type,
                                                 * INTRA/PREDICTED/NOTCODED
                                                 */
    u32 streamSize;      /* Size of output stream in bytes */
} H264EncOut;

/* Encoder configure parameter */
typedef struct {
    RK_U32 intraPicRate;        // set I frame interval, and is 30 default
} H264EncCfg;


typedef struct {
    u32 encStatus;
    RK_U32 lumWidthSrc;  // TODO  need to think again  modify by lance 2016.06.15
    RK_U32 lumHeightSrc;  // TODO  need to think again  modify by lance 2016.06.15
    u32 mbPerFrame;
    u32 mbPerRow;
    u32 mbPerCol;
    u32 frameCnt;
    u32 fillerNalSize;
    u32 testId;
    stream_s stream;
    preProcess_s preProcess;
    sps_s seqParameterSet;
    pps_s picParameterSet;
    slice_s slice;
    h264RateControl_s rateControl;
    madTable_s mad;
    asicData_s asic;
    const void *inst;
#ifdef VIDEOSTAB_ENABLED
    HWStabData vsHwData;
    SwStbData vsSwData;
#endif
    u32 time_debug_init;
    RK_U32 intraPeriodCnt;  //  count the frame amount from last intra frame,
    // then determine next frame to which type to be encoded
    H264EncIn encIn;        // put input struct into instance, todo    modify by lance 2016.05.31
    H264EncOut encOut;      //  put input struct into instance, todo    modify by lance 2016.05.31
    H264EncCfg h264EncCfg;
} h264Instance_s;

#endif
