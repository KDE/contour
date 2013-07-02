/*
 *   Copyright (C) 2013 Lamarque V. Souza <lamarque@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "NetworkManagerQtNotifier.h"

#include <NetworkManagerQt/Manager>
#include <NetworkManagerQt/WirelessDevice>
#include <NetworkManagerQt/AccessPoint>

#include <QHash>

#include <KDebug>

REGISTER_NETWORK_NOTIFIER(NetworkManagerQtNotifier)

class NetworkManagerQtNotifier::Private {
public:
    QHash<QString, NetworkManager::WirelessDevice::Ptr> devices;
};


NetworkManagerQtNotifier::NetworkManagerQtNotifier(QObject *parent)
    : NetworkNotifier(parent), d(new Private())
{
}

void NetworkManagerQtNotifier::init()
{
    kDebug() << "NetworkManagerQt";

    foreach (const NetworkManager::Device::Ptr &device, NetworkManager::networkInterfaces()) {
        // Check whether it is a wifi device.
        if (device->type() != NetworkManager::Device::Wifi) {
            continue;
        }

        NetworkManager::WirelessDevice::Ptr wifiDevice = device.objectCast<NetworkManager::WirelessDevice>();

        d->devices[device->interfaceName()] = wifiDevice;

        connect(wifiDevice.data(), SIGNAL(activeAccessPointChanged(QString)),
                this, SLOT(setActiveAccessPoint(QString)));
    }
}

NetworkManagerQtNotifier::~NetworkManagerQtNotifier()
{
    delete d;
}

