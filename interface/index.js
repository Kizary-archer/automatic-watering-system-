/* globals $ */

// pass
const PASSWORD = 'qqq';

// socket
let socket = {};

// store
const store = {
  autoplay: true,
  autotesting: false,
  ipInput: $('#ip').ipInput(),
  microcontroller: 'Arduino ESP8266',
  range: 60,
  humidity: 22,
  sensorAnalysis: false,
  sleepMode: true,
  wateringMode: false,
  port: 81,
};

// init
store.ipInput.setIp('192.168.0.102');
renderText();
renderCheckbox();
renderRage();

function renderCheckbox() {
  $('#autoplay').attr('checked', store.autoplay);
  $('#sleepMode').attr('checked', store.sleepMode);
  $('#autotesting').attr('checked', store.autotesting);
  $('#sensoranAlysis').attr('checked', store.sensoranAlysis);
}

function renderText() {
  $('#ipText').text(store.ipInput.getIp());
  $('#modelText').text(store.microcontroller);
}

function renderRage() {
  $('#controlRange').val(store.range);
  $('#controlRangeText').text(`${store.range}`);
  if (store.range - store.humidity > 0) {
    $('#progress-comlete')
      .text(`${store.humidity}`)
      .css('width', `${(store.humidity) * 1.25}%`);
    $('#progress-process')
      .removeClass('bg-danger')
      .text(`${store.range - store.humidity} watered`)
      .css('width', `${(store.range - store.humidity) * 1.25}%`);
  } else if (store.range - store.humidity === 0) {
    $('#progress-comlete')
      .text(`${store.humidity} norm`)
      .css('width', `${(store.humidity) * 1.25}%`);
    $('#progress-process')
      .text('')
      .css('width', '0');
  } else {
    $('#progress-comlete')
      .text(`${store.range}`)
      .css('width', `${(store.range) * 1.25}%`);
    $('#progress-process')
      .addClass('bg-danger')
      .text(`${store.range - store.humidity} wait`)
      .css('width', `${(store.humidity - store.range) * 1.25}%`);
  }
}

function errorAlert(text) {
  $('#errorText').text(text);
  $('#errorModal').modal('toggle');
}

function connect() {
  // if ($('#password').val() === PASSWORD) {
  socket = new WebSocket(`ws://${store.ipInput.getIp()}:${store.port}`);

  $('#loadingConnect').show();
  $('#loginForm').hide();

  socket.onopen = () => {
    console.log('connect success');
    renderText();
    renderCheckbox();
    renderRage();

    $('#loginCard').hide();
    $('#loadingConnect').hide();
    $('#dashboard').show();
  };

  socket.onerror = (error) => {
    console.error(`Error: ${error}`);
    errorAlert('Error conected.');
    $('#loginForm').show();
    $('#loadingConnect').hide();
  };

  socket.onclose = (event) => {
    if (event.wasClean) {
      console.log('connect close');
    } else {
      console.warning('connect crash');
    }
    console.error(`Code: ${event.code} reason: ${event.reason}`);
  };

  socket.onmessage = (event) => {
    console.log(`Получены данные ${event}`);
  };
  // } else {
  //   $('#password').val('');
  //   console.error('Error password.');
  //   errorAlert('Error password.');
  // }
  $('#password').val('');
}

function disconnect() {
  $('#dashboard').hide();
  $('#loadingConnect').hide();
  $('#loginForm').show();
  $('#loginCard').show();
}

function updateRange() {
  $('#loadingSend').show();
  store.range = $('#controlRange').val();
  renderRage();

  setTimeout(() => {
    $('#loadingSend').hide();
  }, 2000);
}

// todo
