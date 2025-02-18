/*
 $License:
    Copyright (C) 2014 InvenSense Corporation, All Rights Reserved.
 $
 */

/******************************************************************************
 *
 * $Id:$
 *
 *****************************************************************************/
 
/**
 * @defgroup INVMPLD
 *
 * @{
 *      @file     invmpld.c
 *      @brief    sensor calibration daemon
 */

#include "invmpld.h"

unsigned char dmp3[] = {
    #include "dmp3Default_20645.txt"
};

//#define PIXART_TEST
#ifdef PIXART_TEST
const unsigned char PPG_Data[][13] = { 
 {1,0x10,0x7f,0x3,0x1,0xa,0x4c,0x44,0x14,0x0,0x0,0x80,0x4c},
 {1,0xdd,0xae,0x4c,0x1,0x5,0x7a,0x77,0x15,0x0,0x0,0x80,0x7a},
 {1,0x8b,0x30,0x20,0x6,0x4,0x9c,0x98,0x16,0x0,0x0,0x80,0x9c},
 {1,0xc6,0xaa,0xa0,0x8,0x4,0x95,0x91,0x17,0x0,0x0,0x80,0x95},
 {1,0xa2,0x59,0x77,0x8,0x4,0x95,0x91,0x18,0x0,0x0,0x80,0x95},
 {1,0xa4,0xcd,0x76,0x8,0x4,0x9a,0x96,0x19,0x0,0x0,0x80,0x9a},
 {1,0xf,0xbf,0xa8,0x8,0x4,0x9c,0x98,0x1a,0x0,0x0,0x80,0x9c},
 {1,0x7c,0x70,0xdd,0x8,0x3,0xa1,0x9e,0x1b,0x0,0x0,0x80,0xa1},
 {1,0xc6,0x16,0xa,0x9,0x7,0xa3,0x9f,0x1c,0x0,0x0,0x80,0xa3},
 {1,0x7d,0x84,0x2d,0x9,0x4,0xa5,0xa1,0x1d,0x0,0x0,0x80,0xa5},
 {1,0xa0,0x31,0x4f,0x9,0x4,0xa7,0xa3,0x1e,0x0,0x0,0x80,0xa7},
 {1,0xe9,0xff,0x6c,0x9,0x5,0xa9,0xa4,0x1f,0x0,0x0,0x80,0xa9},
 {1,0xa0,0xf7,0x85,0x9,0x4,0xaa,0xa6,0x20,0x0,0x0,0x80,0xaa},
 {1,0x7d,0x10,0x9b,0x9,0x4,0xad,0xa9,0x21,0x0,0x0,0x80,0xad},
 {1,0xc8,0x4a,0xaf,0x9,0x5,0xae,0xa9,0x22,0x0,0x0,0x80,0xae},
 {1,0xa1,0x61,0xc0,0x9,0x5,0xae,0xa9,0x23,0x0,0x0,0x80,0xae},
 {1,0xc5,0x56,0xce,0x9,0x4,0xae,0xaa,0x24,0x0,0x0,0x80,0xae},
 {1,0xa2,0xab,0xd2,0x9,0x4,0xae,0xaa,0x25,0x0,0x0,0x80,0xae},
 {1,0xe,0x33,0xdb,0x9,0x4,0xaf,0xab,0x26,0x0,0x0,0x80,0xaf},
 {1,0xa4,0x23,0xea,0x9,0x4,0xb1,0xad,0x27,0x0,0x0,0x80,0xb1},
 {1,0xa4,0x23,0xfa,0x9,0x4,0xb2,0xae,0x28,0x0,0x0,0x80,0xb2},
 {1,0xc5,0xca,0x8,0xa,0x4,0xb2,0xae,0x29,0x0,0x0,0x80,0xb2},
 {1,0x7c,0xb4,0x15,0xa,0x4,0xb3,0xaf,0x2a,0x0,0x0,0x80,0xb3},
 {1,0xeb,0x79,0x1f,0xa,0x5,0xb4,0xaf,0x2b,0x0,0x0,0x80,0xb4},
 {1,0xeb,0xf,0x28,0xa,0x4,0xb5,0xb1,0x2c,0x0,0x0,0x80,0xb5},
 {1,0xa1,0x1d,0x35,0xa,0x4,0xb6,0xb2,0x2d,0x0,0x0,0x80,0xb6},
 {1,0xec,0xdd,0x3d,0xa,0x4,0xb7,0xb3,0x2e,0x0,0x0,0x80,0xb7},
 {1,0xc4,0xe4,0x47,0xa,0x4,0xb8,0xb4,0x2f,0x0,0x0,0x80,0xb8},
 {1,0x7d,0x5a,0x51,0xa,0x5,0xb8,0xb4,0x30,0x0,0x0,0x80,0xb8},
 {1,0x56,0xad,0x5b,0xa,0x4,0xb8,0xb4,0x31,0x0,0x0,0x80,0xb8},
 {1,0x7b,0xa,0x64,0xa,0x5,0xb9,0xb4,0x32,0x0,0x0,0x80,0xb9},
 {1,0xa3,0x5b,0x65,0xa,0x5,0xb9,0xb4,0x33,0x0,0x0,0x80,0xb9},
 {1,0x34,0x8e,0x65,0xa,0x7,0xb9,0xb2,0x34,0x0,0x0,0x80,0xb9},
 {1,0x34,0xbe,0x6e,0xa,0x4,0xba,0xb6,0x35,0x0,0x0,0x80,0xba},
 {1,0xea,0x53,0x78,0xa,0x4,0xba,0xb6,0x36,0x0,0x0,0x80,0xba},
 {1,0xc4,0x8e,0x82,0xa,0x4,0xbb,0xb7,0x37,0x0,0x0,0x80,0xbb},
 {1,0xec,0xe1,0x8b,0xa,0x5,0xbc,0xb7,0x38,0x0,0x0,0x80,0xbc},
 {1,0x10,0xcb,0x94,0xa,0x5,0xbc,0xb7,0x39,0x0,0x0,0x80,0xbc},
 {1,0xe9,0xcf,0x9a,0xa,0x5,0xbd,0xb8,0x3a,0x0,0x0,0x80,0xbd},
 {1,0x35,0x4e,0xa3,0xa,0x4,0xbd,0xb9,0x3b,0x0,0x0,0x80,0xbd},
 {1,0xc6,0xa4,0xab,0xa,0x4,0xbe,0xba,0x3c,0x0,0x0,0x80,0xbe},
 {1,0xc7,0xbe,0xb2,0xa,0x4,0xbe,0xba,0x3d,0x0,0x0,0x80,0xbe},
 {1,0x57,0x29,0xb9,0xa,0x4,0xbe,0xba,0x3e,0x0,0x0,0x80,0xbe},
 {1,0x10,0x61,0xc1,0xa,0x5,0xbf,0xba,0x3f,0x0,0x0,0x80,0xbf},
 {1,0xa0,0xfd,0xc7,0xa,0x5,0xc0,0xbb,0x40,0x0,0x0,0x80,0xc0},
 {1,0xea,0xcb,0xc6,0xa,0x5,0xbf,0xba,0x41,0x0,0x0,0x80,0xbf},
 {1,0xc5,0x9a,0xc0,0xa,0x4,0xbf,0xbb,0x42,0x0,0x0,0x80,0xbf},
 {1,0x57,0x45,0xc1,0xa,0x4,0xbf,0xbb,0x43,0x0,0x0,0x80,0xbf},
 {1,0x33,0x74,0xc7,0xa,0x4,0xc0,0xbc,0x44,0x0,0x0,0x80,0xc0},
 {1,0xc5,0x8a,0xce,0xa,0x4,0xc1,0xbd,0x45,0x0,0x0,0x80,0xc1},
 {1,0x10,0x35,0xd7,0xa,0x4,0xc1,0xbd,0x46,0x0,0x0,0x80,0xc1},
 {1,0x5a,0xa5,0xdd,0xa,0x4,0xc1,0xbd,0x47,0x0,0x0,0x80,0xc1},
 {1,0xa1,0xa7,0xe0,0xa,0x5,0xc1,0xbc,0x48,0x0,0x0,0x80,0xc1},
 {1,0xc6,0xae,0xe5,0xa,0x6,0xc2,0xbc,0x49,0x0,0x0,0x80,0xc2},
 {1,0xc3,0xbe,0xed,0xa,0x5,0xc3,0xbe,0x4a,0x0,0x0,0x80,0xc3},
 {1,0xd,0xed,0xf3,0xa,0x4,0xc3,0xbe,0x4b,0x0,0x0,0x80,0xc3},
 {1,0xeb,0x97,0xf9,0xa,0x4,0xc3,0xbf,0x4c,0x0,0x0,0x80,0xc3},
 {1,0xc8,0x90,0xff,0xa,0x5,0xc4,0xbf,0x4d,0x0,0x0,0x80,0xc4},
 {1,0xa3,0x6b,0x5,0xb,0x5,0xc4,0xc0,0x4e,0x0,0x0,0x80,0xc4},
 {1,0x7f,0xc,0xff,0xa,0x4,0xc2,0xbe,0x4f,0x0,0x0,0x80,0xc2},
 {1,0x11,0x2b,0xf8,0xa,0x5,0xc3,0xbe,0x50,0x0,0x0,0x80,0xc3},
 {1,0x32,0x40,0xf9,0xa,0x5,0xc3,0xbe,0x51,0x0,0x0,0x80,0xc3},
 {1,0x56,0x25,0xfc,0xa,0x5,0xc4,0xbf,0x52,0x0,0x0,0x80,0xc4},
 {1,0xa3,0xef,0x1,0xb,0x5,0xc4,0xc0,0x53,0x0,0x0,0x80,0xc4},
 {1,0x30,0x4a,0x8,0xb,0x4,0xc4,0xc0,0x54,0x0,0x0,0x80,0xc4},
 {1,0x5a,0x37,0xb,0xb,0x4,0xc5,0xc0,0x55,0x0,0x0,0x80,0xc5},
 {1,0x59,0x13,0x11,0xb,0x5,0xc6,0xc1,0x56,0x0,0x0,0x80,0xc6},
 {1,0x5a,0xb1,0x15,0xb,0x4,0xc6,0xc2,0x57,0x0,0x0,0x80,0xc6},
 {1,0x58,0x1,0x1d,0xb,0x4,0xc5,0xc1,0x58,0x0,0x0,0x80,0xc5},
 {1,0x36,0x72,0x21,0xb,0x4,0xc6,0xc2,0x59,0x0,0x0,0x80,0xc6},
 {1,0xa1,0xd9,0x26,0xb,0x4,0xc6,0xc2,0x5a,0x0,0x0,0x80,0xc6},
 {1,0x31,0xbe,0x2b,0xb,0x4,0x8d,0x89,0x5b,0x0,0x0,0x80,0x8d},
 {1,0x69,0x6c,0x2a,0xb,0x4,0x8d,0x89,0x5c,0x0,0x0,0x80,0x8d},
 {1,0xb6,0x6a,0x1f,0xb,0x4,0x8c,0x88,0x5d,0x0,0x0,0x80,0x8c},
 {1,0x86,0x49,0x10,0xb,0x5,0x8c,0x87,0x5e,0x0,0x0,0x80,0x8c},
 {1,0xb9,0xd0,0x9,0xb,0x4,0x8c,0x88,0x5f,0x0,0x0,0x80,0x8c},
 {1,0x1e,0x1f,0xa,0xb,0x4,0x8c,0x88,0x60,0x0,0x0,0x80,0x8c},
 {1,0xb7,0xbe,0x9,0xb,0x4,0x8d,0x89,0x61,0x0,0x0,0x80,0x8d},
 {1,0xeb,0x33,0x12,0xb,0x4,0x8d,0x89,0x62,0x0,0x0,0x80,0x8d},
 {1,0xb9,0xea,0x16,0xb,0x5,0x8d,0x88,0x63,0x0,0x0,0x80,0x8d},
 {1,0x51,0x6e,0x19,0xb,0x4,0x8d,0x89,0x64,0x0,0x0,0x80,0x8d},
 {1,0x82,0x5b,0x1b,0xb,0x4,0x8d,0x89,0x65,0x0,0x0,0x80,0x8d},
 {1,0xed,0x91,0x1f,0xb,0x4,0x8d,0x89,0x66,0x0,0x0,0x80,0x8d},
 {1,0xb8,0x12,0x26,0xb,0x4,0x8e,0x8a,0x67,0x0,0x0,0x80,0x8e},
 {1,0xb9,0x70,0x2b,0xb,0x4,0x8e,0x8a,0x68,0x0,0x0,0x80,0x8e},
 {1,0xb7,0x66,0x2f,0xb,0x4,0x8e,0x8a,0x69,0x0,0x0,0x80,0x8e},
 {1,0x1f,0x5d,0x33,0xb,0x4,0x8e,0x8a,0x6a,0x0,0x0,0x80,0x8e},
 {1,0xb9,0xa,0x39,0xb,0x5,0x8e,0x89,0x6b,0x0,0x0,0x80,0x8e},
 {1,0x86,0xb,0x2c,0xb,0x4,0x8d,0x89,0x6c,0x0,0x0,0x80,0x8d},
 {1,0xb8,0x12,0x1d,0xb,0x4,0x8c,0x88,0x6d,0x0,0x0,0x80,0x8c},
 {1,0xec,0x8d,0x13,0xb,0x4,0x8c,0x88,0x6e,0x0,0x0,0x80,0x8c},
 {1,0xb7,0x2e,0x10,0xb,0x4,0x8c,0x88,0x6f,0x0,0x0,0x80,0x8c},
 {1,0x21,0xd5,0x10,0xb,0x5,0x8c,0x87,0x70,0x0,0x0,0x80,0x8c},
 {1,0x51,0xb8,0x13,0xb,0x5,0x8c,0x87,0x71,0x0,0x0,0x80,0x8c},
 {1,0xb6,0x7a,0x15,0xb,0x4,0x8d,0x88,0x72,0x0,0x0,0x80,0x8d},
 {1,0x53,0x30,0x18,0xb,0x4,0x8d,0x89,0x73,0x0,0x0,0x80,0x8d},
 {1,0x86,0xdb,0x1b,0xb,0x4,0x8d,0x89,0x74,0x0,0x0,0x80,0x8d},
 {1,0x84,0xa9,0x20,0xb,0x4,0x8d,0x89,0x75,0x0,0x0,0x80,0x8d},
 {1,0x20,0x5,0x26,0xb,0x4,0x8d,0x89,0x76,0x0,0x0,0x80,0x8d},
 {1,0x52,0xe4,0x27,0xb,0x4,0x8d,0x89,0x77,0x0,0x0,0x80,0x8d},
 {1,0x86,0x7b,0x2b,0xb,0x5,0x8e,0x89,0x78,0x0,0x0,0x80,0x8e},
 {1,0xed,0x3b,0x32,0xb,0x4,0x8e,0x8a,0x79,0x0,0x0,0x80,0x8e},
 {1,0xec,0x1d,0x31,0xb,0x4,0x8d,0x89,0x7a,0x0,0x0,0x80,0x8d},
 {1,0x50,0xa0,0x1d,0xb,0x4,0x8c,0x88,0x7b,0x0,0x0,0x80,0x8c},
 {1,0xb8,0x16,0xb,0xb,0x6,0x8c,0x86,0x7c,0x0,0x0,0x80,0x8c},
 {1,0xee,0x3,0x2,0xb,0x4,0x8b,0x87,0x7d,0x0,0x0,0x80,0x8b},
 {1,0xba,0x2a,0xfc,0xa,0x4,0x8b,0x87,0x7e,0x0,0x0,0x80,0x8b},
 {1,0xeb,0x2d,0xfe,0xa,0x4,0x8c,0x88,0x7f,0x0,0x0,0x80,0x8c},
 {1,0x54,0x4,0x1,0xb,0x4,0x8b,0x87,0x80,0x0,0x0,0x80,0x8b},
 {1,0xef,0xf,0x1,0xb,0x5,0x8b,0x86,0x81,0x0,0x0,0x80,0x8b},
 {1,0xb9,0xca,0x2,0xb,0x3,0x8c,0x89,0x82,0x0,0x0,0x80,0x8c},
 {1,0xba,0xd2,0x5,0xb,0x4,0x8c,0x87,0x83,0x0,0x0,0x80,0x8c},
 {1,0x20,0xa5,0xa,0xb,0x5,0x8c,0x87,0x84,0x0,0x0,0x80,0x8c},
 {1,0x86,0x2d,0x10,0xb,0x4,0x8d,0x87,0x85,0x0,0x0,0x80,0x8d},
 {1,0xb8,0xd6,0x12,0xb,0x4,0x8c,0x88,0x86,0x0,0x0,0x80,0x8c},
 {1,0x52,0x68,0x15,0xb,0x4,0x8d,0x89,0x87,0x0,0x0,0x80,0x8d},
 {1,0x52,0x92,0x1a,0xb,0x5,0x8c,0x87,0x88,0x0,0x0,0x80,0x8c},
 {1,0x20,0x97,0xc,0xb,0x4,0x8a,0x86,0x89,0x0,0x0,0x80,0x8a},
 {1,0x1e,0xf5,0xf1,0xa,0x5,0x89,0x84,0x8a,0x0,0x0,0x80,0x89},
 {1,0x54,0xec,0xdc,0xa,0x4,0x89,0x85,0x8b,0x0,0x0,0x80,0x89},
 {1,0xeb,0xb1,0xd3,0xa,0x4,0x86,0x82,0x8c,0x0,0x0,0x80,0x86},
 {1,0x88,0x11,0xcf,0xa,0x4,0x89,0x85,0x8d,0x0,0x0,0x80,0x89},
 {1,0x20,0xcd,0xd3,0xa,0x4,0x89,0x85,0x8e,0x0,0x0,0x80,0x89},
 {1,0xb9,0x5a,0xd1,0xa,0x4,0x89,0x85,0x8f,0x0,0x0,0x80,0x89},
 {1,0xb6,0x6c,0xd4,0xa,0x4,0x89,0x85,0x90,0x0,0x0,0x80,0x89},
 {1,0xed,0xbf,0xd8,0xa,0x5,0x8a,0x85,0x91,0x0,0x0,0x80,0x8a},
 {1,0x20,0x73,0xdc,0xa,0x4,0x8a,0x86,0x92,0x0,0x0,0x80,0x8a},
 {1,0x85,0x23,0xe1,0xa,0x4,0x8a,0x86,0x93,0x0,0x0,0x80,0x8a},
 {1,0x53,0x64,0xe5,0xa,0x5,0x8a,0x85,0x94,0x0,0x0,0x80,0x8a},
 {1,0xba,0x76,0xe8,0xa,0x5,0x8b,0x86,0x95,0x0,0x0,0x80,0x8b},
 {1,0x1f,0xc5,0xee,0xa,0x5,0x89,0x85,0x96,0x0,0x0,0x80,0x89},
 {1,0x85,0x2d,0xdf,0xa,0x4,0x88,0x84,0x97,0x0,0x0,0x80,0x88},
 {1,0x54,0xbc,0xc1,0xa,0x4,0x87,0x83,0x98,0x0,0x0,0x80,0x87},
 {1,0xbc,0xb2,0xab,0xa,0x5,0x87,0x82,0x99,0x0,0x0,0x80,0x87},
 {1,0xb8,0xd8,0x9f,0xa,0x5,0x86,0x81,0x9a,0x0,0x0,0x80,0x86},
 {1,0x51,0xb8,0x98,0xa,0x4,0x86,0x82,0x9b,0x0,0x0,0x80,0x86},
 {1,0x84,0x3d,0x9c,0xa,0x4,0x86,0x82,0x9c,0x0,0x0,0x80,0x86},
 {1,0xb7,0x92,0x9c,0xa,0x4,0x86,0x82,0x9d,0x0,0x0,0x80,0x86},
 {1,0xeb,0xa3,0x9d,0xa,0x5,0x87,0x82,0x9e,0x0,0x0,0x80,0x87},
 {1,0x87,0x99,0xa0,0xa,0x4,0x87,0x83,0x9f,0x0,0x0,0x80,0x87},
 {1,0xb8,0x5e,0xa6,0xa,0x4,0x88,0x84,0xa0,0x0,0x0,0x80,0x88},
 {1,0x83,0x97,0xa9,0xa,0x5,0x87,0x83,0xa1,0x0,0x0,0x80,0x87},
 {1,0xec,0x77,0xaf,0xa,0x4,0x88,0x84,0xa2,0x0,0x0,0x80,0x88},
 {1,0xec,0x85,0xb3,0xa,0x4,0x88,0x84,0xa3,0x0,0x0,0x80,0x88},
 {1,0x52,0x54,0xb9,0xa,0x4,0x87,0x84,0xa4,0x0,0x0,0x80,0x87}, 
 {1,0xb8,0xe6,0xb0,0xa,0x4,0x86,0x82,0xa5,0x0,0x0,0x80,0x86},
 {1,0x1e,0xbd,0x89,0xa,0x4,0x84,0x80,0xa6,0x0,0x0,0x80,0x84},
 {1,0xec,0xa3,0x62,0xa,0x5,0x82,0x7d,0xa7,0x0,0x0,0x80,0x82},
 {1,0x52,0x84,0x4d,0xa,0x4,0x82,0x7e,0xa8,0x0,0x0,0x80,0x82},
 {1,0x54,0x80,0x40,0xa,0x4,0x82,0x7e,0xa9,0x0,0x0,0x80,0x82},
 {1,0xea,0xfb,0x3e,0xa,0x4,0x82,0x7e,0xaa,0x0,0x0,0x80,0x82},
 {1,0x86,0xa5,0x3f,0xa,0x5,0x82,0x7d,0xab,0x0,0x0,0x80,0x82},
 {1,0xb9,0x6e,0x3d,0xa,0x4,0x82,0x7e,0xac,0x0,0x0,0x80,0x82},
};

