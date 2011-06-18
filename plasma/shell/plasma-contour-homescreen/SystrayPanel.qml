/***************************************************************************
 *   Copyright 2010 Lim Yuen Hoe <yuenhoe@hotmail.com>                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

import Qt 4.7
import org.kde.plasma.core 0.1 as PlasmaCore

Item {
    id: systrayPanel;
    anchors {
        left:parent.left
        right:parent.right
    }
    height: 48

    Item {
        id: containmentParent
        anchors.fill: parent
    }

    property QGraphicsWidget containment

    onContainmentChanged: {
        containment.parent = containmentParent
        resizeTimer.running = true
    }
    onHeightChanged: resizeTimer.running = true
    onWidthChanged: resizeTimer.running = true

    function updateState()
    {
        state = containment.state
    }

    Timer {
        id: resizeTimer
        interval: 500
        running: false
        repeat: false
        onTriggered: resizeContainment()
     }

    function resizeContainment()
    {
        containment.x = 0
        containment.y = 0
        containment.height = containmentParent.height
        containment.width = containmentParent.width
    }

}
