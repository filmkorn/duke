/*
 * PlaylistIterator.h
 *
 *  Created on: 3 nov. 2011
 *      Author: Guillaume Chatelet
 */

#ifndef PLAYLISTITERATOR_H_
#define PLAYLISTITERATOR_H_

#include <dukeengine/chain/Range.h>
#include <dukeengine/chain/PlaylistRange.h>
#include <dukeengine/sequence/PlaylistHelper.h>

struct PlaylistIterator : public OnePassRange<uint64_t> {
    PlaylistIterator(const PlaylistHelper& helper, uint32_t fromFrame, int32_t speed, int32_t security = 5) :
        m_Helper(helper), m_FrameRange(helper.getFirstFrame(), helper.getLastFrame(), fromFrame, speed == 0 ? 0 : (speed > 0 ? -security : security), speed < 0) {
        advanceFrame();
    }

    virtual bool empty() const {
        return m_CurrentFrame == SENTINEL && m_CurrentFrameIteratorIndex == SENTINEL;
    }

    virtual void popFront() {
        assert(!empty());
        ++m_CurrentFrameIteratorIndex;
        if (isLastIterator())
            advanceFrame();
    }

    virtual uint64_t front() {
        assert(m_CurrentFrame!=size_t(-1));
        assert(m_CurrentFrameIteratorIndex!=size_t(-1));
        assert(m_CurrentFrameIteratorIndex<m_IteratorsAtCurrentFrame.size());
        return m_Helper.getHashAtIterator(m_IteratorsAtCurrentFrame[m_CurrentFrameIteratorIndex]);
    }

private:
    inline void advanceFrame() {
        m_CurrentFrame = SENTINEL;
        m_CurrentFrameIteratorIndex = SENTINEL;
        if (m_FrameRange.empty())
            return;
        m_CurrentFrame = m_FrameRange.front();
        m_FrameRange.popFront();
        m_Helper.getIteratorsAtFrame(m_CurrentFrame, m_IteratorsAtCurrentFrame);
        m_CurrentFrameIteratorIndex = 0;
    }

    inline bool isLastIterator() {
        return m_CurrentFrameIteratorIndex >= m_IteratorsAtCurrentFrame.size();
    }

    static const size_t SENTINEL = -1;
    const PlaylistHelper& m_Helper;
    range::LimitedPlaylistFrameRange m_FrameRange;

    std::size_t m_CurrentFrame;
    std::vector<size_t> m_IteratorsAtCurrentFrame;
    std::size_t m_CurrentFrameIteratorIndex;
};

#endif /* PLAYLISTITERATOR_H_ */
