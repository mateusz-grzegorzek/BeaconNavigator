/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "renderarea.h"
#include "point.h"
#include "logger.h"
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>

#define SAMSUNG_S5_NEO

RenderArea::RenderArea(QWidget *parent)
    : QWidget(parent){
}

void RenderArea::addWall(double x1, double y1, double x2, double y2){
    Wall wall;
    wall.start = {x1, y1};
    wall.end = {x2, y2};
    m_walls.append(wall);
}

void RenderArea::calcFlatAspectRatio(){
    double flat_width = 0;
    double flat_height = 0;
    for(Wall& wall: m_walls){
        double x = wall.start.x();
        double y = wall.start.y();
        if(flat_width < x){
            flat_width = x;
        }
        if(flat_height < y){
            flat_height = y;
        }
    }
    QRect rec = QApplication::desktop()->screenGeometry();
    double screen_width = rec.width();
    double screen_height = rec.height();

#ifdef SAMSUNG_S5_NEO
    screen_width -= 50;
    screen_height -= 720;
#endif

    double aspect_ratio_width;
    double aspect_ratio_height;

    if(screen_width > flat_width){
        aspect_ratio_width = screen_width/flat_width;
    } else {
        aspect_ratio_width = flat_width/screen_width;
    }

    if(screen_height > flat_height){
        aspect_ratio_height = screen_height/flat_height;
    } else {
        aspect_ratio_height = flat_height/screen_height;
    }

    if(aspect_ratio_width > aspect_ratio_height){
        m_aspect_ratio = aspect_ratio_height;
    } else {
        m_aspect_ratio = aspect_ratio_width;
    }
}

void RenderArea::setPosition(const Point &position){
    m_position.setX(position.x);
    m_position.setY(position.y);
    Logger::logMessage("RenderArea::setPosition=" + QString::number(m_position.x()) + "," + QString::number(m_position.y()));
    update();
}

void RenderArea::showPosition(bool show){
    m_show_position = show;
    update();
}

void RenderArea::paintEvent(QPaintEvent *){
    Logger::logMessage("RenderArea::paintEvent");
    QPainter painter(this);
    painter.translate(10, 10);

    for(Wall& wall: m_walls){
        QPen pen( Qt::black );
        pen.setWidth( 8 );
        painter.setPen(pen);
        painter.drawLine(wall.start*m_aspect_ratio, wall.end*m_aspect_ratio);
    }

    if(m_show_position){
        QPen pen( Qt::red );
        pen.setWidth( 7 );
        painter.setPen(pen);
        painter.drawEllipse(QPoint(m_position.x()*100*m_aspect_ratio, m_position.y()*100*m_aspect_ratio), 50, 50);
    }
}
