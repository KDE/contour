lightsensor = self.getSensor('QAmbientLightSensor');
dbussensor = self.getSensor("DBus");

mediaplayernumber = 0

print("ID for the light sensor is: " + lightsensor.identifier);
print("ID for the dbus sensor is: " + dbussensor.identifier);

dbussensor.serviceRegistered.connect(function SignalHandler(param) {
    print("Service registered:", param);
    // dbussensor.call(param, "/Player", "org.freedesktop.MediaPlayer", "Pause");

    if (mediaplayernumber == 0) {
        self.addRecommendation(1.0, "pause", "Pause media", "while the light is on", "media-playback-pause");
    }
    mediaplayernumber++;
});

dbussensor.serviceUnregistered.connect(function SignalHandler(param) {
    print("Service unregistered:", param);

    mediaplayernumber--;
    if (mediaplayernumber == 0) {
        self.removeRecommendations();
    }
});

dbussensor.watchFor("org.mpris.bangarang");