float verify_library() { 
	int i = 0 ;  
	float MEMS_Data[3] = {0 ,0, 0};  
	float myHR = 0 ;  

	for(i=0; i<sizeof(PPG_Data)/sizeof(PPG_Data[0]); i++)  {   
		PxiAlg_Process((unsigned char*)PPG_Data[i], MEMS_Data);  
	}  

	PxiAlg_HrGet(&myHR);  
	return myHR ; 
} 
#endif

int icm30628_fd = -1;
static bool mHaveGoodMpuCal = false;
static int mAccelAccuracy = 0;
static int mGyroAccuracy = 0;
static signed char orientation[9]= {0};
static long s_quat_chip_to_body[4] = {(1L<<30), 0, 0, 0};

void kernel_log(const char *format, ...)
{
	va_list arglist;
	char str[1024] ={0}; 
	va_start(arglist, format);
	vsprintf(str, format, arglist);
	ioctl(icm30628_fd, ICM30628_KERNEL_LOG, str);
	va_end(arglist);	
} 

int icm30628_open(int *fd)
{
	if(*fd <= 0){
		*fd = open(DEVICE_NAME, O_RDONLY);	
		if(*fd <= 0){
			return -errno;
		}    
	}
	return 0;
}

int icm30628_close(int fd)
{
	if(fd > 0){
		close(fd);        
	}  
	return 0;
}

