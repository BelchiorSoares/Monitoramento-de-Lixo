#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_VL53L0X.h>
#include <LiquidCrystal_I2C.h>

// Configurações do módulo VL53L0X (sensor de distância)
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

// Configurações do LCD I2C
LiquidCrystal_I2C lcd(0x27, 16, 2); // Endereço I2C e tamanho do display

// Distância máxima e mínima esperada (em cm)
float minDistance = 10;
float maxDistance = 45; // Ajuste este valor de acordo com a sua necessidade

// Porta do botão
const int buttonPin = 2;

void setup()
{
  // Inicialização dos módulos
  Serial.begin(9600);
  Wire.begin();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(4, 0);
  lcd.print("Sistema");
  lcd.setCursor(0, 1);
  lcd.print("Inicializando...");
  delay(2000);

  // Configuração do botão
  pinMode(buttonPin, INPUT_PULLUP);

  // Inicialização do sensor
  if (!lox.begin())
  {
    Serial.println("Falha ao iniciar o sensor VL53L0X");
    lcd.clear();
    lcd.setCursor(0, 0);
    Serial.println(404);
    lcd.setCursor(0, 0);
    lcd.print("Erro de leitura...");
    lcd.setCursor(0, 1);
    lcd.print("Falha no Sensor");
  }
  else
  {
    // Verifica o estado do bot�o
    if (digitalRead(buttonPin) == LOW)
    {
      // Leitura do nivel de residuos (dist�ncia) usando o VL53L0X
      VL53L0X_RangingMeasurementData_t measure;
      lox.rangingTest(&measure, false);
      float distance = measure.RangeMilliMeter / 10.0; // Convertendo para centimetros

      // Limita a leitura de dist�ncia ao valor mAximo definido
      distance = min(distance, maxDistance);

      // Mapeamento invertido do valor de dist�ncia para um valor percentual
      float percent = map(distance, minDistance, maxDistance, 100, 0);
      percent = constrain(percent, 0, 100); // Garante que o percentual esteja entre 0 e 100

      // Determina��oo do estado de acordo com o percentual
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

      // Exibi��o no LCD
      lcd.clear();
      lcd.setCursor((16 - state.length()) / 2, 0); // Centraliza o texto
      lcd.print(state);
      lcd.setCursor(5, 1);
      lcd.print(String(percent, 2) + "%");

      // Exibi��o no Monitor Serial
      Serial.println(percent);
    }
    else
    {
      // Exibi��o no LCD
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Fechar a tampa");
      lcd.setCursor(3, 1);
      lcd.print("apois usar");
    }
  }
  delay(1000);
}