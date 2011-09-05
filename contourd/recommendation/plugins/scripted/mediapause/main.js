lightsensor = self.getSensor('QAmbientLightSensor');
dbussensor = self.getSensor("DBus");

print("ID for the light sensor is: " + lightsensor.identifier);
print("ID for the dbus sensor is: " + dbussensor.identifier);

dbussensor.serviceRegistered.connect(function SignalHandler(param) {
    print("Service registered:", param);
    dbussensor.call(param, "/Player", "org.freedesktop.MediaPlayer", "Pause");
});

dbussensor.serviceUnregistered.connect(function SignalHandler(param) {
    print("Service unregistered:", param);
});

dbussensor.watchFor("org.mpris.*");
