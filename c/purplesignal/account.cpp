/*
 * Implementation of all glue (C → Java) for account management (register, link verify).
 */

#include "purplesignal.hpp"
#include "utils.hpp"

void PurpleSignal::register_account(bool voice, const std::string & captcha) {
    instance.GetMethod<void(jboolean, jstring)>("registerAccount")(voice, jvm->make_jstring(captcha));
    tjni_exception_check(jvm);
}

void PurpleSignal::link_account() {
    instance.GetMethod<void()>("linkAccount")();
    tjni_exception_check(jvm);
}

void PurpleSignal::verify_account(const std::string & code, const std::string & pin) {
    instance.GetMethod<void(jstring, jstring)>("verifyAccount")(
        jvm->make_jstring(code), jvm->make_jstring(pin)
    );
    tjni_exception_check(jvm);
}
