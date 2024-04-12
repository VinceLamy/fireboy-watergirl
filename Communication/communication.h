#include <iostream>
#include <string>
#include <algorithm>

#define COMM_JSON
#ifdef COMM_JSON


#include "json.hpp"
#include "serial/SerialPort.hpp"

#define BAUD 256000
#define MSG_MAX_SIZE 143

using json = nlohmann::json;

class Communication
{
public:
	Communication(const char* port, bool im_open);
	~Communication();

	bool SendToPort(json json_msg);
	bool GetInputData();
	void ClosePort();
	void OpenPort();
	bool IsConnected();

	json rcv_msg, send_msg;

private:
	
	bool RcvFromPort();

	int start_cnt = 0;
	SerialPort* arduino;
	std::string msg;
	std::string Port;

	bool ArduinoIsOn();
};

#endif // COMM_JSON

#ifdef COMM_CLAVIER

#endif // COMM_CLAVIER

