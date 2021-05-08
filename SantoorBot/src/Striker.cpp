//
// Created by Raghavasimhan Sankaranarayanan on 5/4/21.
//

#include "Striker.h"

Striker::Striker(std::string usbName, uint16_t iNodeId):
                EposController(std::move(usbName), iNodeId)
                , m_bInitialized(false)
{

}

Striker::~Striker() = default;

Error_t Striker::init(EposController::OperationMode mode) {
    auto err = EposController::init(mode);
    if (err != kNoError)
        return err;

    err = setHome();
    if (err != kNoError)
        return err;

    m_bInitialized = true;
    return kNoError;
}

Error_t Striker::strike(uint8_t iVelocity) {
    if (!m_bInitialized)
        return kNotInitializedError;

    ul acc = STRIKER_ACC * iVelocity;
    long restPos = STRIKER_REST_POSITION * iVelocity / (1 << 7);
    long strikePos = STRIKER_STRIKE_POSITION * iVelocity / (1 << 7);

    auto err = setPositionProfile(STRIKER_VEL, acc);
    if (err != kNoError)
        return err;

//    err = moveAndWait(restPos);
//    if (err != kNoError)
//        return err;

    err = moveAndWait(strikePos);
    if (err != kNoError)
        return err;

    return moveToPosition(restPos);
}

Error_t Striker::moveAndWait(long lPos) {
    return moveToPosition(lPos, true);
//    return moveToPosition(lPos, ulAcc, ulVel, 1, true);
}
