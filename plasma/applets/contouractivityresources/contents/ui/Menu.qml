/*
 *   Copyright 2011 Marco Martin <mart@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2 or
 *   (at your option) any later version.
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

import QtQuick 1.0
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.mobilecomponents 0.1 as MobileComponents

Item {
    id: menuObject
    width: menuFrame.width
    height: childrenRect.height
    property alias positionState: tipSvgItem.state
    property alias tipX: tipSvgItem.x

    function activateItem(x, y)
    {
        var pos = entriesColumn.mapFromItem(delegate, x, y)
        var item = entriesColumn.childAt(pos.x, pos.y)
        if (item) {
            print("You clicked " + item.text)
            feedbackMessageText.text = item.text
            feedbackMessageAnimation.running = true
            item.activated()
        }
    }

    function highlightItem(x, y)
    {
        var pos = entriesColumn.mapFromItem(delegate, x, y)
        var item = entriesColumn.childAt(pos.x, pos.y)
        if (item && item.text) {
            var itemPos = menuObject.mapFromItem(item, 0, 0)
            highlightFrame.x = -highlightFrame.margins.right + itemPos.x
            highlightFrame.y = -highlightFrame.margins.top + itemPos.y
            highlightFrame.width = entriesColumn.width + highlightFrame.margins.right + highlightFrame.margins.left
            highlightFrame.height = item.height + highlightFrame.margins.top + highlightFrame.margins.bottom
            highlightFrame.opacity = 1
        } else {
            highlightFrame.opacity = 0
        }
    }

    PlasmaCore.SvgItem {
        id: tipSvgItem
        svg: tipSvg
        elementId: "baloon-tip-top"
        width: tipSvg.elementSize(elementId).width
        height: tipSvg.elementSize(elementId).height
        x: parent.width/2-width/2
        state: "top"
        z: 900
        anchors {
            bottomMargin: -tipSvg.elementSize("hint-top-shadow").height
            topMargin: -tipSvg.elementSize("hint-bottom-shadow").height
        }

        states: [
            State {
                name: "top"
                PropertyChanges {
                    target: tipSvgItem
                    elementId: "baloon-tip-bottom"
                }
                AnchorChanges {
                    target: tipSvgItem
                    anchors.top: menuFrame.bottom
                }
            },
            State {
                name: "bottom"
                PropertyChanges {
                    target: tipSvgItem
                    elementId: "baloon-tip-top"
                }
                AnchorChanges {
                    target: tipSvgItem
                    anchors.bottom: menuFrame.top
                }
            }
        ]
    }

    PlasmaCore.FrameSvgItem {
        id: menuFrame
        imagePath: "dialogs/background"
        width: entriesColumn.width + margins.left + margins.right + highlightFrame.margins.left + highlightFrame.margins.right
        height: entriesColumn.height + margins.top + margins.bottom + highlightFrame.margins.top + highlightFrame.margins.bottom
        onHeightChanged: positionMenu()

        PlasmaCore.FrameSvgItem {
            id: highlightFrame
            imagePath: "widgets/viewitem"
            prefix: "hover"
            opacity: 0
            Behavior on y {
                NumberAnimation {
                    duration: 250
                    easing.type: Easing.InOutQuad
                }
            }
            Behavior on opacity {
                NumberAnimation {
                    duration: 250
                    easing.type: Easing.InOutQuad
                }
            }
        }

        Column {
            id: entriesColumn
            x: menuFrame.margins.left + highlightFrame.margins.left
            y: menuFrame.margins.top + highlightFrame.margins.top
            spacing: 5
            Repeater {
                model: actionsModel.model(resourceType)
                Column {
                    spacing: 5
                    property alias text: menuItem.text
                    MenuItem {
                        id: menuItem
                        text: model.text
                    }
                    PlasmaCore.SvgItem {
                        svg: lineSvg
                        elementId: "horizontal-line"
                        width: entriesColumn.width
                        height: lineSvg.elementSize("horizontal-line").height
                    }
                }
            }
            AddToActivityItem {
                
            }
        }
    }
}