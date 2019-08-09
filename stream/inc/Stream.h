/*
 * Copyright (c) 2019, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of The Linux Foundation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef STREAM_H_
#define STREAM_H_

#include "QalDefs.h"
#include <algorithm>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <memory>
#include <mutex>
#include <exception>
#include <errno.h>
#include "QalCommon.h"

typedef enum {
    DATA_MODE_SHMEM = 0,
    DATA_MODE_BLOCKING ,
    DATA_MODE_NONBLOCKING
} dataFlags;

#define BUF_SIZE_PLAYBACK 1024
#define BUF_SIZE_CAPTURE 960
#define NO_OF_BUF 4
#define MUTE_TAG 0
#define UNMUTE_TAG 1
#define PAUSE_TAG 2
#define RESUME_TAG 3
#define MFC_SR_8K 4
#define MFC_SR_16K 5
#define MFC_SR_32K 6
#define MFC_SR_44K 7
#define MFC_SR_48K 8
#define MFC_SR_96K 9
#define MFC_SR_192K 10
#define MFC_SR_384K 11
#define FLUENCE_ON_TAG 12
#define FLUENCE_OFF_TAG 13

class Device;
class ResourceManager;
class Session;

class Stream
{
protected:
    uint32_t noOfDevices;
    std::vector <std::shared_ptr<Device>> devices;
    std::shared_ptr <Device> dev;
    Session* session;
    struct qal_stream_attributes* attr;
    struct qal_volume_data* vdata = NULL;
    std::mutex mutex;
    static std::mutex mtx;
    static std::shared_ptr<ResourceManager> rm;
    struct modifier_kv *modifiers_;
    uint32_t uNoOfModifiers;
    size_t inBufSize;
    size_t outBufSize;
    size_t inBufCount;
    size_t outBufCount;

public:
    virtual int32_t open() = 0;
    virtual int32_t close() = 0;
    virtual int32_t start() = 0;
    virtual int32_t stop() = 0;
    virtual int32_t prepare() = 0;
    virtual int32_t setStreamAttributes(struct qal_stream_attributes *sattr) = 0;
    virtual int32_t setVolume( struct qal_volume_data *volume) = 0;
    virtual int32_t setMute( bool state) = 0;
    virtual int32_t setPause() = 0;
    virtual int32_t setResume()= 0;
    virtual int32_t read(struct qal_buffer *buf) = 0;
    int32_t getStreamAttributes(struct qal_stream_attributes *sattr);
    int32_t getModifiers(struct modifier_kv *modifiers,uint32_t *noOfModifiers);
    int32_t getStreamType(qal_stream_type_t* streamType);
    virtual int32_t write(struct qal_buffer *buf) = 0;
    virtual int32_t registerCallBack(qal_stream_callback cb) = 0;
    virtual int32_t getCallBack(qal_stream_callback *cb) = 0;
    virtual int32_t getParameters(uint32_t param_id, void **payload) = 0;
    virtual int32_t setParameters(uint32_t param_id, void *payload) = 0;
    int32_t getAssociatedDevices(std::vector <std::shared_ptr<Device>> &adevices);
    int32_t getAssociatedSession(Session** session);
    int32_t setBufInfo(size_t *in_buf_size, size_t in_buf_count,
                       size_t *out_buf_size, size_t out_buf_count);
    int32_t getBufInfo(size_t *in_buf_size, size_t *in_buf_count,
                       size_t *out_buf_size, size_t *out_buf_count);
    int32_t getVolumeData(struct qal_volume_data *vData);
    /* static so that this method can be accessed wihtout object */
    static Stream* create(struct qal_stream_attributes *sattr, struct qal_device *dattr,
         uint32_t no_of_devices, struct modifier_kv *modifiers, uint32_t no_of_modifiers);
};

#endif//STREAM_H_
