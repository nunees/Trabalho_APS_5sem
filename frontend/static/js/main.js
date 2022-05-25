
(function ($) {
    $.get('data', function (data) {
        console.log(data)
        document.getElementById("sensorName").innerHTML = "Sensor: " + data["device"]
        document.getElementById("distance").innerHTML = "Distância: " + data["distance"]
        document.getElementById("safeDistance").innerHTML = "Distância Segura: " + data["safeDistance"]
        document.getElementById("status").innerHTML = "Status: " + data["status"]
        if (data["alert"] == 1) {
            document.getElementById("alert").innerHTML = "Alerta: Ligado"
        } else {
            document.getElementById("alert").innerHTML = "Alerta: Desligado"
        }
        setTimeout(getData, 1000);
    });
})(jQuery)


