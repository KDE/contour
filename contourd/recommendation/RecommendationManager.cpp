/*
 * Copyright (C) 2011 Ivan Cukic <ivan.cukic@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "RecommendationManager.h"

#include <QList>

#include <KDebug>
#include <KServiceTypeTrader>
#include <KConfig>
#include <KConfigGroup>

#include "RecommendationEngine.h"
#include "RecommendationScriptEngine.h"

namespace Contour {

class RecommendationManager::Private {
public:
    Private(RecommendationManager * parent)
        : q(parent)
    {
        config = new KConfig("contourrc");
        enginesConfig = new KConfigGroup(config, "RecommendationEngines");
    }

    ~Private()
    {
        delete enginesConfig;
        delete config;
    }

    RecommendationManager * q;

    struct EngineInfo {
        QString name;
        qreal score;
    };

    QHash < RecommendationEngine *, EngineInfo > engines;
    QList < RecommendationItem > recommendations;

    KConfig * config;
    KConfigGroup * enginesConfig;
};


RecommendationManager::RecommendationManager(QObject *parent)
    : QObject(parent),
      d(new Private(this))
{
    kDebug() << "Loading engines...";

    KService::List offers = KServiceTypeTrader::self()->query("Contour/RecommendationEngine");

    // Loading C++ plugins

    foreach (const KService::Ptr & service, offers) {

        kDebug() << "Loading engine:"
            << service->name() << service->storageId() << service->library();

        KPluginFactory * factory = KPluginLoader(service->library()).factory();

        if (!factory) {
            kDebug() << "Failed to load engine:" << service->name();
            continue;
        }

        RecommendationEngine * engine = factory->create < RecommendationEngine > (this);

        if (engine) {
            engine->init();
            d->engines[engine].name = service->name();
            d->engines[engine].score = d->enginesConfig->readEntry(service->name(), 1.0);

            connect(engine, SIGNAL(recommendationsUpdated(QList<RecommendationItem>)),
                    this,   SLOT(updateRecommendations(QList<RecommendationItem>)),
                    Qt::QueuedConnection);

        } else {
            kDebug() << "Failed to load engine:" << service->name();
        }

    }

    // Loading scripted plugins
    kDebug() << "Loading scripted plugins";

    // new RecommendationScriptEngine(this, QString());

    offers = KServiceTypeTrader::self()->query("Contour/RecommendationEngine/QtScript");

    foreach (const KService::Ptr & service, offers) {

        kDebug() << "Loading scripted engine:"
            << service->name() << service->library();

        RecommendationEngine * engine = new RecommendationScriptEngine(this, service->library());

        engine->init();
        d->engines[engine].name = service->name();
        d->engines[engine].score = d->enginesConfig->readEntry(service->name(), 1.0);

        connect(engine, SIGNAL(recommendationsUpdated(QList<RecommendationItem>)),
                this,   SLOT(updateRecommendations(QList<RecommendationItem>)),
                Qt::QueuedConnection);

    }
}

RecommendationManager::~RecommendationManager()
{
    delete d;
}

void RecommendationManager::updateRecommendations(const QList < RecommendationItem > & newRecommendations)
{
    RecommendationEngine * engine = dynamic_cast < RecommendationEngine * > (sender());

    if (!engine || !d->engines.contains(engine)) return;

    const Private::EngineInfo & engineInfo = d->engines[engine];

    kDebug() << engineInfo.name << "updated its recommendations";

    QMutableListIterator < RecommendationItem > i (d->recommendations);
    while (i.hasNext()) {
        const RecommendationItem & val = i.next();
        if (val.engine == engineInfo.name) {
            i.remove();
        }
    }

    int location = 0;

    // A simple merge sort
    foreach (RecommendationItem item, newRecommendations) {
        item.score *= engineInfo.score;
        item.engine = engineInfo.name;

        if (location >= d->recommendations.count()
                || item.score > d->recommendations[location].score) {
            d->recommendations.insert(location, item);
        } else {
            location++;
        }
    }


}

} // namespace Contour

#include "RecommendationManager.moc"
