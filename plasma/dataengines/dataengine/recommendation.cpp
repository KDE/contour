/*
   This file is part of the Nepomuk KDE project.
   Copyright (C) 2011 Sebastian Trueg <trueg@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) version 3, or any
   later version accepted by the membership of KDE e.V. (or its
   successor approved by the membership of KDE e.V.), which shall
   act as a proxy defined in Section 6 of version 3 of the license.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "recommendation.h"
#include "recommendationaction.h"

#include <Nepomuk/Resource>


class Contour::Recommendation::Private
{
public:
  //  Nepomuk::Resource m_resource;
    qreal m_relevance;
    QString m_name;
};


Contour::Recommendation::Recommendation()
    : QObject(),
      d(new Private())
{
}

Contour::Recommendation::~Recommendation()
{
    delete d;
}
/*
Nepomuk::Resource Contour::Recommendation::resource() const
{
    return d->m_resource;
}
*/
QList<Contour::RecommendationAction *> Contour::Recommendation::actions() const
{
    return QList<Contour::RecommendationAction *>();
}

void Contour::Recommendation::setActions(QList<Contour::RecommendationAction *>)
{}

QString Contour::Recommendation::name() const
{
    return d->m_name;
}

void Contour::Recommendation::setName(const QString &name)
{
    d->m_name = name;
}

qreal Contour::Recommendation::relevance() const
{
    return d->m_relevance;
}

void Contour::Recommendation::setRelevance(qreal relevance)
{
    d->m_relevance = relevance;
}

#include "recommendation.moc"
