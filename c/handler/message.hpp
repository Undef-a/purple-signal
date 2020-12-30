#pragma once

#include <purple.h>

void signal_process_message(PurpleConnection *pc, const std::string & chat, const std::string & sender, const std::string & message, const long timestamp, const PurpleMessageFlags flags);

void signal_process_image(PurpleConnection *pc, const std::string & chat, const std::string & sender, const char * message, const int length, const long timestamp, const PurpleMessageFlags flags);
