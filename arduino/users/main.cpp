#include <iostream>
#include <string>
#include <algorithm>

#include "json.hpp"
#include "serial/SerialPort.hpp"

using json = nlohmann::json;
SerialPort* arduino;

bool SendToSerial(SerialPort *arduino, json j_msg){
    // Return 0 if error
    std::string msg = j_msg.dump();
    bool ret = arduino->writeSerialPort(msg.c_str(), msg.length());
    return ret;
}

bool RcvFromSerial(SerialPort *arduino, std::string &msg){
    // Return 0 if error
    // Message output in msg
    std::string str_buffer;
    char char_buffer[MSG_MAX_SIZE];
    int buffer_size;

    msg.clear(); // clear string

    buffer_size = arduino->readSerialPort(char_buffer, MSG_MAX_SIZE);
    str_buffer.assign(char_buffer, buffer_size);
    msg.append(str_buffer);

    msg.pop_back(); //remove '/n' from string

    return true;
}

int main()
{
    std::string port = "COM4";

    arduino = new SerialPort(port.c_str(), BAUD);
    if (!arduino->isConnected())
    {
        std::cerr << "Impossible de se connecter au port "<< std::string(port) <<". Fermeture du programme!" << std::endl;
        exit(1);
    }


    int etat_joueur = 0;
    
    int compteur_depart = VALEUR_TIMER;
    int dt = 0;

    std::string msg;
    json rcv_msg, send_msg;
    int start_cnt = 0;
    while (true)
    {
        rcv_msg.clear();

        if (!RcvFromSerial(arduino, msg))
        {
            std::cout << "Erreur lecture port serie" << std::endl;
        }

        if (msg.size() > 0)
        {
            // msg.erase(std::remove(msg.begin(), msg.end(), '\n'), msg.end());
            // std::cout << msg.size() << std::endl;
            // std::cout << msg << std::endl;
            if (msg.size() > 135 && msg.size() < 143 && start_cnt > 2)
            {
                rcv_msg = json::parse(msg);
                std::cout << rcv_msg << std::endl;
            }
        }

        if (rcv_msg["boutons"]["1"] == 1 )
        {
            if (etat_joueur == 0)
            {
                send_msg["joueur"] = 1;
                etat_joueur = 1;
            }
            else if (etat_joueur == 1)
            {
                send_msg["joueur"] = 2;
                etat_joueur = 0;
            }
        }
        
        int deltaT = 0;

        if (start_cnt > 2)
            deltaT = rcv_msg["dt"].template get<int>();
        else
        {
            deltaT = 50;
        }

        dt += deltaT;

        if (dt >= 1000)
        {
            --compteur_depart;
            dt = 0;
        }
            

        if (compteur_depart <= 0)
            compteur_depart = VALEUR_TIMER;

        // std::cout << compteur_depart << std::endl;

        send_msg["seg"] = compteur_depart;
        send_msg["lcd"] = "Salut!";

        SendToSerial(arduino, send_msg);
        ++start_cnt;
        Sleep(50);
    }

    arduino->closeSerial();
}