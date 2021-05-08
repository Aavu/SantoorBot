//
// Created by Raghavasimhan Sankaranarayanan on 5/4/21.
//

#ifndef SANTURBOT_EPOSCONTROLLER_H
#define SANTURBOT_EPOSCONTROLLER_H

#include "iostream"
#include "cstring"

#include "ErrorDef.h"
#include "Definitions.h"
#include "Defines.h"

class EposController {
public:
    enum OperationMode {
        Position, ProfilePosition
    };

    explicit EposController(std::string portName, int iNodeID);
    virtual ~EposController();

    Error_t openDevice();
    virtual Error_t init(OperationMode mode);
    Error_t closeDevice();

    Error_t activateProfilePositionMode();
    Error_t activatePositionMode();
    Error_t setPositionProfile(ul ulVelocity, ul ulAcc, bool shouldActivate=false);

    Error_t moveToPosition(long targetPos, bool shouldWait = false, unsigned int uiTimeOut=WAIT_TIMEOUT);
    Error_t moveToPosition(long lPos, ul ulAcc, ul ulVel=2500, BOOL bAbsolute=1, bool shouldWait=false, unsigned int uiTimeOut=WAIT_TIMEOUT);

    virtual Error_t setHome();

    [[nodiscard]] uint16_t getNodeId() const { return m_iNodeID; }

private:
    OperationMode m_operationMode = Position;

    HANDLE pKeyHandle = nullptr;
    uint16_t m_iNodeID = 1;
    std::string m_strDeviceName = "EPOS4";
    std::string m_strProtocolStackName = "MAXON SERIAL V2";
    std::string m_strInterfaceName = "USB";
    std::string m_strPortName;
    unsigned int m_uiBaudRate = 1000000;
    bool m_bIsOpen = false;
};


#endif //SANTURBOT_EPOSCONTROLLER_H
