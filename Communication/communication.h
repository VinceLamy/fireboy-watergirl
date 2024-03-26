#include <iostream>
#include <string>
#include <algorithm>

typedef struct
{
	bool moveUp;
	bool moveRight;
	bool moveDown;
	bool moveLeft;

	bool switchChars;
	bool interact;
	bool jump;
	bool menu;

	bool accelUp;

	int dt;
	int random;

} input_data;

#define COMM_JSON

#ifdef COMM_JSON

#include "json.hpp"
#include "serial/SerialPort.hpp"

#define BAUD 256000
#define MSG_MAX_SIZE 143

#define VALEUR_TIMER 60

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

	json rcv_msg, send_msg;

private:
	
	bool RcvFromPort();

	int start_cnt = 0;
	SerialPort* arduino;
	std::string msg;
	std::string Port;

};

#endif // COMM_JSON

#ifdef COMM_CLAVIER

#endif // COMM_CLAVIER

