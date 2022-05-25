from flask import Flask, render_template, jsonify, redirect, request
import requests

app = Flask(__name__)


@app.route('/', methods={'GET'})
def overview():
    return render_template('overview.htm')

@app.route('/data', methods={'GET'})
def parseJson():
    data = requests.get("http://10.0.0.2:3333/api/v1/reportaenchente/stats")
    return data.json()


@app.route('/parametros', methods={'GET', 'POST'})
def parametros():
    if request.method == 'POST':
        deviceName = request.form.get("sensorName")
        distance = request.form.get("distance")
        print(deviceName)

        response = requests.post("http://10.0.0.2:3333/api/v1/reportaenchente/setdevice", 
        json={"device" : deviceName, "status" : "", "safeDistance" : distance, "measureUnit" : "cm" })
        if response.ok:
            print(response.json())

        return render_template('overview.htm')

    return render_template('parametros.htm')

@app.route('/dispositivos', methods={'GET'})
def dispositivos():
    return render_template('dispositivos.htm')


if __name__ == "__main__":
    app.run(host="0.0.0.0", port="8080", debug=True)
