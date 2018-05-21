/* КЛИЕТ ОТПРАВЛЯЕТ */

```
// обертка
{
  event: String,
  data:  Any,
}
```

```
// рестарт
{"event":"restart"}
```
```
// проверка системы
{"event":"systemCheck"}
```
```
// установить мод поливки 
// каждый час = true
// каждый день = false
{"event":"setWateringMode","data":false}
```
```
// установить автоматическое тестирование
{"event":"setautotesting","data":true}
```
```
// установить аналитику сенсора 
{"event":"setsensoranAlysis","data":true}
```
```
// установить энергосбережение
{"event":"setenergySavingMode","data":false}
```
```
// установить автоматическое поливание
{"event":"setAutomaticWatering","data":false}
```
```
// установить уровень желаемой влажности почвы
{"event":"setHumidity","data":"75"}
```

/* СЕРВЕР ОТПРАВЛЯЕТ */

```
// init
{
  dataHumidity: Array,        // значения замеров влажности почвы
  lastWatering: Number,       // дата начала автополива
  autoplay: Boolean,          // автополив
  autotesting: Boolean,       // автотестирование
  sensorAnalysis: Boolean,    // анализ датчиков
  energySavingMode: Boolean,  // режим энергопотребления
  wateringMode: Boolean,      // частота полива час/день
  microcontroller: String,    // название системы (вдруг их несколько)
  humidity: Number,           // настройка влажности почвы
}

// EXEMPLE
{
  dataHumidity: [69, 53, 73, 60, 68, 61, 59, 64, 65, 50, 60, 73, 55, 76, 57, 75, 60, 65, 57, 56, 73, 53, 73, 60,53, 57, 77, 73, 54, 51, 63, 53, 61, 58, 74, 77, 61, 68, 76, 63, 53, 57, 77, 73, 54, 51, 63,53,58, 56, 71, 61, 58, 74, 77, 61, 68, 76, 63, 53, 57, 77, 73, 54, 51, 63, 53, 58, 56, 71,61, 58,74, 77, 61, 68, 76, 63, 53, 58, 56, 71, 61, 58, 74, 77, 61, 68, 76, 63, 53, 57, 77,73, 54, 51,63, 53, 58, 56, 71, 61, 58, 74, 77, 61, 68, 76, 63, 53, 57, 77, 73, 54, 51, 63,53, 58, 56, 71,61, 58, 74, 77, 61, 68, 76, 63, 68, 76, 63, 53, 57, 77, 73, 54, 51, 63, 53,58, 56, 71, 61, 58,74, 77, 61, 68, 76, 63, 68, 76, 63, 53, 57, 77, 73 56, 71, 61, 58, 74, 77,61, 68, 76, 63,
  ],
  lastWatering: 1526381147614,
  autoplay: true,
  autotesting: false,
  sensorAnalysis: false,
  energySavingMode: true,
  wateringMode: false,
  microcontroller: 'Arduino ESP8266',
  humidity: 22,
}
```