#ifdef PIXART_HRM_LIBRARY
void inv_open_HRM_library(void)
{
	PxiAlg_Open(20); // library expects input frequency is always 20Hz
	return;
}

void inv_close_HRM_library(void)
{
	PxiAlg_Close();
	return;
}

int inv_get_HRM_grade(void)
{
	int ret = 0;

	ret = PxiAlg_GetSigGrade();

	return ret;
}

int inv_get_HRM_touch_flag(void)
{
	int ret = 0;

	ret = PxiAlg_GetTouchFlag();

	return ret;
}

int inv_get_HRM_ready_flag(void)
{
	int ret = 0;

	ret = PxiAlg_GetReadyFlag();

	return ret;
}

int inv_get_HRM_motion_flag(void)
{
	int ret = 0;

	ret = PxiAlg_GetMotionFlag();

	return ret;
}

int inv_get_HRM_version(void)
{
	int ret = 0;

	ret = PxiAlg_Version();
	kernel_log("PxiAlg version is %d\n", ret);

	return ret;
}

int inv_process_HRM_data(void)
{
	int ret = 0;
	char ppg_data[13 + 4] = {0,};
	float mems_data[3] = {0.0,};
	float hr_f = 0.0f;
	int hr[2] = {0,};

	kernel_log("inv_process_HRM_data(), received data from kernel\n");

	ret = ioctl(icm30628_fd, ICM30628_GET_HRM_DATA, ppg_data);
	if(ret){
		kernel_log("error - inv_process_HRM_data(), ICM30628_GET_HRM_DATA\n");
		return -1;
	}				

	kernel_log("hr data = %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n"
		,ppg_data[0]
		,ppg_data[1]
		,ppg_data[2]
		,ppg_data[3]
		,ppg_data[4]
		,ppg_data[5]
		,ppg_data[6]
		,ppg_data[7]
		,ppg_data[8]
		,ppg_data[9]
		,ppg_data[10]
		,ppg_data[11]
		,ppg_data[12]
		);

	ret = PxiAlg_Process(ppg_data , mems_data);
	kernel_log("PxiAlg_Process(), return value is %d\n", ret);
	PxiAlg_HrGet(&hr_f);  
	kernel_log("PxiAlg_HrGet(), return value is %f\n", hr_f);
	hr[0] = hr_f * 1000000;
	memcpy(&hr[1], &ppg_data[13], sizeof(int));//timestamp
	ret = ioctl(icm30628_fd, ICM30628_SEND_HRM_DATA, hr );
	if(ret){
		kernel_log("error - inv_process_HRM_data(), ICM30628_GET_HRM_DATA\n");
		return -1;
	}				

	return ret;
}
#endif

