#pragma once

#include <Arduino.h>

class OtaHandler {
public:
    static void Begin(const char* hostname = nullptr,
                      const char* password = nullptr);

    static void Handle();

private:
    static uint32_t lastOtaTime;
};