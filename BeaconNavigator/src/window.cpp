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
#include "window.h"
#include "logger.h"
#include "beacons.h"
#include <QPushButton>
#include <QGridLayout>
#include <QCoreApplication>
#include <QTextStream>

Window::Window(){
    initRenderArea();
    initBeacons();
    initFlat();
}

void Window::initRenderArea(){
    m_render_area = new RenderArea;
    m_navigator_button = new QPushButton(m_start_navigating_text, this);
    m_navigator_button->setMinimumHeight(200);
    m_estimation_button = new QPushButton(m_multilateration_text, this);
    m_estimation_button->setMinimumHeight(100);
    m_quit_button = new QPushButton(m_quit_text, this);
    m_quit_button->setMinimumHeight(200);

    connect(m_navigator_button, SIGNAL(clicked()),
            this, SLOT(navigatorButtonClicked()));
    connect(m_estimation_button, SIGNAL(clicked()),
            this, SLOT(estimationButtonClicked()));
    connect(m_quit_button, SIGNAL(clicked()),
            this, SLOT(quitButtonClicked()));

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(m_render_area, 0, 0, 1, 4);
    mainLayout->addWidget(m_navigator_button, 1, 0, 1, 4);
    mainLayout->addWidget(m_estimation_button, 2, 0, 1, 4);
    mainLayout->addWidget(m_quit_button, 3, 0, 1, 4);
    setLayout(mainLayout);

    setWindowTitle(tr("Beacon Navigator"));
}

void Window::initBeacons(){
    m_beacons = new Beacons;
    m_beacons->initModules();

    connect(m_beacons, SIGNAL(positionChanged(Point)),
            m_render_area, SLOT(setPosition(Point)));
    connect(m_beacons, SIGNAL(showPosition(bool)),
            m_render_area, SLOT(showPosition(bool)));
    connect(m_beacons, SIGNAL(navigatorStateChanged(bool)), this, SLOT(navigatorStateChanged(bool)));

    for(DistanceToBeacon& dtb: m_beacons->getDistances()){
        m_render_area->addBeacon(dtb.point.x, dtb.point.y);
    }
}

void Window::initFlat(){
    QFile flat_file("/sdcard/Download/flat.txt");
    if (!flat_file.open(QIODevice::ReadOnly | QIODevice::Text)){
        Logger::logMessage("Can't open flat file!");
        return;
    }
    QTextStream in(&flat_file);
    Logger::logMessage("Flat: ");
    while (!in.atEnd()){
        QString line = in.readLine();
        QStringList words = line.split(" ");
        QStringList first_position = words[0].split(",");
        double x1 = first_position[0].toDouble();
        double y1 = first_position[1].toDouble();
        QStringList second_position = words[1].split(",");
        double x2 = second_position[0].toDouble();
        double y2 = second_position[1].toDouble();
        Logger::logMessage(QString::number(x1) + "," + QString::number(y1) + " " +
                           QString::number(x2) + "," + QString::number(y2));
        m_render_area->addWall(x1, y1, x2, y2);
    }
    m_render_area->calcFlatAspectRatio();
}

void Window::navigatorStateChanged(bool state){
    if(state){
        m_navigator_button->setText(m_stop_navigating_text);
    } else {
        m_navigator_button->setText(m_start_navigating_text);
    }
}

void Window::navigatorButtonClicked(){
    Logger::logMessage("Window::navigatorButtonClicked");
    if(!m_beacons->isNavigating()){
        m_beacons->startNavigation();
    } else {
        m_beacons->stopNavigation();
    }
}

void Window::estimationButtonClicked(){
    Logger::logMessage("Window::estimationButtonClicked");
    m_beacons->changeEstimation();
    if(m_beacons->getEstimationType() == multilateration){
        m_estimation_button->setText(m_multilateration_text);
    } else {
        m_estimation_button->setText(m_weighted_arith_mean_text);
    }
}

void Window::quitButtonClicked(){
    Logger::logMessage("Window::quitButtonClicked");
    QCoreApplication::quit();
}
