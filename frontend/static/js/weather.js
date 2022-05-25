var latitude;
var longitude;

function getLocation() {
    if (navigator.geolocation) {
        navigator.geolocation.getCurrentPosition(getCurrentPosition, handleErrors)
    } else {
        console.log("Geolocation not suported on this browser")
    }
}

function getCurrentPosition(position)
{
    latitude = position.coords.latitude
    longitude = position.coords.longitude

    const url = `https://api.open-meteo.com/v1/forecast?latitude=${latitude}&longitude=${longitude}&current_weather=true`
    $.get(url).done(function(data){
        console.log(data)
        console.log(data["current_weather"]["temperature"])
        document.getElementById("condicoes").innerHTML = data["current_weather"]["temperature"]+"&#176;"

    });

}

function handleErrors(error) {
    switch (error.code) {
        case error.PERMISSION_DENIED:
            console.log("User denied the request for Geolocation.")
            break;
        case error.POSITION_UNAVAILABLE:
            console.log("Location information is unavailable.")
            break;
        case error.TIMEOUT:
            console.log("The request to get user location timed out.")
            break;
        case error.UNKNOWN_ERROR:
            console.log("An unknown error occurred.")
            break;
    }
}

getLocation()