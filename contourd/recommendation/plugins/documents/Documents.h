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

#ifndef DOCUMENTS_H_
#define DOCUMENTS_H_

#include "RecommendationEngine.h"

/**
 *
 */
class DocumentsEnginePrivate;

class DocumentsEngine: public Contour::RecommendationEngine {
    Q_OBJECT
    RECOMMENDATION_ENGINE

public:
    DocumentsEngine(QObject * parent = 0, const QVariantList & args = QVariantList());
    virtual ~DocumentsEngine();

    virtual void init();
    virtual void activate(const QString & id, const QString & action = QString());

Q_SIGNALS:
    // note that you need to pass sorted items to
    // this method
    void recommendationsUpdated(const QList<Contour::RecommendationItem> & recommendations);

private:
    DocumentsEnginePrivate * const d;
    friend class DocumentsEnginePrivate;
};

#endif // DOCUMENTS_H_