void inv_get_rotation(float r[3][3], long quat_9_axis[4])
{
    long rot[9];
    float conv = 1.f / (1L<<30);

    inv_quaternion_to_rotation(quat_9_axis, rot);
    r[0][0] = rot[0]*conv;
    r[0][1] = rot[1]*conv;
    r[0][2] = rot[2]*conv;
    r[1][0] = rot[3]*conv;
    r[1][1] = rot[4]*conv;
    r[1][2] = rot[5]*conv;
    r[2][0] = rot[6]*conv;
    r[2][1] = rot[7]*conv;
    r[2][2] = rot[8]*conv;
}

void google_orientation(float *g, long quat_9_axis[4])
{
    float rad2deg = (float)(180.0 / M_PI);
    float R[3][3];

    inv_get_rotation(R, quat_9_axis);

    g[0] = atan2f(-R[1][0], R[0][0]) * rad2deg;
    g[1] = atan2f(-R[2][1], R[2][2]) * rad2deg;
    g[2] = asinf ( R[2][0])          * rad2deg;
    if (g[0] < 0)
        g[0] += 360;

	g[0] = 540 - g[0];
	if(g[0] >  360){
		g[0] = g[0] - 360;
	}

	if(g[1] > 0){
		g[1] = 180 - g[1];
	} else {
		g[1] = -180 - g[1];
	}	
}

