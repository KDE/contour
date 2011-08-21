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

#ifndef RECOMMENDATIONMANAGER_H
#define RECOMMENDATIONMANAGER_H

#include <QObject>
#include <QtCore/QList>

namespace Contour {

class RecommendationManager : public QObject
{
    Q_OBJECT
    // Q_PROPERTY(QList<Contour::Recommendation> recommendations READ recommendations)

public:
    RecommendationManager(QObject* parent = 0);
    ~RecommendationManager();

private:
    class Private;
    Private* const d;
};

}


#endif
