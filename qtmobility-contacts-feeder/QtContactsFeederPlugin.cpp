/*
 *   Copyright (C) 2011 Ivan Cukic ivan.cukic(at)kde.org
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation; either version 2 of
 *   the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "QtContactsFeederPlugin.h"

#include <QDBusConnection>
#include <KDebug>
#include <KWindowSystem>
#include <KUrl>

#include "QtContactsManager.h"

QtContactsFeederPlugin::QtContactsFeederPlugin(QObject * parent, const QVariantList & args)
    : Plugin(parent)
{
    Q_UNUSED(args)
    kDebug() << "We are in the QtContactsFeederPlugin";

    (void)new Contour::QtContactsManager(this);
}

QtContactsFeederPlugin::~QtContactsFeederPlugin()
{
}

KAMD_EXPORT_PLUGIN(QtContactsFeederPlugin, "activitymanger_plugin_qtmobilitycontactsfeeder")
