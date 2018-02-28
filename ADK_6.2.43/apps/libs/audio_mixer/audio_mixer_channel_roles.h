/****************************************************************************
Copyright (c) 2017 Qualcomm Technologies International, Ltd.

FILE NAME
    audio_mixer_channel_roles.h

DESCRIPTION
    Definition of common channel roles, used to connect chains together
*/

#ifndef _AUDIO_MIXER_CHANNEL_ROLES_H_
#define _AUDIO_MIXER_CHANNEL_ROLES_H_

typedef enum
{
    left_channel,
    right_channel,
    secondary_left_channel,
    secondary_right_channel,
    voice_channel,
    music_left_channel,
    music_right_channel,
    prompt_left_channel,
    prompt_right_channel,
} audio_mixer_channel_role_t;

#endif /* _AUDIO_MIXER_CHANNEL_ROLES_H_ */
