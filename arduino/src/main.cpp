#include <Arduino.h>
#include <ArduinoJson.h>
#include <LiquidCrystal.h>

#define BAUD 256000

#define pin_joystick_x A0
#define pin_joystick_y A1
#define pin_accel_x A2
#define pin_accel_y A3
#define pin_accel_z A4

#define pin_fire_led 52
#define pin_water_led 53

#define switch_pin 44
#define action_pin 46
#define jump_pin 48
#define menu_pin 50

#define lcd_char_buffer_size 34

// #define debug_led_pin 45

LiquidCrystal lcd(2,3,4,5,8,9);

typedef struct
{
	float accelX;
	float accelY;
	float accelZ;

	float joyX;
	float joyY;

	uint8_t buttons;
	uint32_t dt;
	uint32_t random;
} sensor_data_t;

typedef struct
{
	int seg;
	
	uint8_t playerChoice;
	char lcd_data[lcd_char_buffer_size];
} display_data_t;

sensor_data_t sensor_data;
display_data_t display_data;

uint64_t time = 0;
uint64_t prevTime;

char prevMsg[lcd_char_buffer_size];
char msg[lcd_char_buffer_size];


void SendUpdate(sensor_data_t* data)
{
	JsonDocument doc;

	doc["accel"]["x"] = String(data->accelX);
	doc["accel"]["y"] = String(data->accelY);
	doc["accel"]["z"] = String(data->accelZ);

	doc["boutons"]["1"] = (data->buttons & 1) >> 0;
	doc["boutons"]["2"] = (data->buttons & 2) >> 1;
	doc["boutons"]["3"] = (data->buttons & 4) >> 2;
	doc["boutons"]["4"] = (data->buttons & 8) >> 3;

	doc["joystick"]["x"] = String(data->joyX);
	doc["joystick"]["y"] = String(data->joyY);
	doc["dt"] = data->dt;
	doc["random"] = data->random;

	serializeJson(doc, Serial);
	doc.shrinkToFit();
	if (Serial.availableForWrite() > 0)
		Serial.println();
}

void ReadUpdate(display_data_t* data)
{

	JsonDocument doc;
	// lcd.write(Serial.available());
	DeserializationError err = deserializeJson(doc, Serial);
	
	if (err)
	{
		Serial.print("deserialize() failed: ");
    	Serial.println(err.c_str());
    	return;
	}

	data->playerChoice = doc["joueur"];
	data->seg = doc["seg"];
	strncpy(data->lcd_data, doc["lcd"], lcd_char_buffer_size);
	
}

void setup() 
{
  	// Serial.begin(BAUD);

	pinMode(pin_joystick_x, INPUT);
	pinMode(pin_joystick_y, INPUT);
	
	pinMode(switch_pin, INPUT);
	pinMode(action_pin, INPUT);
	pinMode(jump_pin, INPUT);
	pinMode(menu_pin, INPUT);

	pinMode(pin_fire_led, OUTPUT);
	pinMode(pin_water_led, OUTPUT);

	pinMode(pin_accel_x, INPUT);
	pinMode(pin_accel_y, INPUT);
	pinMode(pin_accel_z, INPUT);

	DDRC = 0b11111111;

	lcd.begin(16, 2);

	sensor_data.accelX = 0;
	sensor_data.accelY = 0;
	sensor_data.accelZ = 0;

	sensor_data.buttons = 0;
	sensor_data.joyX = 0;
	sensor_data.joyY = 0;

	display_data.seg = 0;
	display_data.playerChoice = 0;
	strncpy(display_data.lcd_data, "", lcd_char_buffer_size);
	delay(2000);
}

void loop() 
{
	strncpy(prevMsg, msg, lcd_char_buffer_size);
	prevTime = time;
	time = millis();

	sensor_data.accelX = 0;
	sensor_data.accelY = 0;
	sensor_data.accelZ = 0;

	int accelX = analogRead(pin_accel_x) - 342;
	int accelY = analogRead(pin_accel_y) - 338;
	int accelZ = analogRead(pin_accel_z) - 419;

	sensor_data.accelX = (float)accelX/342;
	sensor_data.accelY = (float)accelY/338;
	sensor_data.accelZ = (float)accelZ/419;

	sensor_data.buttons = ~(digitalRead(switch_pin) | (digitalRead(action_pin) << 1) | (digitalRead(jump_pin) << 2) | (digitalRead(menu_pin) << 3));
	
	int joyX = analogRead(pin_joystick_x) - 506;
	int joyY = analogRead(pin_joystick_y) - 504;

	sensor_data.joyX = (float)joyX / 506;
	sensor_data.joyY = (float)joyY / 504;

	sensor_data.dt = time - prevTime;

	sensor_data.random = time % 10000;

	Serial.begin(BAUD);
	SendUpdate(&sensor_data);
	delay(80);
	ReadUpdate(&display_data);
	Serial.end();
	
	sprintf(msg, "%s %d", display_data.lcd_data, display_data.seg);

	//strncpy(display_data.lcd_data, "Hello, World!", lcd_char_buffer_size);
	// display_data.seg = 32;

	if (strcmp(msg, prevMsg) != 0)
	{
		lcd.clear();
		lcd.print(msg);
	}

	digitalWrite(pin_fire_led, (display_data.playerChoice & 1) >> 0);
	digitalWrite(pin_water_led, (display_data.playerChoice & 2) >> 1);

	int decimal = display_data.seg / 10;
	if (decimal > 10 || decimal < 0)
		decimal = 0;

	int unit = display_data.seg % 10;
 
	PORTC = (decimal << 4 | unit);

	delay(10);
}