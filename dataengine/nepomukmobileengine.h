/*
 *   Copyright (C) 2010 Marco Martin <mart@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License version 2 as
 *   published by the Free Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef NEPOMUKTESTENGINE_H
#define NEPOMUKTESTENGINE_H

#include <plasma/dataengine.h>


class NepomukMobileTest : public Plasma::DataEngine
{
    Q_OBJECT

public:
    NepomukMobileTest(QObject* parent, const QVariantList& args);
    ~NepomukMobileTest();

protected:
    //from DataEngine
    bool sourceRequestEvent(const QString &name);

protected slots:
    bool updateSourceEvent(const QString &name);

private:


};

K_EXPORT_PLASMA_DATAENGINE(nepomukmobiletest, NepomukMobileTest)

#endif
