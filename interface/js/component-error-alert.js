(function () {
  window.renderErrorAlert = function renderErrorAlert(data) {
    $('#errorText').text(data);
    $('#errorModal').modal('toggle');
  };
}());
