lightsensor = self.getSensor('QAmbientLightSensor');

print("ID for the light sensor is: " + lightsensor.identifier);

self.addRecommendation(1.0, "http://www.kde.org", "KDE homepage", "This is the homepage of an uber-awesome community", "kde");
self.addRecommendation(1.6, "http://plasma.kde.org", "Plasma homepage", "This is the plasma home page", "plasma");
self.addRecommendation(1.0, "http://www.wikipedia.org", "Wikipedia", "The biggest encyclopedia in the world", "konqueror");
