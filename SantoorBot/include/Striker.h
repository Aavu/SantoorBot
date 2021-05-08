//
// Created by Raghavasimhan Sankaranarayanan on 5/4/21.
//

#ifndef SANTURBOT_STRIKER_H
#define SANTURBOT_STRIKER_H

#include "iostream"

#include "ErrorDef.h"
#include "Defines.h"
#include "EposController.h"

class Striker final: EposController {
public:
    explicit Striker(std::string usbName=STRIKER_USB, uint16_t iNodeId=STRIKER_NODE_ID);
    ~Striker() override;

    Error_t init(EposController::OperationMode mode) override;

    Error_t strike(uint8_t iVelocity = 80);

    Error_t home() { return setHome(); }

private:
    Error_t moveAndWait(long lPos);
    bool m_bInitialized;
};


#endif //SANTURBOT_STRIKER_H
