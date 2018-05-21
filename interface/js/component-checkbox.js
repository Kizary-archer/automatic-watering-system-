/* eslint-disable */

(function () {
  window.renderCheckbox = function renderCheckbox(data) {
    $('#autotesting').prop('checked', data.autotesting);
    $('#sensoranAlysis').prop('checked', data.sensoranAlysis);
    $('#energySavingMode').prop('checked', data.energySavingMode);
    $('#automaticWatering').prop('checked', data.automaticWatering);
    $(`#wateringMode [value='${data.wateringMode}']`).prop('selected', 'selected');
  };
}());
