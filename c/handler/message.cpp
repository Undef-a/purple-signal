/*
 * Implementation of handlers for incoming message (Java → C).
 * This is all to be executed on the main purple thread.
 */

#include "../purple_compat.h"
#include "../connection.hpp"
//#include <gmodule.h>

PurpleConversation *signal_find_conversation(const char *username, PurpleAccount *account) {
    PurpleIMConversation *imconv = purple_conversations_find_im_with_account(username, account);
    if (imconv == NULL) {
        imconv = purple_im_conversation_new(account, username);
    }
    PurpleConversation *conv = PURPLE_CONVERSATION(imconv);
    if (conv == NULL) {
        imconv = purple_conversations_find_im_with_account(username, account);
        conv = PURPLE_CONVERSATION(imconv);
    }
    return conv;
}

void
signal_display_message(PurpleConnection *pc, const std::string & chat, const std::string & sender, const std::string & message, const long timestamp, const PurpleMessageFlags flags)
{
    PurpleSignalConnection *sa = static_cast<PurpleSignalConnection*>(purple_connection_get_protocol_data(pc));
    PurpleConversation *conv = signal_find_conversation(chat.c_str(), sa->account);
    purple_conversation_write(conv, sender.c_str(), message.c_str(), flags, timestamp);
}

void
signal_display_message(PurpleConnection *pc, const std::string & chat, const std::string & sender, const char * message, const long timestamp, const PurpleMessageFlags flags)
{
    PurpleSignalConnection *sa = static_cast<PurpleSignalConnection*>(purple_connection_get_protocol_data(pc));
    PurpleConversation *conv = signal_find_conversation(chat.c_str(), sa->account);
    purple_conversation_write(conv, sender.c_str(), message, flags, timestamp);
}
void
signal_display_image(PurpleConnection *pc, const std::string & chat, const std::string & sender, const char * attachment, const long timestamp, const PurpleMessageFlags flags)
{
	/* PurpleImage pimg = purple_image_new_from_bytes(attachment); */
	/* purple_image_store_add(pimg); */
    PurpleSignalConnection *sa = static_cast<PurpleSignalConnection*>(purple_connection_get_protocol_data(pc));
    PurpleConversation *conv = signal_find_conversation(chat.c_str(), sa->account);
    purple_conversation_write(conv, sender.c_str(), attachment, flags, timestamp);
}

void
signal_process_message(PurpleConnection *pc, const std::string & chat, const std::string & sender, const std::string & message, const long timestamp, const PurpleMessageFlags flags)
{
    long t = timestamp / 1000; // in Java, signal timestamps are milliseconds
    if (!t) {
        t = time(NULL);
    }
    signal_display_message(pc, chat, sender, message, t, flags);
}

void
signal_process_message(PurpleConnection *pc, const std::string & chat, const std::string & sender, const char * message, const long timestamp, const PurpleMessageFlags flags)
{
    long t = timestamp / 1000; // in Java, signal timestamps are milliseconds
    if (!t) {
        t = time(NULL);
    }
    signal_display_message(pc, chat, sender, message, t, flags);
}
