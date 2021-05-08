//
// Created by Raghavasimhan Sankaranarayanan on 5/4/21.
//

#ifndef SANTURBOT_ERRORDEF_H
#define SANTURBOT_ERRORDEF_H

enum Error_t
{
    kNoError,

    kFileOpenError,
    kFileAccessError,
    kFileWriteError,
    kFileCloseError,

    kFunctionInvalidArgsError,
    kFunctionExecOrderError,

    kNotInitializedError,
    kFunctionIllegalCallError,
    kInvalidString,

    kGetValueError,
    kSetValueError,

    kOutOfBoundsError,

    kMemError,

    kUnknownError,

    kNumErrors
};

#endif //SANTURBOT_ERRORDEF_H
