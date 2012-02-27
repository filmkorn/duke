#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "time_statistics/Durations.h"
#include "audio/AudioEngine.h"
#include "playback/Playback.h"
#include "playback/Timings.h"
#include "image/SmartCache.h"
#include "image/FileBufferHolder.h"
#include "host/renderer/Renderer.h"
#include "host/io/ImageDecoderFactoryImpl.h"

#include <protocol.pb.h>

#include <dukeapi/sequence/PlaylistHelper.h>
#include <dukeapi/MessageQueue.h>
#include <dukerenderer/ofxRenderer.h>

#include <boost/shared_ptr.hpp>

// fwd declaration
class Renderer;
namespace duke {namespace protocol {struct PlaylistHelper;}}
namespace google {namespace protobuf {class Message;}}

class Application {
public:
    Application(const char* rendererFilename, ImageDecoderFactoryImpl& imageDecoderFactory, IMessageIO &IO, int &returnCode, const uint64_t cacheSize, const size_t cacheThreads);
    ~Application();

    void* fetchSuite(const char* suiteName, int suiteVersion);
    void renderStart();
    void verticalBlanking(bool presented);
    bool renderFinished(unsigned msToPresent);
    OfxRendererSuiteV1::PresentStatus getPresentStatus();
    void pushEvent(const google::protobuf::serialize::MessageHolder&);
    const google::protobuf::serialize::MessageHolder * popEvent();

private:
    void applyTransport(const ::duke::protocol::Transport&);
    void consumeUntilRenderOrQuit();
    void consumeTransport();
    bool handleQuitMessage(const ::google::protobuf::serialize::SharedHolder&);
    std::string dumpInfo(const ::duke::protocol::Debug_Content& debug) const;

private:
    // order of variables are very important because of multi threading issues
    IMessageIO &m_IO;
    MessageQueue m_RendererMessages;
    ::google::protobuf::serialize::SharedHolder m_RendererMessageHolder;
    ImageDecoderFactoryImpl &m_ImageDecoderFactory;
    ::duke::protocol::PlaylistHelper m_Playlist;
    playback::RealtimePlaybackState m_Playback;
    AudioEngine m_AudioEngine;
    SmartCache m_Cache;
    FileBufferHolder m_FileBufferHolder;
    Timings m_VbiTimings;
    Timings m_FrameTimings;
    Durations m_Durations;
    uint32_t m_PreviousFrame;
    uint32_t m_StoredFrame;
    bool m_bRequestTermination;
    bool m_bAutoNotifyOnFrameChange;
    int &m_iReturnCode;
    // all the following objects are to be initialized in this order because of the dependencies between them
    // they also have to be initialized after the previous ones
    Renderer m_Renderer;
};

#endif /* APPLICATION_H_ */