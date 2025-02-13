#include <Wire.h>
#include <Adafruit_VL53L0X.h>
#include <LiquidCrystal_I2C.h>

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
LiquidCrystal_I2C lcd(0x27, 16, 2);

float minDistance = 10;
float maxDistance = 45;
const int buttonPin = 2;

void setup()
{
	Serial.begin(9600);
	Wire.begin();
	lcd.init();
	lcd.backlight();
	lcd.setCursor(4, 0);
	lcd.print("Sistema");
	lcd.setCursor(0, 1);
	lcd.print("Inicializando...");
	pinMode(buttonPin, INPUT_PULLUP);
	delay(2000);
}

void loop()
{
	if (!lox.begin())
	{
		Serial.println(404);
		lcd.setCursor(0, 0);
		lcd.print("Erro de leitura...");
		lcd.setCursor(0, 1);
		lcd.print("Falha no Sensor");
	}
	else
	{
		if (digitalRead(buttonPin) == LOW)
		{
			VL53L0X_RangingMeasurementData_t measure;
			lox.rangingTest(&measure, false);
			float distance = measure.RangeMilliMeter / 10.0;
			distance = min(distance, maxDistance);

			float percent = map(distance, minDistance, maxDistance, 100, 0);
			percent = constrain(percent, 0, 100);

			String state;
			if (percent <= 25)
			{
				state = "Esta Vazio";
			}
			else if (percent <= 50)
			{
				state = "Esta Normal";
			}
			else if (percent <= 75)
			{
				state = "Esta Quase cheio";
			}
			else
			{
				state = "Esta Cheio";
			}

			lcd.clear();
			lcd.setCursor((16 - state.length()) / 2, 0);
			lcd.print(state);
			lcd.setCursor(5, 1);
			lcd.print(String(percent, 2) + "%");
			Serial.println(percent);
		}
		else
		{
			lcd.clear();
			lcd.setCursor(1, 0);
			lcd.print("Fechar a tampa");
			lcd.setCursor(3, 1);
			lcd.print("apois usar");
		}
	}
	delay(1000);
}