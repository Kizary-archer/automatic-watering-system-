/* globals $ */

// pass
const PASSWORD = '';

// socket
let socket = {};

// store
const store = {
  dataHumidity: [],
  dateWatering: 0,

  humidity: 22,
  autotesting: false,
  wateringMode: false,
  sensorAnalysis: false,
  energySavingMode: false,
  automaticWatering: false,
  microcontroller: '',

  chart: {},
};

// init
const ip = $('#ip').ipInput();
const port = $('#port');

if (localStorage.getItem('ip')) ip.setIp(localStorage.getItem('ip'));
if (localStorage.getItem('port')) port.val(localStorage.getItem('port'));
$('[data-toggle="tooltip"]').tooltip();


function login() {
  if ($('#password').val() === PASSWORD) {
    connect();
  } else {
    $('#password').val('');
    renderErrorAlert('Error password.');
  }
  $('#password').val('');
}

function connect() {
  socket = new WebSocket(`ws://${ip.getIp()}:${port.val()}`);

  $('#loadingConnect').show();
  $('#loginForm').hide();

  socket.onopen = () => {
    localStorage.setItem('ip', ip.getIp());
    localStorage.setItem('port', port.val());
    openDashboard();
  };

  socket.onerror = (error) => {
    renderErrorAlert('Error websocket.');
    disconnect();
  };

  socket.onmessage = (data) => {
    const message = JSON.parse(data.data);

    console.log(message);

    switch (message.event) {
      case 'init':
        store.dateWatering = message.data.dateWatering;
        store.dataHumidity = message.data.dataHumidity;
        store.humidity = message.data.humidity;
        store.automaticWatering = message.data.automaticWatering;
        store.autotesting = message.data.autotesting;
        store.wateringMode = message.data.wateringMode;
        store.sensorAnalysis = message.data.sensorAnalysis;
        store.energySavingMode = message.data.energySavingMode;
        store.microcontroller = message.data.microcontroller;

        renderText(store);
        renderCheckbox(store);
        renderRage(store);
        renderTimer(store);
        chart = renderChart(store);

        break;
      default:
        break;
    }
  };

  socket.onclose = (event) => {
    if (event.wasClean) {
      socket.close();
      openLogin();
    } else {
      renderErrorAlert('Error connect websocket.');
      disconnect();
    }
  };
}

function updateRange() {
  store.humidity = $('#controlRange').val();
  renderRage(store);
  updateChartPlotLines(chart, store);
}
function openDashboard() {
  $('#loadingSend').hide();
  $('#loginCard').hide();
  $('#loadingConnect').hide();
  $('#dashboard').show();
}

function openLogin() {
  $('#dashboard').hide();
  $('#loadingConnect').hide();
  $('#loginForm').show();
  $('#loginCard').show();
}

function confirmСhanges(setting) {
  if (setting === 'wateringMode') {
    bootbox.dialog({
      title: 'Watering mode',
      message: '<p>Сhanging the watering time, removing all statistics and the system will start a new cycle. Are you sure you want to switch mode?</p>',
      buttons: {
        cancel: {
          label: 'NO',
          className: 'btn-secondary',
          callback() {
            renderCheckbox(store);
          },
        },
        ok: {
          label: "I'm sure",
          className: 'btn-danger',
          callback() {
            store.wateringMode = !store.wateringMode;
            send('setWateringMode', store.wateringMode);
          },
        },
      },
    });
  }

  if (setting === 'automaticWatering') {
    if (store.automaticWatering) {
      bootbox.dialog({
        title: 'Automatic watering',
        message: '<p>Disabling the automatic irrigation system, soil moisture will be measured. Do you want to continue?</p>',
        buttons: {
          cancel: {
            label: 'NO',
            className: 'btn-secondary',
            callback() {
              renderCheckbox(store);
            },
          },
          ok: {
            label: "I'm sure",
            className: 'btn-danger',
            callback() {
              store.automaticWatering = !store.automaticWatering;
              send('setAutomaticWatering', store.automaticWatering);
            },
          },
        },
      });
    } else {
      store.automaticWatering = !store.automaticWatering;
      send('setAutomaticWatering', store.automaticWatering);
    }
  }
}

// sending

function send(event, data = undefined) {
  const message = JSON.stringify({ event, data });
  console.log(message);
  socket.send(message);
}

function switchCheckbox(checkbox) {
  store[checkbox] = !store[checkbox];
  send(`set${checkbox}`, store[checkbox]);
}

function systemCheck() {
  send('systemCheck');
}

function range() {
  send('setHumidity', store.humidity);
}

function restart() {
  send('restart');
}


function disconnect() {
  socket.close();
  openLogin();
}