int inv_process_orientation_data(void)
{
	int ret = 0;
	long quat[4] = {0,};
	int kernel_quat[5] = {0,};
	float orientation[3] = {0.0,};
	int kernel_orientation[4] = {0,};

	ret = ioctl(icm30628_fd, ICM30628_GET_ORIENTATION_DATA, kernel_quat);
	if(ret){
		kernel_log("error - inv_process_orientation_data(), ICM30628_GET_ORIENTATION_DATA\n");
		return -1;
	}				

	quat[0] = kernel_quat[1];
	quat[1] = kernel_quat[2];
	quat[2] = kernel_quat[3];
	quat[3] = kernel_quat[0];
	google_orientation(orientation, quat);
	kernel_orientation[0] = orientation[0] * 1000000;
	kernel_orientation[1] = orientation[1] * 1000000;
	kernel_orientation[2] = orientation[2] * 1000000;
	
	memcpy(&kernel_orientation[3], &kernel_quat[4], sizeof(int)); //timestamp

	ret = ioctl(icm30628_fd, ICM30628_SEND_ORIENTATION_DATA, kernel_orientation);
	if(ret){
		kernel_log("error - inv_process_orientation_data(), ICM30628_SEND_ORIENTATION_DATA\n");
		return -1;
	}					

	return ret;
}

