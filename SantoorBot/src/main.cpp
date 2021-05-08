#include <iostream>
#include "SantoorBot.h"
#include "ErrorDef.h"

//std::atomic<bool> exiting = false;
//
//void sigint_f(int) {
//    if (exiting) {
//        exit(1);
//    }
//    exiting = true;
//    INFO("SIGINT received. Closing.");
//
//}

int main(int argc, char* argv[]) {
    SantoorBot bot(MIDI_CHANNEL);
    auto err = bot.init(ROBOT_NAME);
    if (err != kNoError) {
        std::cerr << "Error initializing Santurbot." << std::endl;
        return err;
    }

    bot.startListening();
    return 0;
}
