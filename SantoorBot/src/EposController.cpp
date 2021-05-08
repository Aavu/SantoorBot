//
// Created by Raghavasimhan Sankaranarayanan on 5/4/21.
//

#include "EposController.h"

EposController::EposController(std::string portName, int iNodeID) : m_iNodeID(iNodeID), m_strPortName(std::move(portName))
{
}

EposController::~EposController()
{
    if (m_bIsOpen)
        closeDevice();
}

Error_t EposController::init(OperationMode mode)
{
    BOOL oIsFault = 0;
    unsigned int errorCode = 0;

    Error_t err;
    if ((err = openDevice()) != kNoError)
    {
        std::cerr << "OpenDevice" << std::endl;
        return err;
    }

    if (VCS_GetFaultState(pKeyHandle, m_iNodeID, &oIsFault, &errorCode) == 0) {
        std::cerr << "VCS_GetFaultState" << std::endl;
        return kGetValueError;
    }

    if (oIsFault) {
        if (VCS_ClearFault(pKeyHandle, m_iNodeID, &errorCode) == 0) {
            std::cerr << "VCS_ClearFault" << std::endl;
            return kSetValueError;
        }
    }

    BOOL oIsEnabled = 0;
    if (VCS_GetEnableState(pKeyHandle, m_iNodeID, &oIsEnabled, &errorCode) == 0) {
        std::cerr << "VCS_GetEnableState" << std::endl;
        return kGetValueError;
    }

    if (!oIsEnabled) {
        if (VCS_SetEnableState(pKeyHandle, m_iNodeID, &errorCode) == 0) {
            std::cerr << "VCS_SetEnableState" << std::endl;
            return kSetValueError;
        }
    }

    m_operationMode = mode;
    return kNoError;
}

Error_t EposController::openDevice()
{
    int lResult = MMC_FAILED;
    unsigned int errorCode = 0;
    char *pDeviceName = new char[255];
    char *pProtocolStackName = new char[255];
    char *pInterfaceName = new char[255];
    char *pPortName = new char[255];

    strcpy(pDeviceName, m_strDeviceName.c_str());
    strcpy(pProtocolStackName, m_strProtocolStackName.c_str());
    strcpy(pInterfaceName, m_strInterfaceName.c_str());
    strcpy(pPortName, m_strPortName.c_str());

    std::cout << "Opening device " << m_strPortName << "..." << std::endl;
    pKeyHandle = VCS_OpenDevice(pDeviceName, pProtocolStackName, pInterfaceName, pPortName, &errorCode);

    if (pKeyHandle != nullptr && errorCode == 0) {
        unsigned int lBaudrate = 0;
        unsigned int lTimeout = 0;

        if (VCS_GetProtocolStackSettings(pKeyHandle, &lBaudrate, &lTimeout, &errorCode) != 0) {
            if (VCS_SetProtocolStackSettings(pKeyHandle, m_uiBaudRate, lTimeout, &errorCode) != 0) {
                if (VCS_GetProtocolStackSettings(pKeyHandle, &lBaudrate, &lTimeout, &errorCode) != 0) {
                    if (m_uiBaudRate == lBaudrate) {
                        lResult = MMC_SUCCESS;
                    }
                }
            }
        }
    } else {
        pKeyHandle = nullptr;
    }

    delete[]pDeviceName;
    delete[]pProtocolStackName;
    delete[]pInterfaceName;
    delete[]pPortName;

    if (lResult != MMC_SUCCESS)
        return kNotInitializedError;

    m_bIsOpen = true;
    return kNoError;
}

Error_t EposController::closeDevice()
{
    unsigned int errorCode = 0;

    if (VCS_SetDisableState(pKeyHandle, m_iNodeID, &errorCode) == 0) {
        std::cerr << "VCS_SetDisableState " << m_iNodeID << std::endl;
//        return kSetValueError;
    }

    if (VCS_CloseDevice(pKeyHandle, &errorCode) == 0 && errorCode == 0) {
        std::cerr << "VCS_CloseDevice " << m_iNodeID << std::endl;
        return kFileCloseError;
    }

    m_bIsOpen = false;
    return kNoError;
}

Error_t EposController::activateProfilePositionMode()
{
    Error_t err = kNoError;
    unsigned int errorCode = 0;
    if (VCS_ActivateProfilePositionMode(pKeyHandle, m_iNodeID, &errorCode) == 0) {
        err = kSetValueError;
        std::cerr << "VCS_ActivateProfilePositionMode" << std::endl;
        return err;
    }

    return err;
}

Error_t EposController::activatePositionMode()
{
    Error_t err = kNoError;
    unsigned int errorCode = 0;
    if (VCS_ActivatePositionMode(pKeyHandle, m_iNodeID, &errorCode) == 0) {
        err = kSetValueError;
        std::cerr << "VCS_ActivatePositionMode" << std::endl;
        return err;
    }

    if (VCS_SetMaxFollowingError(pKeyHandle, m_iNodeID, MAX_FOLLOW_ERROR, &errorCode) == 0) {
        err = kGetValueError;
        std::cerr << "VCS_GetMaxFollowingError" << std::endl;
        return err;
    }

    unsigned int get_maxErr = 0;

    if (VCS_GetMaxFollowingError(pKeyHandle, m_iNodeID, &get_maxErr, &errorCode) == 0) {
        err = kGetValueError;
        std::cerr << "VCS_GetMaxFollowingError" << std::endl;
        return err;
    }

    if (MAX_FOLLOW_ERROR != get_maxErr) {
        err = kSetValueError;
        std::cerr << "MaxFollowingError" << std::endl;
        return err;
    }

    return kNoError;
}

