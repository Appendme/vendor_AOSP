/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein is
 * confidential and proprietary to MediaTek Inc. and/or its licensors. Without
 * the prior written permission of MediaTek inc. and/or its licensors, any
 * reproduction, modification, use or disclosure of MediaTek Software, and
 * information contained herein, in whole or in part, shall be strictly
 * prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER
 * ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
 * NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH
 * RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 * INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES
 * TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
 * RECEIVER EXPRESSLY ACKNOWLEDGES THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO
 * OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES CONTAINED IN MEDIATEK
 * SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE
 * RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S
 * ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE
 * RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE
 * MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE
 * CHARGE PAID BY RECEIVER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek
 * Software") have been modified by MediaTek Inc. All revisions are subject to
 * any receiver's applicable license agreements with MediaTek Inc.
 */

#include "../include/CamThread.h"
#include <sys/resource.h>

#include "../include/DebugControl.h"
#define PIPE_TRACE TRACE_CAM_THREAD
#define PIPE_CLASS_TAG "CamThread"
#include "../include/PipeLog.h"

using android::sp;
using android::status_t;
using android::Mutex;

namespace NSCam {
namespace NSCamFeature {
namespace NSFeaturePipe {

CamThread::CamThread(const char *name)
  : WaitHub(name)
  , mHandle(NULL)
  , mPolicy(SCHED_NORMAL)
  , mPriority(0)
{
  TRACE_N_FUNC_ENTER(mName);
  TRACE_N_FUNC_EXIT(mName);
}

CamThread::CamThread(const char *name, MUINT32 policy, MUINT32 priority)
  : WaitHub(name)
  , mHandle(NULL)
  , mPolicy(policy)
  , mPriority(priority)
{
  TRACE_N_FUNC_ENTER(mName);
  TRACE_N_FUNC_EXIT(mName);
}

CamThread::~CamThread()
{
  TRACE_N_FUNC_ENTER(mName);
  if( mHandle.get() != NULL )
  {
    MY_LOGE("Child class MUST call stopThread() in own destrctor()");
    mHandle = NULL;
  }
  TRACE_N_FUNC_EXIT(mName);
}

MBOOL CamThread::startThread()
{
  TRACE_N_FUNC_ENTER(mName);

  Mutex::Autolock lock(mThreadMutex);
  MBOOL ret = MFALSE;

  if( mHandle.get() == NULL )
  {
    this->resetSignal();
    mHandle = new CamThreadHandle(this);
    mHandle->run(this->getName());
    ret = MTRUE;
  }

  TRACE_N_FUNC_EXIT(mName);
  return ret;
}

MBOOL CamThread::stopThread()
{
  TRACE_N_FUNC_ENTER(mName);
  MBOOL ret = MFALSE;
  Mutex::Autolock lock(mThreadMutex);
  if( mHandle != NULL )
  {
    android::sp<CamThreadHandle> handle = mHandle;
    this->triggerSignal(WaitHub::SIGNAL_STOP);
    mThreadMutex.unlock();
    ret = (handle->join() == OK);
    mThreadMutex.lock();
    handle = NULL;
    mHandle = NULL;
    mCB.clear();
    mIdleCB.clear();
    mStatusCB = NULL;
  }
  TRACE_N_FUNC_EXIT(mName);
  return ret;
}

MBOOL CamThread::insertCB(const android::sp<NotifyCB> &cb)
{
  TRACE_N_FUNC_ENTER(mName);
  MBOOL ret = MFALSE;
  Mutex::Autolock lock(mThreadMutex);
  if( cb != NULL )
  {
    this->mCB.push_back(cb);
    this->triggerSignal(WaitHub::SIGNAL_CB);
    ret = MTRUE;
  }
  TRACE_N_FUNC_EXIT(mName);
  return ret;
}

MBOOL CamThread::insertIdleCB(const android::sp<NotifyCB> &cb)
{
  TRACE_N_FUNC_ENTER(mName);
  MBOOL ret = MFALSE;
  Mutex::Autolock lock(mThreadMutex);
  if( cb != NULL )
  {
    this->mIdleCB.push_back(cb);
    this->triggerSignal(WaitHub::SIGNAL_IDLE_CB);
    ret = MTRUE;
  }
  TRACE_N_FUNC_EXIT(mName);
  return ret;
}

MBOOL CamThread::registerStatusCB(const android::sp<StatusCB> &cb)
{
  TRACE_N_FUNC_ENTER(mName);
  Mutex::Autolock lock(mThreadMutex);
  this->mStatusCB = cb;
  if( this->mStatusCB != NULL )
  {
    this->triggerSignal(WaitHub::SIGNAL_SYNC_CB | WaitHub::SIGNAL_SYNC_CB_INIT);
  }
  else
  {
    this->resetSignal(WaitHub::SIGNAL_SYNC_CB | WaitHub::SIGNAL_SYNC_CB_INIT);
  }
  TRACE_N_FUNC_EXIT(mName);
  return MTRUE;
}

MBOOL CamThread::waitIdle()
{
  TRACE_N_FUNC_ENTER(mName);
  MBOOL ret = MFALSE;
  android::sp<WaitNotifyCB> waiter = new WaitNotifyCB();
  if( this->insertIdleCB(waiter) )
  {
    ret = waiter->wait();
  }
  TRACE_N_FUNC_EXIT(mName);
  return ret;
}

MBOOL CamThread::tryProcessStop(MUINT32 signal)
{
  TRACE_N_FUNC_ENTER(mName);
  MBOOL ret = MFALSE;
  Mutex::Autolock lock(mThreadMutex);
  ret = signal & WaitHub::SIGNAL_STOP;
  TRACE_N_FUNC_EXIT(mName);
  return ret;
}

MBOOL CamThread::tryProcessCB(MUINT32 signal)
{
  TRACE_N_FUNC_ENTER(mName);
  MBOOL ret = MFALSE;
  android::Mutex::Autolock lock(mThreadMutex);
  if( signal & WaitHub::SIGNAL_CB )
  {
    while( !mCB.empty() )
    {
      ret = MTRUE;
      sp<NotifyCB> cb = mCB[0];
      mCB.pop_front();
      mThreadMutex.unlock();
      cb->onNotify();
      mThreadMutex.lock();
    }
    this->resetSignal(WaitHub::SIGNAL_CB);
  }
  TRACE_N_FUNC_EXIT(mName);
  return ret;
}

MBOOL CamThread::tryProcessIdleCB(MUINT32 signal)
{
  TRACE_N_FUNC_ENTER(mName);
  MBOOL ret = MFALSE;
  android::Mutex::Autolock lock(mThreadMutex);
  if( (signal & WaitHub::SIGNAL_IDLE_CB) &&
      (signal & WaitHub::SIGNAL_IDLE) )
  {
    ret = MTRUE;
    if( !mIdleCB.empty() )
    {
      sp<NotifyCB> cb = mIdleCB[0];
      mIdleCB.pop_front();
      mThreadMutex.unlock();
      cb->onNotify();
      mThreadMutex.lock();
    }
    if( mIdleCB.empty() )
    {
      this->resetSignal(WaitHub::SIGNAL_IDLE_CB);
    }
  }
  TRACE_N_FUNC_EXIT(mName);
  return ret;
}

MBOOL CamThread::tryProcessStatusCB(MUINT32 signal)
{
  TRACE_N_FUNC_ENTER(mName);
  MBOOL ret = MFALSE;
  MBOOL inSync;
  android::Mutex::Autolock lock(mThreadMutex);
  if( signal & WaitHub::SIGNAL_SYNC_CB_INIT )
  {
    this->resetSignal(WaitHub::SIGNAL_SYNC_CB_INIT);
  }
  if( signal & WaitHub::SIGNAL_SYNC_CB )
  {
    ret = MTRUE;
    if( mStatusCB != NULL )
    {
      inSync = (signal & WaitHub::SIGNAL_IDLE) && this->isAllQueueEmpty();
      android::sp<StatusCB> cb = mStatusCB;
      mThreadMutex.unlock();
      cb->onUpdate(inSync);
      mThreadMutex.lock();
    }
  }
  TRACE_N_FUNC_EXIT(mName);
  return ret;
}

CamThread::CamThreadHandle::CamThreadHandle(CamThread *parent)
  : mParent(parent)
  , mIsFirst(MTRUE)
{
  TRACE_N_FUNC_ENTER(mParent->mName);
  TRACE_N_FUNC_EXIT(mParent->mName);
}

CamThread::CamThreadHandle::~CamThreadHandle()
{
  TRACE_N_FUNC_ENTER(mParent->mName);
  TRACE_N_FUNC_EXIT(mParent->mName);
}

android::status_t CamThread::CamThreadHandle::readyToRun()
{
  TRACE_N_FUNC_ENTER(mParent->mName);
  MINT tid;
  struct sched_param sched_p;
  ::sched_getparam(0, &sched_p);
  if( mParent->mPolicy == SCHED_OTHER )
  {
    sched_p.sched_priority = 0;
    ::sched_setscheduler(0, mParent->mPolicy, &sched_p);
    // "priority" => nice value.
    ::setpriority(PRIO_PROCESS, 0, mParent->mPriority);
  }
  else
  {
    // "priority" => real-time priority.
    sched_p.sched_priority = mParent->mPriority;
    ::sched_setscheduler(0, mParent->mPolicy, &sched_p);
  }
  TRACE_N_FUNC_EXIT(mParent->mName);
  return android::NO_ERROR;
}

bool CamThread::CamThreadHandle::threadLoop()
{
  TRACE_N_FUNC_ENTER(mParent->mName);
  MBOOL ret = MTRUE;
  MBOOL idleCB, syncCB;
  MUINT32 signal;

  if( mIsFirst )
  {
    mIsFirst = MFALSE;
    if( !mParent->onThreadStart() )
    {
      ret = MFALSE;
    }
  }
  if( ret )
  {
    signal = mParent->waitSignal();
    if( (signal & WaitHub::SIGNAL_STOP) &&
        mParent->tryProcessStop(signal) )
    {
      mParent->onThreadStop();
      ret = MFALSE;
    }

    if( ret )
    {
      if( signal & WaitHub::SIGNAL_CB )
      {
        mParent->tryProcessCB(signal);
      }
      if( signal & WaitHub::SIGNAL_IDLE_CB )
      {
        mParent->tryProcessIdleCB(signal);
      }
      if( signal & WaitHub::SIGNAL_SYNC_CB )
      {
        mParent->tryProcessStatusCB(signal);
      }
      mParent->onThreadLoop();
    }
  }

  TRACE_N_FUNC_EXIT(mParent->mName);
  return ret;
}

}; // namespace NSFeaturePipe
}; // namespace NSCamFeature
}; // namespace NSCam
