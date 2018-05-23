/* eslint-disable */

(function() {
  window.renderTimer = function renderTimer(data) {
    setInterval(() => {
      let interval = 86400000;
      if (data.wateringMode) interval = 3600000;

      const time = new Date((Math.floor((Date.now() - new Date(data.dateWatering)) / interval) + 1) * interval) - (Date.now() - new Date(data.dateWatering));

      const h = new Date(time).getHours();
      const s = new Date(time).getSeconds();
      const m = new Date(time).getMinutes();

      if (data.wateringMode) {
        $('#timer').text(`${m} : ${s}`);
      } else {
        $('#timer').text(`${h} : ${m}`);
      }

    }, 1000);
  };
}());