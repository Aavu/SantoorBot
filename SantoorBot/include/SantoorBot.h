//
// Created by Raghavasimhan Sankaranarayanan on 5/4/21.
//

#ifndef SANTOORBOT_SANTURBOT_H
#define SANTOORBOT_SANTURBOT_H

#include "ErrorDef.h"
#include "Defines.h"
#include "Slider.h"
#include "Striker.h"
#include "thread"
#include "atomic"
#include "condition_variable"

#include "rtpmidi.h"
#include "EventCallback.h"

class SantoorBot: EventCallback {
public:
    enum Articulation {
        Legato = 0,
        Staccato = 1,
        Trill = 2,
        Tremolo = 3
    };

private:
    struct playData_t {
        uint8_t note = 0;
        uint8_t velocity = 80;
        Articulation articulation = Legato;
        std::atomic<bool> play = false;
    };

public:
    explicit SantoorBot(int midiChannel);
    ~SantoorBot();

    Error_t init(const std::string& robotName);
    Error_t startListening();

    Error_t play(uint8_t uiMidiNote, uint8_t uiMidiVelocity, Articulation articulation);
    Error_t play(playData_t& data);

private:
    Error_t legato(uint8_t uiMidiNote, uint8_t uiMidiVelocity);
    Error_t staccato(uint8_t uiMidiNote, uint8_t uiMidiVelocity);
    Error_t trill(uint8_t uiMidiNote, uint8_t uiMidiVelocity);
    Error_t tremolo(uint8_t uiMidiNote, uint8_t uiMidiVelocity);

    void noteOn(uint8_t note, uint8_t velocity) override;

    void peerConnected(int port, const std::shared_ptr< ::rtpmidid::rtppeer> &peer) override;
    void peerDisconnected(rtpmidid::rtppeer::disconnect_reason_e reason) override;

    void playThreadCallback();
    [[nodiscard]] bool noteAvailable() const;

    bool m_bInitialized;

    RtpMidi m_rtpMidi;
    Slider m_slider;
    Striker m_striker;

    playData_t m_playData;

    std::atomic<bool> m_bRunning = false;
    std::thread* m_pPlayThread = nullptr;
    std::mutex m_mtx;
    std::condition_variable m_cv;
};


#endif //SANTOORBOT_SANTURBOT_H
