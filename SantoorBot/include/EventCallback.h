//
// Created by Raghavasimhan Sankaranarayanan on 4/15/21.
//

#ifndef RTPMIDIPEEREXAMPLE_EVENTCALLBACK_H
#define RTPMIDIPEEREXAMPLE_EVENTCALLBACK_H

#include <iostream>
#include <functional>
#include "rtpmidid/logger.hpp"
#include "rtpmidid/rtppeer.hpp"

#define CLK_PER_BEAT 6

class EventCallback {
public:
    explicit EventCallback(int channel) : m_iChannel(channel) {
        if (m_iChannel == 0) {
            std::cout << "Midi Channel is set to 0. Messages from all channels will be parsed." << std::endl;
        } else if (m_iChannel < 1 || m_iChannel > 16) {
            WARNING("Invalid Midi Channel. No message will be parsed");
        }
    }

    ~EventCallback() = default;

    virtual void cc(uint8_t key, uint8_t value) {}

    virtual void noteOn(uint8_t note, uint8_t velocity) {
        std::cout << "Midi note on" << std::endl;
    }

    virtual void noteOff(uint8_t note, uint8_t velocity) {}
    virtual void channelPressure(uint8_t pressure) {}
    virtual void clock() {
        ++m_lCurrentPosition;
//        std::cout << m_lCurrentPosition << std::endl;
    }

    virtual void clockStart() {
        m_lCurrentPosition = 0;
    }

    virtual void clockContinue() {}

    virtual void clockStop() {}

    virtual void songPositionPointer(int position) {
        m_lCurrentPosition = (position * CLK_PER_BEAT);
    }

    virtual void peerConnected(int port, const std::shared_ptr<::rtpmidid::rtppeer>& peer) {}

    virtual void peerDisconnected(rtpmidid::rtppeer::disconnect_reason_e reason) {}

    [[nodiscard]] int getMidiChannel() const { return m_iChannel; }
    [[nodiscard]] long long getPlayHeadPosition() const { return m_lCurrentPosition; }

    [[nodiscard]] bool canParseMessage(int midiChannel) const {
        return m_iChannel == 0 || m_iChannel == midiChannel;
    }

protected:
    int m_iChannel = 0;
    long long m_lCurrentPosition = 0;
};

#endif //RTPMIDIPEEREXAMPLE_EVENTCALLBACK_H
