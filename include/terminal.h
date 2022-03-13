#pragma once

#include <cstdint>

#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <unistd.h>
#include <string>
#include <mutex>
#include <thread>
#include <functional>
#include <atomic>

enum UserAction
{
    useraction_close
};

using read_callback_t = std::function<void(const UserAction)>;

class Terminal
{
public:
    Terminal(const std::string& _port);
    virtual ~Terminal();

    void set_read_callback(read_callback_t callback);
    void unset_read_callback();

    void start_polling();
    void stop_polling();

    uint8_t read_serial(char (&_output)[100]);
    void write_serial(const char* _msg);

private:
    void run_polling();

private:
    struct termios m_config;
    int m_serial_port;
    const std::string m_port;

    std::mutex m_serial_mutex;

    read_callback_t m_read_callback;
    std::mutex m_mutex; 
    std::thread m_handler_thread;
    std::atomic<bool> m_polling_condition;
};