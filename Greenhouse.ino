#include "DHT.h"
#define DHTPIN 4    // ������ ��������� � �����������
#define SOILPIN A0  // ������ ��������� �����
#define LUMPIN A5   // ������������
#define DRYPIN 13   // ���� ���������
#define HUMPIN 12   // ���� �����������
#define EXHPIN 11   // ���� �������
#define HEAPIN 10   // ���� �������
#define LIGPIN 9    // ���� ���������
#define WATPIN 8    // ���� ������ �����

DHT dht(DHTPIN, DHT11);

  int m=10; //������
  int h=14; //����
  int s=55;  //�������

  int lightOnM = 11, lightOnH = 14; 	//����� ��������� �����
  int lightOffM = 12, lightOffH = 14;    //����� ���������� �����
  float max_humidity = 75; 
  float min_humidity = 45; //������� ��������� �������
  float max_temp = 25, min_temp = 20;   //������� ����������� �������
  float max_soil_humidity = 80; 
  float min_soil_humidity = 70; //������� ��������� �����
  float max_lum = 60; //������ ������������ �����, ������ - ������
  bool need_light = false; //���� ��� ������������ �����

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
  
  s=s+2;      //������� �������
  delay(1900); //����� ��� ������� 2 ������
  if (s>=60)  //��� 60�������� 
  {            
   s=0;       //�������� �������
   m=m+1;     //��������� ������
   if(m>=60)  //��� 60 �������
   {
    m=0;      // �������� ������
    h=h+1;    //��������� ���
    if(h>=24) h=0;  //��� 24 �����
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

    float hum = dht.readHumidity(); //��������� ���������
    float tem = dht.readTemperature(); //��������� �����������
    
    // �������� ������ ������ �� ����������.
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

    if (soilHum > max_soil_humidity)  // ��������� �����
      digitalWrite(WATPIN, LOW);
    else if (soilHum < min_soil_humidity)
      digitalWrite(WATPIN, HIGH);
    if ((lightOnH == h)&(lightOnM == m))  //�������� ������� ���������
      need_light = true;
    else if((lightOffH == h)&(lightOffM == m))
      need_light = false;

    if (need_light&(light>max_lum))        //�������� �������������
      digitalWrite(LIGPIN, HIGH);
    if (!need_light||(light<=max_lum))
      digitalWrite(LIGPIN, LOW);

    if (tem > max_temp) {           // �����������
      digitalWrite(EXHPIN, HIGH);   // ���� ��������
      digitalWrite(HEAPIN, LOW);    // ���� ���������
    }
    else if (tem < min_temp){
      digitalWrite(EXHPIN, LOW);
      digitalWrite(HEAPIN, HIGH);
    }
    else {
      digitalWrite(EXHPIN, LOW);
      digitalWrite(HEAPIN, LOW);
    }
     
    if (hum > max_humidity) {       // ��������� �������
      digitalWrite(DRYPIN, HIGH);   // ���� ��������
      digitalWrite(HUMPIN, LOW);    // ���� ���������
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
