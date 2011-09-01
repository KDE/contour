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


#include "RecommendationScriptEngine.h"

#include <QScriptEngine>
#include <QScriptValue>
#include <QTextStream>
#include <QFile>

#include <QtSensors/QSensor>

#include <KDebug>
#include <KStandardDirs>

namespace Contour {

/**
 *
 */
class RecommendationScriptEngine::Private {
public:
    Private()
    {
    }

    ~Private()
    {
    }

    QScriptEngine * engine;

    QList<RecommendationItem> recommendations;
    QString script;

};

RecommendationScriptEngine::RecommendationScriptEngine(QObject * parent, const QString & script)
    : RecommendationEngine(parent), d(new Private())
{
    kDebug() << "RecommendationScriptEngine()" << script;
    kDebug() << "Available sensors" << QtMobility::QSensor::sensorTypes();

    d->script = script;
}

void RecommendationScriptEngine::init()
{
    QFile file(KStandardDirs::locate("data", "contour/scripts/" + d->script + "/main.js"));
    if (!file.open(QIODevice::ReadOnly)) {
        kDebug() << "Failed to open main.js for" << d->script;
        return;
    }

    d->engine = new QScriptEngine(this);
    connect(d->engine, SIGNAL(signalHandlerException(QScriptValue)),
            this, SLOT(signalHandlerException(QScriptValue)));

    d->engine->globalObject().setProperty("self",
            d->engine->newQObject(this));

    d->engine->evaluate(QTextStream(&file).readAll());

}

RecommendationScriptEngine::~RecommendationScriptEngine()
{
    delete d;
}

QScriptValue RecommendationScriptEngine::getSensor(const QString & sensor)
{
    kDebug() << "sensor" << sensor;
    return d->engine->newQObject(new QtMobility::QSensor(sensor.toAscii(), this));
}

void RecommendationScriptEngine::signalHandlerException(const QScriptValue & exception)
{
    kDebug() << "exception" << exception.toVariant();
}

void RecommendationScriptEngine::addRecommendation(
        qreal score,
        const QString & id,
        const QString & title,
        const QString & description,
        const QString & icon
    )
{
    kDebug() << d->script << score << id << title;

    int i = 0;

    while (d->recommendations.size() > i &&
           d->recommendations[i].score > score) {
        ++i;
    }

    RecommendationItem ri;

    ri.score       = score;
    ri.id          = id;
    ri.title       = title;
    ri.description = description;
    ri.icon        = icon;

    d->recommendations.insert(i, ri);

    emit recommendationsUpdated(d->recommendations);
}

void RecommendationScriptEngine::removeRecommendation(const QString & id)
{
    QMutableListIterator < RecommendationItem > i(d->recommendations);
    while (i.hasNext()) {
        RecommendationItem ri = i.next();

        if (ri.id == id) {
            i.remove();
        }
    }

    emit recommendationsUpdated(d->recommendations);
}


} // namespace Contour

// class RecommendationScriptEngine

