#include "communication.h"

#ifdef COMM_JSON

// im_open = immediate open
Communication::Communication(const char* port, bool im_open)
{
    if (im_open)
        arduino = new SerialPort(port, BAUD);
    else
        arduino = nullptr;
    Port = std::string(port);
}

Communication::~Communication()
{
	delete arduino;
}

bool Communication::SendToPort(json json_msg)
{
	// Return 0 if error
	std::string msg = json_msg.dump();
	bool ret = arduino->writeSerialPort(msg.c_str(), msg.length());
	return ret;
}

bool Communication::RcvFromPort() {
    // Return 0 if error
    // Message output in msg
    std::string str_buffer;
    char char_buffer[MSG_MAX_SIZE];
    int buffer_size;

    msg.clear(); // clear string

    buffer_size = arduino->readSerialPort(char_buffer, MSG_MAX_SIZE);
    str_buffer.assign(char_buffer, buffer_size);
    msg.append(str_buffer); 

    //msg.pop_back();

    return true;
}

bool Communication::GetInputData()
{
    RcvFromPort();

    rcv_msg.clear();

    if (msg.size() > 0)
    {
        msg.erase(std::remove(msg.begin(), msg.end(), '\n'), msg.end());
        //std::cout << msg.size() << std::endl;
        std::cout << msg << std::endl;

        if (msg.size() > 135 && msg.size() < 143 && start_cnt > 2)
        {
            try
            {
                rcv_msg = json::parse(msg);
                //std::cout << rcv_msg << std::endl;
            }
            catch (const std::exception& e)
            {
                send_msg["lcd"] = e.what();
                return false;
            }
            
            //std::cout << rcv_msg << std::endl;
            return true;
        }
    }

    ++start_cnt;

    return false;
}

void Communication::ClosePort()
{
    delete arduino;
}

void Communication::OpenPort()
{
    if (arduino != nullptr)
        delete arduino;
    
    Sleep(100);
    arduino = new SerialPort(Port.c_str(), BAUD);
}

bool Communication::ArduinoIsOn()
{
    return arduino != nullptr;
}

#endif // COMM_JSON