void inv_set_chip_to_body_axis_quaternion(signed char *accel_gyro_matrix, float angle)
{
	int i;
	float rot[9];
	long qcb[4],q_all[4];
	long q_adjust[4];
	
	for (i=0; i<9; i++) {
		rot[i] = (float)accel_gyro_matrix[i];
	}
	
	inv_rotation_to_quaternion(rot, qcb);
	
	qcb[1] = -qcb[1];
	qcb[2] = -qcb[2];
	qcb[3] = -qcb[3];

	q_adjust[0] = (long)((1L<<30) * cos(-angle*M_PI/180./2.));
	q_adjust[1] = 0;
	q_adjust[2] = (long)((1L<<30)*sin(-angle*M_PI/180./2.));
	q_adjust[3] = 0;
	
	inv_q_mult(q_adjust, qcb, s_quat_chip_to_body);
}

int downloadFirmware()
{
	int ret = 0;
	FILE *fp;
	unsigned int fsize, readbytes;
	unsigned char *firmware;

	fp = fopen(FIRMWARE_FILE,"rb");
	if (fp == NULL) {
		kernel_log("error - downloadFirmware(), fopen\n");
		ret =  -1;
		return ret;
	}

	fseek (fp, 0 , SEEK_END);
	fsize = ftell (fp);
	rewind (fp);

	kernel_log("downloadFirmware(), firmware size = %d\n", fsize);

	firmware = (unsigned char *)malloc(fsize);

	readbytes = fread(firmware, 1, fsize, fp);
	if (readbytes  != fsize) {
		kernel_log("error - downloadFirmware(), readbytes  != fsize\n");
		free(firmware);
		ret = -1;
		return ret;
	}

	kernel_log("downloadFirmware(), read firmware size = %d\n", readbytes);

	ret = ioctl(icm30628_fd, ICM30628_FIRMWARE_SIZE, &fsize);
	if(ret){
		kernel_log("error - downloadFirmware(), ICM30628_FIRMWARE_SIZE\n");
		free(firmware);
		return -1;
	}				

	ret = ioctl(icm30628_fd, ICM30628_DOWNLOAD_FIRMWARE, firmware);
	if(ret){
		kernel_log("error - downloadFirmware(), ICM30628_DOWNLOAD_FIRMWARE\n");
		free(firmware);
		return -1;
	}				

	free(firmware);

	return ret;
}

