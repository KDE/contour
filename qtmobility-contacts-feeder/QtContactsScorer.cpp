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


#include "QtContactsScorer.h"

#include <QtMessaging/QMessageManager>
#include <QtMessaging/QMessageAccount>

#include <KDebug>

using namespace QtMobility;

/**
 *
 */
class QtContactsScorerPrivate {
public:
    QMessageManager * manager;

};

QtContactsScorer::QtContactsScorer()
    : d(new QtContactsScorerPrivate())
{
    d->manager = new QMessageManager();

    kDebug() << "We have the following accounts:";

    foreach (const QMessageAccountId id, d->manager->queryAccounts()) {
        kDebug() << "Message account" << id.toString();
    }

    kDebug() << "[acc list end]";
}

QtContactsScorer::~QtContactsScorer()
{
    delete d->manager;
    delete d;
}

void QtContactsScorer::run()
{

}

// class QtContactsScorer