Error_t EposController::setPositionProfile(ul ulVelocity, ul ulAcc, bool shouldActivate)
{
    Error_t err = kNoError;
    if (m_operationMode != ProfilePosition && shouldActivate) {
        err = activateProfilePositionMode();
        if (err != kNoError)
            return err;
    }

    unsigned int errorCode = 0;
    if (VCS_SetPositionProfile(pKeyHandle, m_iNodeID, ulVelocity, ulAcc, ulAcc, &errorCode) == 0) {
        err = kSetValueError;
        std::cerr << "VCS_SetPositionProfile " << errorCode << std::endl;
    }
    return err;
}

Error_t EposController::moveToPosition(long targetPos, bool shouldWait, unsigned int uiTimeOut)
{
    Error_t lResult;
    unsigned int errorCode = 0;

    if (m_operationMode == ProfilePosition) {
        if (VCS_MoveToPosition(pKeyHandle, m_iNodeID, targetPos, 1, 1, &errorCode) == 0) {
            lResult = kSetValueError;
            std::cerr << "VCS_MoveToPosition" << std::endl;
            return lResult;
        }
    } else if (m_operationMode == Position) {
        if (std::abs(targetPos) < std::abs(MAX_ENCODER_VALUE) && std::abs(targetPos) >= std::abs(MIN_ENCODER_VALUE)) {
            if (VCS_SetPositionMust(pKeyHandle, m_iNodeID, targetPos, &errorCode) == 0) {
                std::cerr << "VCS_SetPositionMust" << std::endl;
                return kSetValueError;
            }
        } else {
            std::cout << "Warning: position " << targetPos << " out of range" << std::endl;
        }
    }

    if (shouldWait)
        if (VCS_WaitForTargetReached(pKeyHandle, m_iNodeID, uiTimeOut, &errorCode) == 0) {
            if (errorCode == 0x1000000b) {
//                std::cout << "Warning: VCS_WaitForTargetReached timeout" << std::endl;
                return kNoError;
            }
            lResult = kGetValueError;
            std::cerr << "VCS_WaitForTargetReached Error code: " << errorCode << std::endl;
            return lResult;
        }

    return kNoError;
}

Error_t EposController::moveToPosition(long lPos, ul ulAcc, ul ulVel, BOOL bAbsolute, bool shouldWait, unsigned int uiTimeOut)
{
//    if (m_operationMode != ProfilePosition)
//        return kFunctionIllegalCallError;

    Error_t lResult = kNoError;
    unsigned int errorCode = 0;

    if (VCS_SetPositionProfile(pKeyHandle, m_iNodeID, ulVel, ulAcc, ulAcc, &errorCode) == 0) {
        lResult = kSetValueError;
        std::cerr << "VCS_SetPositionProfile " << errorCode << std::endl;
        return lResult;
    }

    if (VCS_MoveToPosition(pKeyHandle, m_iNodeID, lPos, bAbsolute, 1, &errorCode) == 0) {
        lResult = kSetValueError;
        std::cerr << "VCS_MoveToPosition" << std::endl;
        return lResult;
    }

    if (shouldWait)
        if (VCS_WaitForTargetReached(pKeyHandle, m_iNodeID, uiTimeOut, &errorCode) == 0) {
            if (errorCode == 0x1000000b) {
//                std::cout << "Warning: VCS_WaitForTargetReached timeout" << std::endl;
                return kNoError;
            }
            lResult = kGetValueError;
            std::cerr << "VCS_WaitForTargetReached Error code: " << std::hex << errorCode << std::endl;
            return lResult;
        }

    return lResult;
}

Error_t EposController::setHome()
{
    unsigned int errorCode = 0;

    if (VCS_DefinePosition(pKeyHandle, m_iNodeID, 0, &errorCode) == 0) {
        std::cerr << "VCS_DefinePosition" << std::endl;
        return kSetValueError;
    }

    Error_t err;
    switch (m_operationMode) {
        case ProfilePosition:
            err = activateProfilePositionMode();
            break;
        case Position:
            err = activatePositionMode();
            break;
    }

    return err;
//    if (err != kNoError) {
//        return err;
//    }

//    if (m_iNodeID == STRIKER_NODE_ID)
//        return moveToPosition(STRIKER_REST_POSITION, true);
//    else
//        return moveToPosition(kStringPosition[0], 2500, 2000, 1, true, 2000);




//    // else if slider
//    Error_t err = kNoError;
//    int _lastPosition = 10000;
//    int _currentPosition = -10000;
//    activateProfilePositionMode();
//    while (true) {
//        if (moveToPosition(25, 2000, 2000, 0) != kNoError) {
//            err = kSetValueError;
//            std::cerr << "moveToPosition" << std::endl;
//            return err;
//        }
//        if (VCS_GetPositionIs(pKeyHandle, m_iNodeID, &_currentPosition, &errorCode) == 0) {
//            err = kGetValueError;
//            std::cerr << "VCS_GetPositionIs" << std::endl;
//            return err;
//        }
//        std::cout << _currentPosition << " " << _lastPosition << std::endl;
//        if (abs(_currentPosition - _lastPosition) < 5)
//            break;
//        _lastPosition = _currentPosition;
//    }
//    if (VCS_DefinePosition(pKeyHandle, m_iNodeID, 0, &errorCode) == 0) {
//        err = kSetValueError;
//        std::cerr << "VCS_DefinePosition" << std::endl;
//        return err;
//    }
}