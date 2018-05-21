const WebSocketServer = require('websocket').server;
const http = require('http');

const initData = {
  dataHumidity: [69, 53, 73, 60, 68, 61, 59, 64, 65, 50, 60, 73, 55, 76, 57, 75, 60, 65, 57, 56, 73, 53, 73, 60, 53, 57, 77, 73, 54, 51, 63, 53, 61, 58, 74, 77, 61, 68, 76, 63, 53, 57, 77, 73, 54, 51, 63, 53, 58, 56, 71, 61, 58, 74, 77, 61, 68, 76, 63, 53, 57, 77, 73, 54, 51, 63, 53, 58, 56, 71, 61, 58, 74, 77, 61, 68, 76, 63, 53, 58, 56, 71, 61, 58, 74, 77, 61, 68, 76, 63, 53, 57, 77, 73, 54, 51, 63, 53, 58, 56, 71, 61, 58, 74, 77, 61, 68, 76, 63, 53, 57, 77, 73, 54, 51, 63, 53, 58, 56, 71, 61, 58, 74, 77, 61, 68, 76, 63, 68, 76, 63, 53, 57, 77, 73, 54, 51, 63, 53, 58, 56, 71, 61, 58, 74, 77, 61, 68, 76, 63, 68, 76, 63, 53, 57, 77, 73, 56, 71, 61, 58, 74, 77, 61, 68, 76, 61, 68, 76,
  ],
  dateWatering: 1526381147614,
  autotesting: false,
  wateringMode: true,
  sensorAnalysis: false,
  energySavingMode: true,
  automaticWatering: true,
  microcontroller: 'Arduino ESP8266',
  humidity: 55,
};


const server = http.createServer((request, response) => {});
server.listen(1337, () => {});

const wsServer = new WebSocketServer({
  httpServer: server,
});

wsServer.on('request', (request) => {
  const connection = request.accept(null, request.origin);

  // setInterval(() => {
  //   connection.send(JSON.stringify({ analysisHumidity: Math.floor(Math.random() * (80 - 50) + 50) }) );
  // }, 10000);

  console.log('connection');

  console.log('send init');
  connection.send(JSON.stringify({
    event: 'init',
    data: initData,
  }));

  connection.on('message', (message) => {
    if (message.type === 'utf8') {
      console.log(message.utf8Data);
      connection.send(message.utf8Data);
    }
  });

  connection.on('close', (connection) => {
    console.log('close');
  });
});