int downloadDMP()
{
	int ret = 0;
	unsigned int dmpsize, readbytes;
	unsigned char *dmp;
	FILE *fp;

	dmpsize = sizeof(dmp3);
	dmp = dmp3;

	ret = ioctl(icm30628_fd, ICM30628_DMP3_SIZE, &dmpsize);
	if(ret){
		kernel_log("error - downloadDMP(), ICM30628_DMP3_SIZE\n");
		return -1;
	}				

	ret = ioctl(icm30628_fd, ICM30628_DOWNLOAD_DMP3, dmp);
	if(ret){
		kernel_log("error - (), ICM30628_DOWNLOAD_DMP3\n");
		return -1;
	}				

	fp = fopen(DMP4_FILE,"rb");
	if (fp == NULL) {
		kernel_log("error - downloadDMP(), fopen, dmp4\n");
		ret =  -1;
		return ret;
	}

	fseek (fp, 0 , SEEK_END);
	dmpsize = ftell (fp);
	rewind (fp);

	kernel_log("downloadDMP(), dmp4 size = %d\n", dmpsize);

	dmp = (unsigned char *)malloc(dmpsize);

	readbytes = fread(dmp, 1, dmpsize, fp);
	if (readbytes  != dmpsize) {
		kernel_log("error - downloadDMP(), readbytes  != dmpsize\n");
		free(dmp);
		ret = -1;
		return ret;
	}


	ret = ioctl(icm30628_fd, ICM30628_DMP4_SIZE, &dmpsize);
	if(ret){
		kernel_log("error - downloadDMP(), ICM30628_DMP4_SIZE\n");
		free(dmp);
		return -1;
	}				

	ret = ioctl(icm30628_fd, ICM30628_DOWNLOAD_DMP4, dmp);
	if(ret){
		kernel_log("error - downloadDMP(), ICM30628_DOWNLOAD_DMP4\n");
		free(dmp);
		return -1;
	}				

	free(dmp);

	return ret;
}


int invensense_read_cal(unsigned char *cal, size_t len)
{
	int ret = 0;
	FILE *fp;
	size_t fsize, bytesRead;

	kernel_log("invensense_read_cal()\n");

	fp = fopen(CALIBRATION_FILE,"rb");
	if (fp == NULL) {
		kernel_log("error - invensense_read_cal(), fopen\n");
		ret = -1;
		goto read_cal_end;
	}

	fseek (fp, 0 , SEEK_END);
	fsize = ftell (fp);
	rewind (fp);

	bytesRead = fread(cal, 1, fsize, fp);
	if (bytesRead != fsize) {
		kernel_log("error - invensense_read_cal(), fread\n");
		ret = -1;
		goto read_cal_end;
	}
	if (bytesRead != len) {
		kernel_log("error - invensense_read_cal(), size is not matched\n");
		ret = -1;
		goto read_cal_end;
	}

	read_cal_end:	
	if(fp){
		fclose(fp);
	}

	if(ret < 0){
		memset(cal, 0, len);
	}
	
	return ret;
}

int invensense_write_cal(unsigned char *cal, size_t len)
{
	int ret = 0;
	FILE *fp;
	int bytesWritten;

	kernel_log("invensense_write_cal()\n");

	if (len <= 0) {
		return -1;
	}

	fp = fopen(CALIBRATION_FILE,"wb");
	if (fp == NULL) {
		kernel_log("error - invensense_write_cal(), fopen\n");
		return -1;
	}

	kernel_log("invensense_write_cal(), fopen is ok\n");

	bytesWritten = fwrite(cal, 1, len, fp);
	if (bytesWritten != len) {
		kernel_log("error - invensense_write_cal(), fwrite\n");
		ret = -1;
		goto write_cal_end;		
	}

	kernel_log("invensense_write_cal(), fwrite is ok\n");

	write_cal_end:	
	fclose(fp);
	return ret;
}

int invensense_load_calibration(void)
{
	int ret = 0;
	struct icm30628_calibration_info_t calibration;

	kernel_log("invensense_load_calibration()\n");

	ret = invensense_read_cal((unsigned char *)&calibration, sizeof(calibration));
	if(ret < 0){
		//kernel_log("error - invensense_read_cal(), invensense_load_calibration\n");
		return ret;
	}

	ret = ioctl(icm30628_fd, ICM30628_LOAD_CAL, (void *)&calibration);
	if(ret){
		kernel_log("error - invensense_load_calibration(), ICM30628_LOAD_CAL\n");
		return -1;
	}

	return ret;
}

int invensense_store_calibration(void)
{
	int ret = 0;
	struct icm30628_calibration_info_t calibration;

	kernel_log("invensense_store_calibration()\n");

	ret = ioctl(icm30628_fd, ICM30628_STORE_CAL, (void *)&calibration);
	if(ret){
		kernel_log("error - invensense_store_calibration(), ICM30628_STORE_CAL\n");
		return -1;
	}

	ret = invensense_write_cal((unsigned char *)&calibration, sizeof(calibration));
	if(ret < 0){
		kernel_log("error - invensense_write_cal(), invensense_store_calibration\n");
		return ret;
	}

	return ret;
}

void inv_convert_quaternion_to_body2(long *mcb, long *qcb)
{
	float Rcb[9];
	int i;

	for (i=0; i<9; i++)
		Rcb[i] = (float)mcb[i];
	
	inv_rotation_to_quaternion(Rcb, qcb);

	return;
}

