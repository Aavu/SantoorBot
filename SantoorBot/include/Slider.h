//
// Created by Raghavasimhan Sankaranarayanan on 5/4/21.
//

#ifndef SANTURBOT_SLIDER_H
#define SANTURBOT_SLIDER_H

#include "iostream"
#include "EposController.h"

class Slider final: EposController {
public:
    explicit Slider(std::string usbName=SLIDER_USB, uint16_t iNodeId=SLIDER_NODE_ID);
    ~Slider() override;

    Error_t init(EposController::OperationMode mode, ul ulVelocity, ul ulAcc);

    Error_t move(uint8_t iStringId);

    Error_t home() { return setHome(); }

private:

    bool m_bInitialized;
};


#endif //SANTURBOT_SLIDER_H
