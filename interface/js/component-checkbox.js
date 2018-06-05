/* eslint-disable */

(function () {
  window.renderCheckbox = function renderCheckbox(data) {
    $('#autotesting').prop('checked', data.autotesting);
    $('#sensorAnalysis').prop('checked', data.sensorAnalysis);
    $('#automaticWatering').prop('checked', data.automaticWatering);
    $(`#wateringMode [value='${data.wateringMode}']`).prop('selected', 'selected');
  };
}());
