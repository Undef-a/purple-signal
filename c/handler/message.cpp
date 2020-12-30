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
signal_display_image(PurpleConnection *pc, const std::string & chat, const std::string & sender, const std::string fileName, const long timestamp, const PurpleMessageFlags flags)
{
    PurpleSignalConnection *sa = static_cast<PurpleSignalConnection*>(purple_connection_get_protocol_data(pc));
    PurpleConversation *conv = signal_find_conversation(chat.c_str(), sa->account);
	// Note, this does not actually add the image to the purple_imgstore.
	PurpleStoredImage psi = purple_imgstore_new_from_file(fileName.c_str());
	// Actually adds the image to the purple_imgstore and gives us an ID.
	int imgid = purple_imgstore_add_with_id(purple_imgstore_get_data(psi), 
											purple_imgstore_get_size(psi),
											filename.c_str());
	/* Taken from purple-matrix: https://github.com/matrix-org/purple-matrix/blob/1d23385e6c22f63591fcbfc85c09999953c388ed/matrix-room.c#L673 */
	gchar * message = g_strdup_printf("IMG ID=\"%d\">", imgid);
    purple_conversation_write(conv, sender.c_str(), message, flags, timestamp);
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
signal_process_image(PurpleConnection *pc, const std::string & chat, const std::string & sender, const char * image, const int length, const long timestamp, const PurpleMessageFlags flags)
{
    long t = timestamp / 1000; // in Java, signal timestamps are milliseconds
    if (!t) {
        t = time(NULL);
    }
    signal_display_image(pc, chat, sender, image, length, t, flags);
}