int invensense_set_orientation()
{
	int ret = 0;
	unsigned short orientId;
	long mtx[9]= {0};
	
	ret = ioctl(icm30628_fd, ICM30628_GET_ORIENTATION, &orientation);
	if(ret){
		kernel_log("error - invensense_set_orientation(), ICM30628_GET_ORIENTATION\n");
		return -1;
	}

	orientId = inv_orientation_matrix_to_scalar((const signed char *)orientation);
	inv_orientation_scalar_to_matrix(orientId, mtx);	
	inv_convert_quaternion_to_body2(mtx, s_quat_chip_to_body);
	ret = ioctl(icm30628_fd, ICM30628_SEND_ORIENTATION, s_quat_chip_to_body);
	if(ret){
		kernel_log("error - invensense_set_orientation(), ICM30628_SEND_ORIENTATION\n");
		return -1;
	}				

	return ret;
}

int initialize()
{
	int ret = 0;

	ret = downloadFirmware();
	if(ret < 0){
		kernel_log("error - initialize(), downloadFirmware\n");
		return ret;
	}

	ret = downloadDMP();
	if(ret < 0){
		kernel_log("error - initialize(), downloadDMP\n");
		return ret;
	}

	ret = invensense_set_orientation();
	if(ret < 0){
		kernel_log("error - initialize(), invensense_set_orientation\n");
	}

	ret = invensense_load_calibration();
	if(ret < 0){
		//kernel_log("error - initialize(), invensense_load_calibration\n");
	}

#ifdef PIXART_HRM_LIBRARY
	inv_open_HRM_library();
	inv_get_HRM_version();

#ifdef PIXART_TEST
	kernel_log("PIXART TEST = %f\n", verify_library());
#endif
#endif

	return 0; // must be success
}

void receiveRequest(int n, siginfo_t *info, void *unused) 
{	
	int ret = 0;

	switch(info->si_int){
		case REQUEST_SIGNAL_LOAD_CALIBRATION:
			invensense_load_calibration();
			break;
		case REQUEST_SIGNAL_STORE_CALIBRATION:
			invensense_store_calibration();
			break;
#ifdef PIXART_HRM_LIBRARY
		case REQUEST_SIGNAL_PROCESS_HRM:			
			inv_process_HRM_data();
			break;	
#endif
		case REQUEST_SIGNAL_PROCESS_ORIENTATION:	
			inv_process_orientation_data();
			break;	
		default :
			break;
	}	
}

int sendPID(int pid) 
{	
	int ret = 0;

	ret = ioctl(icm30628_fd, ICM30628_WRITE_DAEMON_PID, &pid);
	if(ret){
		kernel_log("error - sendPID(), ICM30628_WRITE_DAEMON_PID\n");
		return -1;
	}	
	return ret;
}

int main(int argc, char** argv)
{
	int ret = 0;
	pid_t pid;
	struct sigaction sig;
	struct sched_param sp;
	uid_t uid = getuid();
	pid_t sid;

	if( icm30628_open(&icm30628_fd) !=0 ){	
		return 0;
	}

	if (0 != getuid()){
		printf("error, daemon needs root privilege, check init.rc\n");
		exit(EXIT_FAILURE);			
	}
	sp.sched_priority = sched_get_priority_max(SCHED_RR);
	if (sp.sched_priority < 0){
		printf("error, failed to get maximum value of priority from system\n");
		exit(EXIT_FAILURE);
	}
	if (0 != sched_setscheduler(0, SCHED_RR, &sp)){
	 	printf("error, failed to set maximum priority\n");
		exit(EXIT_FAILURE);
	}
	
	pid = fork();
	if (pid < 0) {
		printf("error, fork() failed and pid=%d.\n", pid);
		exit(EXIT_FAILURE);
	}
	if (pid > 0) {
		printf("got good pid, parent child process is now going to be a daemon\n");
		exit(EXIT_SUCCESS);
	}
	sid = setsid();
	if (sid < 0) {
		printf("error, setsid() failed and sid=%d\n", sid);
		exit(EXIT_FAILURE);
	}
	
	uid = SHELL_UID;
	if ((chdir("/")) < 0) {
		printf("error, change directory to / failed\n");
		exit(EXIT_FAILURE);
	}
	if (0 != setgid(uid)){
		printf("error, setgid(%d) failed\n", uid);
		exit(EXIT_FAILURE);
	}
	if (0 != setuid(uid)){
		printf("error, setuid(%d) failed.\n", uid);
		exit(EXIT_FAILURE);
	}
		
	sig.sa_sigaction = receiveRequest;
	sig.sa_flags = SA_SIGINFO;
	pid = getpid();
	if (pid < 0) {
		printf("error, getpid() failed\n");
		exit(EXIT_FAILURE);
	}	
	ret = sendPID(pid);
	if(ret < 0) {	
		printf("error, sending pid to kernel driver failed\n");		
		exit(EXIT_FAILURE);
	}
	
	sigaction(SIG_ICM30628, &sig, NULL);

	ret = initialize();
	if(ret < 0) {		
		exit(EXIT_FAILURE);
	}

	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	while(1)
	{
		sleep(LONG_MAX);
	}

	icm30628_close(icm30628_fd);

	exit(EXIT_SUCCESS);
}

