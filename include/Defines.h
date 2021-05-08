//
// Created by Raghavasimhan Sankaranarayanan on 5/4/21.
//

#ifndef SANTURBOT_DEFINES_H
#define SANTURBOT_DEFINES_H

typedef void* HANDLE;
typedef int BOOL;
typedef unsigned long ul;

#define ROBOT_NAME "Santoor Bot"
#define MIDI_CHANNEL 0
#define PORT 5004

#define MMC_SUCCESS 0
#define MMC_FAILED 1

#define MAX_FOLLOW_ERROR 30000
#define MAX_ENCODER_VALUE (22000)
#define MIN_ENCODER_VALUE (0)

#define NUM_NOTES 9

#define STRIKER_NODE_ID 1
#define SLIDER_NODE_ID 2

#define STRIKER_USB "USB0"
#define SLIDER_USB "USB1"

#define STRIKER_REST_POSITION (-50L)
#define STRIKER_STRIKE_POSITION (50L)

#define SLIDER_ACC 10000
#define SLIDER_VEL 5000

#define STRIKER_ACC 7000 //6250 //5e5
#define STRIKER_VEL 5000

#define WAIT_TIMEOUT 100 // ms

static inline const long kStringPosition[NUM_NOTES] = {21710, 18963, 16471, 13659, 11185, 8210, 5566, 2896, 0}; //{-9268, -12015, -14507, -17319, -19793, -22768, -25412, -28082, -30978};
static inline const int kScaleMap[NUM_NOTES] = {76, 77, 79, 81, 83, 84, 86, 88, 89};
#endif //SANTURBOT_DEFINES_H
