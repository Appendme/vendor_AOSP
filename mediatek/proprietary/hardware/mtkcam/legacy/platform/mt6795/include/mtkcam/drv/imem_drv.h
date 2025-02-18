/********************************************************************************************
 *     LEGAL DISCLAIMER
 *
 *     (Header of MediaTek Software/Firmware Release or Documentation)
 *
 *     BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 *     THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED
 *     FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS
 *     ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED,
 *     INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR
 *     A PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY
 *     WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 *     INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK
 *     ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *     NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION
 *     OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
 *
 *     BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE LIABILITY WITH
 *     RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION,
TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE
 *     FEES OR SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.
 ************************************************************************************************/
#ifndef _IMEM_DRV_H_
#define _IMEM_DRV_H_

/*=================================================================================
                                                            MEMORY TYPE DEFINITION
=================================================================================*/
typedef intptr_t                MINTPTR;//QQ
typedef uintptr_t               MUINTPTR;//QQ

/*=================================================================================

=================================================================================*/
#define IMEM_MIN_ION_FD (0)
//
#if 0
typedef enum
{
    BUF_TYPE_PMEM     = 0,// by pmem alloc
    BUF_TYPE_STD_M4U,     // by stblib malloc
    BUF_TYPE_ION,         // by ION alloc
}EBUF_TYPE;
#endif
//
struct IMEM_BUF_INFO
{
    MUINT32     size;
        MINT32      memID;
    MUINTPTR    virtAddr;//QQ
    MUINTPTR    phyAddr;//QQ
    MINT32      bufSecu;
    MINT32      bufCohe;
    MINT32      useNoncache;
    //
    IMEM_BUF_INFO(
        MUINT32     _size = 0,
        MINT32      _memID = -1,
        MUINTPTR    _virtAddr = 0,
        MUINTPTR    _phyAddr = 0,
        MINT32      _bufSecu = 0,
        MINT32      _bufCohe = 0,
        MINT32      _useNoncache =0)
    : size(_size)
    , memID(_memID)
    , virtAddr(_virtAddr)
    , phyAddr(_phyAddr)
    , bufSecu(_bufSecu)
    , bufCohe(_bufCohe)
    , useNoncache(_useNoncache)
    {}
};

typedef enum
{
    IMEM_CACHECTRL_ENUM_FLUSH       =0,
    IMEM_CACHECTRL_ENUM_INVALID     =1
}IMEM_CACHECTRL_ENUM;

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
class IMemDrv
{
    protected:
        IMemDrv(){};
        virtual ~IMemDrv() {};
    //
    public:
        static IMemDrv*  createInstance();
        virtual void    destroyInstance(void) = 0;
        virtual MBOOL   init(void) = 0;
        virtual MBOOL   uninit(void) = 0;
        virtual MBOOL   reset(void) = 0;
        virtual MINT32  allocVirtBuf(IMEM_BUF_INFO* pInfo) = 0;
        virtual MINT32  freeVirtBuf(IMEM_BUF_INFO* pInfo) = 0;
        virtual MINT32  mapPhyAddr(IMEM_BUF_INFO* pInfo) = 0;
        virtual MINT32  unmapPhyAddr(IMEM_BUF_INFO* pInfo) = 0;
        virtual MINT32  cacheSyncbyRange(IMEM_CACHECTRL_ENUM ctrl,IMEM_BUF_INFO* pInfo) = 0;

        //following functions are removed later
        virtual MINT32  cacheFlushAll(void) = 0;

};
//----------------------------------------------------------------------------
#endif  // _IMEM_DRV_H_


