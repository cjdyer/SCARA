#include "terminal.h"
#include "log.h"

Terminal::Terminal(const std::string& _port) : m_port(_port)
{
    m_serial_port = open(m_port.c_str(), O_RDWR);

    if (tcgetattr(m_serial_port, &m_config) != 0)
    {
        // Need Error handling
        return;
    }

    m_config.c_cflag &= ~PARENB;    
    m_config.c_cflag &= ~CSTOPB;        
    m_config.c_cflag &= ~CSIZE;         
    m_config.c_cflag |= CS8;          
    m_config.c_cflag &= ~CRTSCTS;      
    m_config.c_cflag |= CREAD | CLOCAL; 

    m_config.c_lflag &= ~ICANON;
    m_config.c_lflag &= ~ISIG;                                                        
    m_config.c_iflag &= ~(IXON | IXOFF | IXANY);                                     
    m_config.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);

    // Reading fucntionality
    m_config.c_cc[VTIME] = 0; // Time waited Deciseconds
    m_config.c_cc[VMIN] = 0;  // Characters recieved MIN

    // Set in/out baud rate to be 115200
    cfsetspeed(&m_config, B115200);

    if (tcsetattr(m_serial_port, TCSANOW, &m_config) != 0)
    {
        // Need Error handling
        return;
    }

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
    while(m_polling_condition.load())
    {
        char output[100];
        uint8_t num_bytes = read_serial(output);
        if (num_bytes > 0)
        {
            Log::log_info(output);
        }
    }
    
    // Timeout: notify subscriber
    // std::lock_guard<std::mutex> lck(m_timeout_callback_mutex);
    // if (m_timeout_callback) { m_timeout_callback(); }

    // Log::log_info("Timer::run_timer - Terminating thread...");
}


uint8_t Terminal::read_serial(char (&_output)[100]) 
{
    return read(m_serial_port, &_output, sizeof(_output));
}

void Terminal::write_serial(const char* _msg)
{
    write(m_serial_port, _msg, sizeof(_msg));
    Log::log_info("Terminal::write_serial - " + std::string(_msg));
}