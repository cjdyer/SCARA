#include "terminal.h"
#include "log.h"
#include <cstring>

Terminal::Terminal()
{
    m_serial_port = open(m_port.c_str(), O_RDWR);

    usleep(1000);

    if (tcgetattr(m_serial_port, &m_config) != 0)
    {
        std::cout << "ERROR 1" << std::endl;
        // Need Error handling
        return;
    }

    m_config.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    m_config.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    m_config.c_cflag &= ~CSIZE; // Clear all bits that set the data size 
    m_config.c_cflag |= CS8; // 8 bits per byte (most common)
    m_config.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    m_config.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    m_config.c_lflag &= ~ICANON;
    m_config.c_lflag &= ~ECHO; // Disable echo
    m_config.c_lflag &= ~ECHOE; // Disable erasure
    m_config.c_lflag &= ~ECHONL; // Disable new-line echo
    m_config.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    m_config.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    m_config.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

    m_config.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    m_config.c_oflag &= ~ONLCR;

    // Reading fucntionality
    m_config.c_cc[VTIME] = 0; // Time waited Deciseconds
    m_config.c_cc[VMIN] = 0;  // Characters recieved MIN

    // Set in/out baud rate to be 115200
    cfsetspeed(&m_config, B115200);

    if (tcsetattr(m_serial_port, TCSANOW, &m_config) != 0)
    {
        std::cout << "ERROR 2" << std::endl;
        // Need Error handling
        return;
    }

    tcflush(m_serial_port, TCIOFLUSH);

    Log::log_info("Terminal::Terminal - Opened serial port at " + m_port);
}

Terminal::~Terminal()
{
    close(m_serial_port);
    Log::log_info("Terminal::~Terminal - Closed serial port at " + m_port);
}

void Terminal::set_read_callback(read_callback_t callback)
{
    std::lock_guard<std::mutex> guard(m_mutex);
    m_read_callback = callback;
}

void Terminal::unset_read_callback()
{
    std::lock_guard<std::mutex> guard(m_mutex);
    m_read_callback = nullptr;
}

void Terminal::start_polling()
{
    // Terminate thread if already polling
    stop_polling();
    m_polling_condition.store(true);
    m_handler_thread = std::thread(&Terminal::run_polling, this);
}

void Terminal::stop_polling()
{
    m_polling_condition.store(false);

    if (m_handler_thread.joinable()) {
        m_handler_thread.join();
    }
}

void Terminal::run_polling()
{
    Log::log_info("Terminal::run_polling - Starting thread...");

    while(m_polling_condition.load())
    {
        char output[100];
        uint8_t num_bytes = read_serial(output);
        if (num_bytes > 0)
        {
            // Log::log_info("Terminal::read_serial - " + std::string(output));
            if (output[1] == 'A' && output[2] == 'T') // This is a shit way of checking it
            {
                std::lock_guard<std::mutex> lck(m_mutex);
                if (m_read_callback) { m_read_callback(); }
            }

        }
    }

    Log::log_info("Terminal::run_polling - Terminating thread...");
}

uint8_t Terminal::read_serial(char (&_output)[100]) 
{
    std::lock_guard<std::mutex> guard(m_serial_mutex);
    return read(m_serial_port, &_output, sizeof(_output));
}

void Terminal::write_serial(const char* _msg)
{
    std::lock_guard<std::mutex> guard(m_serial_mutex);
    write(m_serial_port, _msg, strlen(_msg));
    Log::log_info("Terminal::write_serial - " + std::string(reinterpret_cast<const char*>(_msg)));
}