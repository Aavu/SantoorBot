//
// Created by Raghavasimhan Sankaranarayanan on 5/4/21.
//

#include "Slider.h"

Slider::Slider(std::string usbName, uint16_t iNodeId) : EposController(std::move(usbName), iNodeId), m_bInitialized(false) {

}

Slider::~Slider() = default;

Error_t Slider::init(EposController::OperationMode mode, ul ulVelocity, ul ulAcc) {
    auto err = EposController::init(mode);
    if (err != kNoError)
        return err;

    err = setHome();
    if (err != kNoError)
        return err;

    err = setPositionProfile(ulVelocity, ulAcc);
    if (err != kNoError)
        return err;

    m_bInitialized = true;
    return kNoError;
}

Error_t Slider::move(uint8_t iStringId) {
    if (!m_bInitialized)
        return kNotInitializedError;

    std::cout << "Position: " << kStringPosition[iStringId] << std::endl;
    return moveToPosition(kStringPosition[iStringId], true);
}
