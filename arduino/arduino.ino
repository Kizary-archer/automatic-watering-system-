#include <EEPROM.h>
#include "TM1637.h"
#include <iarduino_RTC.h>
#include <MsTimer2.h>

////////////Settings///////////////
#define WetsensorPower 8 //подача питания на датчик влажности
#define WetlavelEditPower 7 //питание патенциометра
#define Button 6 //кнопка режима настройки
#define pomp 5 //помпа
#define DispPower 4 //питание дисплея
#define WetlavelEdit 1 //потенциометр
#define Wetlavelnow 0 // датчик влажности
TM1637 tm1637(3, 2); //Создаём объект класса TM1637, в качестве параметров передаём номера пинов подключения
iarduino_RTC time(RTC_DS1307);
#define keeper 1008 //Сторож первого запуска
#define countlog word(EEPROM.read(1010),EEPROM.read(1011)) // размер лога
#define Wetlavelmin 1009 // минимальный уровень влажности
#define mode 1012 //режим работы (час,день)
#define analize_mode 1005 // минимальный уровень влажности

//дата начала измерений
#define TimeSensorHourStart 1000 //Час в памяти
#define TimeSensorDaysStart 1001 //День в памяти
#define TimeSensorMonthStart 1002 // Месяц в памяти
#define TimeSensorYearStart 1003 // Год в памяти
#define TimeSensorHourLast 1004 //Час последней записи в памяти

void EEPROMwrite();
void EEPROMread();
void EEPROMclear();
void SerialReadTimer();
void WetlavelEditor();
void timerDelay();
bool analize();
void CountLogValue();
void reStart();
void WetlavelEditWifi();
void SerialRead();
bool operating_mode();
void setup()
{
  pinMode(Wetlavelnow, INPUT);
  pinMode(WetsensorPower, OUTPUT);
  pinMode(WetlavelEditPower, OUTPUT);
  pinMode(Button, INPUT_PULLUP);
  pinMode(pomp, OUTPUT);
  pinMode(DispPower, OUTPUT);
  digitalWrite(DispPower, HIGH);
  Serial.begin(9600);
  MsTimer2::set(100, SerialRead); // задаем период прерывания по таймеру 100 мс
  MsTimer2::start();
  time.begin();
  time.period(1);
  tm1637.init();
  tm1637.set(BRIGHT_DARKEST);
  int val = EEPROM.read(countlog);
  tm1637.display(val);
  if (EEPROM.read(Wetlavelmin) == 0) EEPROM.update(Wetlavelmin, 60);//минимальный уровень влажности при не установленном вручную
  if (EEPROM.read(keeper) == 0)
  {
    time.gettime();
    EEPROM.update(TimeSensorHourStart, time.Hours);
    EEPROM.update(TimeSensorDaysStart, time.day);
    EEPROM.update(TimeSensorMonthStart, time.month);
    EEPROM.update(TimeSensorYearStart, time.year);

    EEPROM.update(keeper, 1);
    CountLogValue(-1); //начальное значение
    EEPROMwrite();
  }
}

void(* resetFunc) (void) = 0;//перезагрузка

void loop()
{
  time.gettime();
  if (time.Hours != EEPROM.read(TimeSensorHourLast)) {
    MsTimer2::stop();
    digitalWrite(WetsensorPower, HIGH);
    timerDelay(2000);
    if (map (analogRead(Wetlavelnow), 0, 1023, 0, 100) < EEPROM.read(Wetlavelmin))
      if (analize())
        watering (); //полив
    EEPROMwrite();
    digitalWrite(WetsensorPower, LOW);
    MsTimer2::start();
  }
}
void EEPROMwrite()
{
  time.gettime();
  unsigned short addr = countlog + 1;
  if (addr > 999) reStart(); //Переполнение памяти EEPROM
  EEPROM.update(addr, map (analogRead(Wetlavelnow), 0, 1023, 0, 100));
  int val = EEPROM.read(addr);
  CountLogValue(addr);
  EEPROM.update(TimeSensorHourLast, time.Hours);
  tm1637.display(val);
}
void EEPROMread(unsigned short ind)
{
  for (unsigned short i = 0; i <= ind; i++) Serial.println(EEPROM.read(i));
}
void reStart()
{
  EEPROM.update(keeper, 0);
  resetFunc();
}
void EEPROMclear(unsigned short ind)
{
  for (unsigned short i = 0; i < ind; i++)
    EEPROM.update(i, 0);
  EEPROM.update(keeper, 0);
  resetFunc();
}
void CountLogValue (short value)
{
  value = constrain(value, -1, 999); //диапазон лога
  EEPROM.update(1010, highByte(value));
  EEPROM.update(1011, lowByte(value));
}
void WetlavelEditor ()
{
  digitalWrite(WetlavelEditPower, HIGH);
  digitalWrite(WetsensorPower, HIGH);
  digitalWrite(pomp, LOW);
  tm1637.point(true);
  while (digitalRead(Button) == LOW)
  {
    byte sensVal = constrain(map (analogRead(WetlavelEdit), 0, 1023, 0, 100), 20, 100); //ограничение уровня влажности 20-100
    tm1637.display(sensVal);
    delay(10000);
  }
  byte sensVal = constrain(map (analogRead(WetlavelEdit), 0, 1023, 0, 100), 20, 100); //ограничение уровня влажности 20-100
  EEPROM.update(Wetlavelmin, sensVal );
  tm1637.point(false);
  digitalWrite(WetlavelEditPower, LOW);
  int val = EEPROM.read(countlog);
  tm1637.display(val);
}

/////////////Wi-fi Func//////////////

void WetlavelEditWifi ()
{
  // Serial.print('80');
  if (Serial.available() > 0)
  {
    byte val = Serial.read();
    EEPROM.update(Wetlavelmin, val);
    Serial.print(EEPROM.read(Wetlavelmin));
  }
}
bool operating_mode()
{
bool  bvalue = !EEPROM.read(mode);
  EEPROM.write(mode,bvalue);
  return bvalue; 
}
void watering ()
{
  digitalWrite(pomp, HIGH);
  timerDelay(4000);
  digitalWrite(pomp, LOW);
}
bool analize ()
{
  if (map (analogRead(Wetlavelnow), 0, 1023, 0, 100) < 10)
    return 0;
  return 1;
}
void timerDelay(unsigned short t)
{
  unsigned long ts = millis();
  while (1) {
    unsigned long currentMillis = millis();
    // Serial.println(currentMillis/1000);
    if (currentMillis - ts > t)break;
  }
}
void SerialRead()
{
  if (Serial.available() > 0)
  {
    String event = Serial.readString();
    if (event == "setHumidity") WetlavelEditWifi();
    else if (event == "setWateringMode") Serial.print(operating_mode());
   // else if (event == "help") help();
    //else if (event == "help") help();
    else
    {
      Serial.println("this command does not exist");
      Serial.println("enter h for help");
    }
  }
  if (digitalRead(Button) == LOW) WetlavelEditor();
}
