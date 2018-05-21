/* eslint-disable */

(function () {
  window.renderRage = function renderRage(data) {
    lastHumidity = data.dataHumidity[data.dataHumidity.length - 1];
    $('#controlRange').val(data.humidity);
    $('#controlRangeText').text(`${data.humidity}`);
    if (data.humidity - lastHumidity > 0) {
      $('#progress-comlete')
        .text(`${lastHumidity}`)
        .css('width', `${(lastHumidity) * 1.25}%`);
      $('#progress-process')
        .removeClass('bg-danger')
        .text(`${data.humidity - lastHumidity} watered`)
        .css('width', `${(data.humidity - lastHumidity) * 1.25}%`);
    } else if (data.humidity - lastHumidity === 0) {
      $('#progress-comlete')
        .text(`${lastHumidity} norm`)
        .css('width', `${(lastHumidity) * 1.25}%`);
      $('#progress-process')
        .text('')
        .css('width', '0');
    } else {
      $('#progress-comlete')
        .text(`${data.humidity}`)
        .css('width', `${(data.humidity) * 1.25}%`);
      $('#progress-process')
        .addClass('bg-danger')
        .text(`${data.humidity - lastHumidity} wait`)
        .css('width', `${(lastHumidity - data.humidity) * 1.25}%`);
    }
  };
}());
