/* eslint-disable */

(function () {
  window.renderText = function renderText(data) {
    $('#ipText').text(localStorage.getItem('ip'));
    $('#modelText').text(data.microcontroller);
  };
}());
