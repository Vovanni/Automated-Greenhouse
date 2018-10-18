#include "DHT.h"
#define DHTPIN 4    // датчик влажности и температуры
#define SOILPIN A0  // датчик влажности почвы
#define LUMPIN A5   // фоторезистор
#define DRYPIN 13   // реле осушителя
#define HUMPIN 12   // реле увлажнителя
#define EXHPIN 11   // реле вытяжки
#define HEAPIN 10   // реле вытяжки
#define LIGPIN 9    // реле освящения
#define WATPIN 8    // реле полива почвы

DHT dht(DHTPIN, DHT11);

  int m=10; //минуты
  int h=14; //часы
  int s=55;  //секунды

  int lightOnM = 11, lightOnH = 14; 	//Время включения света
  int lightOffM = 12, lightOffH = 14;    //Время выключения света
  float max_humidity = 75; 
  float min_humidity = 45; //Пределы влажности воздуха
  float max_temp = 25, min_temp = 20;   //Пределы температуры воздуха
  float max_soil_humidity = 80; 
  float min_soil_humidity = 70; //Пределы влажности почвы
  float max_lum = 60; //Предел переключения света, больше - темнее
  bool need_light = false; //Флаг для переключения света

void setup() {
    pinMode(DRYPIN, OUTPUT);
    pinMode(HUMPIN, OUTPUT);
    pinMode(EXHPIN, OUTPUT);
    pinMode(HEAPIN, OUTPUT);
    pinMode(LIGPIN, OUTPUT);
    pinMode(WATPIN, OUTPUT);
    Serial.begin(9600);
    dht.begin();
}

void loop() {
  
  s=s+2;      //считаем секунды
  delay(1900); //пауза для отсчета 2 секунд
  if (s>=60)  //при 60секундах 
  {            
   s=0;       //обнуляем секунды
   m=m+1;     //добавляем минуту
   if(m>=60)  //при 60 минутах
   {
    m=0;      // обнуляем минуты
    h=h+1;    //добавляем час
    if(h>=24) h=0;  //при 24 часах
   }
  }  
    int moisture = analogRead(SOILPIN);
    float soilHum = (moisture/1023.0 - 0.6)*250.0;
    Serial.println("Soil humidity:");
    Serial.println(soilHum);

    int sun = analogRead(LUMPIN);
    float light = (sun/1023.0 - 0.6)*250.0;
    Serial.println("Illumination:");
    Serial.println(light);

    float hum = dht.readHumidity(); //Считываем влажность
    float tem = dht.readTemperature(); //Считываем температуру
    
    // Проверка удачно прошло ли считывание.
    if (isnan(hum) || isnan(tem)) {
    Serial.println("Error!");
    return;
    }
    Serial.println("Humidity:");
    Serial.println(hum);
    Serial.println("Temperature:");
    Serial.println(tem);
    Serial.println("Time:");
    Serial.print(h);
    Serial.print(":");
    Serial.print(m);
    Serial.println();

    if (soilHum > max_soil_humidity)  // Влажность почвы
      digitalWrite(WATPIN, LOW);
    else if (soilHum < min_soil_humidity)
      digitalWrite(WATPIN, HIGH);
    if ((lightOnH == h)&(lightOnM == m))  //Проверка таймера освещения
      need_light = true;
    else if((lightOffH == h)&(lightOffM == m))
      need_light = false;

    if (need_light&(light>max_lum))        //Проверка фоторезистора
      digitalWrite(LIGPIN, HIGH);
    if (!need_light||(light<=max_lum))
      digitalWrite(LIGPIN, LOW);

    if (tem > max_temp) {           // Температура
      digitalWrite(EXHPIN, HIGH);   // реле включено
      digitalWrite(HEAPIN, LOW);    // реле выключено
    }
    else if (tem < min_temp){
      digitalWrite(EXHPIN, LOW);
      digitalWrite(HEAPIN, HIGH);
    }
    else {
      digitalWrite(EXHPIN, LOW);
      digitalWrite(HEAPIN, LOW);
    }
     
    if (hum > max_humidity) {       // Влажность воздуха
      digitalWrite(DRYPIN, HIGH);   // реле включено
      digitalWrite(HUMPIN, LOW);    // реле выключено
    }
    else if (hum < min_humidity){
      digitalWrite(DRYPIN, LOW);
      digitalWrite(HUMPIN, HIGH);
    }
    else {
      digitalWrite(DRYPIN, LOW);
      digitalWrite(HUMPIN, LOW);
    }
}
