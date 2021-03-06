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
#include "RecommendationScriptEngine_p.h"

#include <QScriptEngine>
#include <QScriptValue>
#include <QTextStream>
#include <QFile>
#include <QTimer>
#include <QDesktopServices>
#include <QUrl>

// #include <QtSensors/QSensor>

#include <KDebug>
#include <KStandardDirs>

#include "sensors/dbus/DBusSensor.h"

namespace Contour {

RecommendationScriptEngineConfig::RecommendationScriptEngineConfig(QObject * parent, KConfigGroup * config)
    : QObject(parent), m_config(config)
{
}

RecommendationScriptEngineConfig::~RecommendationScriptEngineConfig()
{
    delete m_config;
}

#define CREATE_CONFIG_METHODS(TYPE, TYPECASE) \
    TYPE RecommendationScriptEngineConfig::TYPECASE##Value(const QString & field, TYPE defaultValue) const \
    { return m_config->readEntry(field, defaultValue); } \
    void RecommendationScriptEngineConfig::Set##TYPECASE##Value(const QString & field, TYPE newValue) const \
    { kDebug() << field << newValue; m_config->writeEntry(field, newValue); m_config->sync(); }

    CREATE_CONFIG_METHODS(bool, Bool)
    CREATE_CONFIG_METHODS(int, Int)
    CREATE_CONFIG_METHODS(QString, String)
    CREATE_CONFIG_METHODS(QStringList, StringList)
#undef CREATE_CONFIG_METHODS


/**
 *
 */
RecommendationScriptEngine::RecommendationScriptEngine(QObject * parent, const QString & script)
    : RecommendationEngine(parent), d(new Private())
{
    d->script = script;

    kDebug() << "RecommendationScriptEngine()" << script << name();

    d->delay.setInterval(300);
    d->delay.setSingleShot(true);

    d->autoremove = config()->readEntry("autoRemoveChosenRecommendation", true);

    connect(&(d->delay), SIGNAL(timeout()),
            this, SLOT(sendUpdateNotification()));

}

RecommendationScriptEngine::~RecommendationScriptEngine()
{
    delete d;
}

void RecommendationScriptEngine::init()
{
    Contour::RecommendationEngine::init();

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

    const QScriptValue & result = d->engine->evaluate(QTextStream(&file).readAll());
    if (d->engine->hasUncaughtException()) {
        int line = d->engine->uncaughtExceptionLineNumber();
        kDebug() << "uncaught exception at line" << line << ":" << result.toString();
    }

}

void RecommendationScriptEngine::activate(const QString & id, const QString & action)
{
    if (d->autoremove)
        removeRecommendation(id);

    emit activationRequested(id, action);
}

void RecommendationScriptEngine::setAutoRemoveChosenRecommendation(bool remove)
{
    d->autoremove = remove;
    KConfigGroup * cg = config();
    cg->writeEntry("autoRemoveChosenRecommendation", d->autoremove);
    cg->sync();
}

QString RecommendationScriptEngine::name() const
{
    kDebug() << d->script;
    return d->script;
}

QScriptValue RecommendationScriptEngine::getSensor(const QString & sensor)
{
    kDebug() << "sensor" << sensor;

    QObject * result = NULL;

    if (sensor == "DBus") {
        kDebug() << "Returning the D-Bus sensor. This is not in QtMobility";
        result = new DBusSensor();
    } else {
        // result = new QtMobility::QSensor(sensor.toAscii());
    }

    return d->engine->newQObject(result, QScriptEngine::AutoOwnership);
}

QScriptValue RecommendationScriptEngine::getConfig()
{
    return d->engine->newQObject(new RecommendationScriptEngineConfig(this, config()));
}

QScriptValue RecommendationScriptEngine::getTimer(int msec)
{
    kDebug() << "timer" << msec;

    QTimer * timer = new QTimer();
    timer->setSingleShot(false);
    timer->setInterval(msec);
    timer->start();

    return d->engine->newQObject(timer, QScriptEngine::AutoOwnership);
}

void RecommendationScriptEngine::openUrl(const QString & url)
{
    QDesktopServices::openUrl(QUrl(url));
}

void RecommendationScriptEngine::signalHandlerException(const QScriptValue & exception)
{
    kDebug() << "exception" << exception.toVariant();
}

void RecommendationScriptEngine::addRecommendation(
        double score,
        const QString & id,
        const QString & title,
        const QString & description,
        const QString & icon
    )
{
    // kDebug() << d->script << score << id << title;

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

    delayedUpdateNotification();
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

    delayedUpdateNotification();
}

void RecommendationScriptEngine::removeRecommendations()
{
    d->recommendations.clear();

    delayedUpdateNotification();
}

void RecommendationScriptEngine::delayedUpdateNotification()
{
    d->delay.start();
}

void RecommendationScriptEngine::sendUpdateNotification()
{
    // just in case, although it is a single-shot
    d->delay.stop();

    emit recommendationsUpdated(d->recommendations);
}


} // namespace Contour

// class RecommendationScriptEngine

