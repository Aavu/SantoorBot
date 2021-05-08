//
// Created by Raghavasimhan Sankaranarayanan on 5/4/21.
//

#include "SantoorBot.h"

SantoorBot::SantoorBot(int midiChannel) : EventCallback(midiChannel), m_bInitialized(false), m_rtpMidi(*this, PORT) {

}

SantoorBot::~SantoorBot() {
    m_bRunning = false;
    m_bInitialized = false;

    if (m_pPlayThread) {
        if (m_pPlayThread->joinable())
            m_pPlayThread->join();
        delete m_pPlayThread;
        m_pPlayThread = nullptr;
    }
}

Error_t SantoorBot::init(const std::string& robotName) {
    Error_t err = kNoError;
    err = m_striker.init(EposController::ProfilePosition);
    if (err != kNoError)
        return err;

    err = m_slider.init(EposController::ProfilePosition, SLIDER_VEL, SLIDER_ACC);
    if (err != kNoError)
        return err;

    if (m_rtpMidi.init(robotName) != 0) {
        return kNotInitializedError;
    }

    m_bInitialized = true;
    return kNoError;
}

Error_t SantoorBot::play(uint8_t uiMidiNote, uint8_t uiMidiVelocity, Articulation articulation) {
    if (!m_bInitialized) {
        std::cout << "Bot not initialized" << std::endl;
        return kNotInitializedError;
    }

    switch (articulation) {
        case Legato:
            return legato(uiMidiNote, uiMidiVelocity);
        case Staccato:
            return staccato(uiMidiNote, uiMidiVelocity);
        case Trill:
            return trill(uiMidiNote, uiMidiVelocity);
        case Tremolo:
            return tremolo(uiMidiNote, uiMidiVelocity);
    }

    return kNoError;
}

Error_t SantoorBot::play(playData_t& data) {
    if (data.play) {
        auto err = play(data.note, data.velocity, data.articulation);
        if (err != kNoError)
            return err;
        data.play = false;
    }
    return kNoError;
}

Error_t SantoorBot::legato(uint8_t uiMidiNote, uint8_t uiMidiVelocity) {
    int noteId = -1;
    for (int i=0; i<NUM_NOTES; ++i) {
        if (kScaleMap[i] == uiMidiNote) {
            noteId = i;
        }
    }
    std::cout << "Playing note: " << (int)uiMidiNote << "\twith velocity: " << (int)uiMidiVelocity << "\tNoteID: " << noteId << std::endl;

    if (noteId < 0) {
        std::cout << "Illegal call. Note Id " << noteId << std::endl;
        return kFunctionIllegalCallError;
    }

    auto err = m_slider.move(noteId);

    if (err != kNoError) {
        std::cout << "Error sliding" << std::endl;
        return err;
    }

    return m_striker.strike(uiMidiVelocity);
//    return kNoError;
}

Error_t SantoorBot::staccato(uint8_t uiMidiNote, uint8_t uiMidiVelocity) {
    return kNoError;
}

Error_t SantoorBot::trill(uint8_t uiMidiNote, uint8_t uiMidiVelocity) {
    return kNoError;
}

Error_t SantoorBot::tremolo(uint8_t uiMidiNote, uint8_t uiMidiVelocity) {
    return kNoError;
}

void SantoorBot::noteOn(uint8_t note, uint8_t velocity) {
    std::cout << "Note ON" << std::endl;
//    std::cout << "Note " << (int) note << "\t velocity " << (int)velocity << std::endl;
    std::lock_guard<std::mutex> lk(m_mtx);
    m_playData.note = note;
    m_playData.velocity = velocity;
    m_playData.play = true;
    m_cv.notify_all();
}

void SantoorBot::peerConnected(int port, const shared_ptr<::rtpmidid::rtppeer> &peer) {
    std::cout << "Peer connected: " << peer->local_name << std::endl;
}

void SantoorBot::peerDisconnected(rtpmidid::rtppeer::disconnect_reason_e reason) {
    std::cout << "Peer disconnected: " << reason << std::endl;
}

Error_t SantoorBot::startListening() {
    if (!m_bInitialized)
        return kNotInitializedError;

    m_bRunning = true;
    m_pPlayThread = new std::thread(&SantoorBot::playThreadCallback, this);
    m_rtpMidi.run();
    return kNoError;
}

void SantoorBot::playThreadCallback() {
    while (m_bRunning) {
        std::unique_lock<std::mutex> lk(m_mtx);
        m_cv.wait(lk, [this] { return (noteAvailable() || !m_bRunning); });
        auto err = play(m_playData);
        if (err != kNoError)
            break;
    }
}

bool SantoorBot::noteAvailable() const {
    return m_playData.play;
}
