/*
 *   Copyright (C) 2011 Ivan Cukic <ivan.cukic(at)kde.org>
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

#include "QtContactsManager.h"

#include <QString>
#include <QtContacts/QContactManager>

#include <KDebug>

#include "QtContactsFeeder.h"

using namespace QtMobility;

namespace Contour {

QtContactsManager::QtContactsManager(QObject * parent)
    : QObject(parent)
{
    kDebug() << "availableManagers" << QContactManager::availableManagers();

    foreach (const QString & managerName, QContactManager::availableManagers()) {
        if (managerName == "invalid") continue;
#ifndef QTCONTACTS_PLUGIN_DEBUG
        if (managerName == "memory") continue;
#endif

        (new QtContactsFeeder(managerName))->start();
    }
}

QtContactsManager::~QtContactsManager()
{
}

} // namespace Conto

