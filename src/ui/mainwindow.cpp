/*
 * LibreTuner
 * Copyright (C) 2018 Altenius
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mainwindow.h"
#include "libretuner.h"

#include "datalog.h"

#include "diagnosticswidget.h"
#include "flowlayout.h"
#include "logview.h"
#include "romwidget.h"
#include "titlebar.h"
#include "tunemanager.h"
#include "tunewidget.h"
#include "sidebarwidget.h"
#include "romswidget.h"

#include <QAction>
#include <QDockWidget>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QWindowStateChangeEvent>

#include <future>


MainWindow::MainWindow(QWidget *parent) : StyledWindow(parent) {
    resize(QSize(1100, 630));

    setTitle("LibreTuner");

    main_ = new QMainWindow;
    layout_->addWidget(main_);

    setupMenu();

    // Blank central widget
    QLabel *central = new QLabel("TEST");
    main_->setCentralWidget(central);
    central->hide();

    main_->setDockOptions(main_->dockOptions() | QMainWindow::AllowNestedDocks);
    main_->setDocumentMode(true);

    // Create docks
    logDock_ = createLogDock();
    overviewDock_ = createOverviewDock();
    tunesDock_ = createTunesDock();
    romsDock_ = createRomsDock();
    loggingDock_ = createLoggingDock();
    diagnosticsDock_ = createDiagnosticsDock();
    sidebarDock_ = createSidebarDock();

    // Setup corners
    main_->setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
    main_->setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);

    // Place docks

    // Bottom
    main_->addDockWidget(Qt::BottomDockWidgetArea, logDock_);

    // Roms | Central | Sidebar
    main_->addDockWidget(Qt::TopDockWidgetArea, romsDock_);
    main_->splitDockWidget(romsDock_, overviewDock_, Qt::Horizontal);
    main_->splitDockWidget(overviewDock_, sidebarDock_, Qt::Horizontal);

    // Left
    main_->splitDockWidget(romsDock_, tunesDock_, Qt::Vertical);

    // Top (central)

    main_->tabifyDockWidget(overviewDock_, loggingDock_);
    main_->tabifyDockWidget(loggingDock_, diagnosticsDock_);


    connect(RomManager::get(), &RomManager::updateRoms, this,
            &MainWindow::updateRoms);
    connect(TuneManager::get(), &TuneManager::updateTunes, this,
            &MainWindow::updateTunes);
    updateRoms();
    updateTunes();
}



QDockWidget *MainWindow::createLoggingDock() {
    QDockWidget *dock = new QDockWidget("Logging", this);
    auto *layout = new QVBoxLayout();

    auto *hlayout = new QHBoxLayout();
    layout->addLayout(hlayout);

    QPushButton *buttonNewLog = new QPushButton(tr("New Log"));
    hlayout->addWidget(buttonNewLog);
    connect(buttonNewLog, &QPushButton::clicked, this,
            &MainWindow::newLogClicked);

    comboLogVehicles_ = new QComboBox();
    QSizePolicy policy = comboLogVehicles_->sizePolicy();
    policy.setHorizontalPolicy(QSizePolicy::Expanding);
    comboLogVehicles_->setSizePolicy(policy);
    hlayout->addWidget(comboLogVehicles_);

    listLogs_ = new QListView;
    layout->addWidget(listLogs_);

    dock->setLayout(layout);
    return dock;
}



QDockWidget *MainWindow::createDiagnosticsDock() {
    QDockWidget *dock = new QDockWidget("Diagnostics", this);
    dock->setWidget(new DiagnosticsWidget);
    return dock;
}



QDockWidget *MainWindow::createLogDock() {
    LogView *log = new LogView;
    log->setModel(&LibreTuner::get()->log());

    QDockWidget *dock = new QDockWidget("Log", this);
    dock->setWidget(log);
    return dock;
}



QDockWidget *MainWindow::createSidebarDock()
{
    QDockWidget *dock = new QDockWidget("Sidebar", this);
    dock->setWidget(new SidebarWidget);
    return dock;
}



QDockWidget *MainWindow::createRomsDock() {
    QDockWidget *dock = new QDockWidget("ROMs", this);

    RomsWidget *roms = new RomsWidget(dock);
    RomsModel *model = new RomsModel(roms);
    model->setRoms(RomManager::get());
    model->setTunes(TuneManager::get());
    roms->setModel(model);
    dock->setWidget(roms);

    /*QWidget *widget = new QWidget;
    auto *layout = new QVBoxLayout();

    auto *area = new QScrollArea;
    area->setWidgetResizable(true);

    area->setStyleSheet(
        "QScrollArea { background: transparent; }\n"
        "QScrollArea > QWidget > QWidget { background: transparent; }\n");
    layout->addWidget(area);

    QWidget *scrollContents = new QWidget();
    romsLayout_ = new FlowLayout();
    scrollContents->setLayout(romsLayout_);
    area->setWidget(scrollContents);

    QPushButton *buttonDownload = new QPushButton(tr("Download new ROM"));
    connect(buttonDownload, &QPushButton::clicked, this,
            &MainWindow::on_buttonDownloadRom_clicked);
    layout->addWidget(buttonDownload);

    widget->setLayout(layout);
    dock->setWidget(widget);*/
    return dock;
}



QDockWidget *MainWindow::createTunesDock() {
    QDockWidget *dock = new QDockWidget("Tunes", this);
    auto *layout = new QVBoxLayout();

    auto *area = new QScrollArea;
    area->setWidgetResizable(true);
    area->setStyleSheet(
        "QScrollArea { background: transparent; }\n"
        "QScrollArea > QWidget > QWidget { background: transparent; }\n"
        "QScrollArea > QWidget > QScrollBar { background: palette(base); }");
    layout->addWidget(area);

    QWidget *scrollContents = new QWidget();
    tunesLayout_ = new FlowLayout();
    scrollContents->setLayout(tunesLayout_);
    area->setWidget(scrollContents);

    QPushButton *buttonCreateTune = new QPushButton(tr("Create new tune"));
    layout->addWidget(buttonCreateTune);

    dock->setLayout(layout);
    return dock;
}


QDockWidget *MainWindow::createOverviewDock() { return new QDockWidget("Overview", this); }



void MainWindow::setupMenu() {
    auto *menuBar = new QMenuBar;
    QMenu *fileMenu = menuBar->addMenu(tr("&File"));
    QMenu *editMenu = menuBar->addMenu(tr("&Edit"));
    QMenu *helpMenu = menuBar->addMenu(tr("&Help"));
    QMenu *viewMenu = menuBar->addMenu(tr("&View"));

    QAction *logAct = viewMenu->addAction(tr("CAN Log"));
    connect(logAct, &QAction::triggered, [this] { canViewer_.show(); });

    QAction *interfacesAct = viewMenu->addAction(tr("Interfaces"));
    connect(interfacesAct, &QAction::triggered,
            [this] { interfacesWindow_.show(); });
    main_->setMenuBar(menuBar);
}

void MainWindow::updateTunes() {
    QLayoutItem *child;
    while ((child = tunesLayout_->takeAt(0)) != nullptr) {
        delete child;
    }

    for (const TuneMeta &tune : TuneManager::get()->tunes()) {
        tunesLayout_->addWidget(new TuneWidget(tune));
    }
}



void MainWindow::updateRoms() {
    /*QLayoutItem *child;
    while ((child = romsLayout_->takeAt(0)) != nullptr) {
        delete child;
    }

    for (const RomMeta &rom : RomManager::get()->roms()) {
        romsLayout_->addWidget(new RomWidget(rom));
    }*/
}



void MainWindow::on_buttonDownloadRom_clicked() {
    if (downloadWindow_) {
        delete downloadWindow_;
        downloadWindow_ = nullptr;
    }

    if (const auto &link = LibreTuner::get()->getVehicleLink()) {
        auto di = link->downloader();
        downloadWindow_ =
            new DownloadWindow(std::move(di), link->vehicle(), this);
        downloadWindow_->show();
    }
}



void MainWindow::newLogClicked() {
    std::unique_ptr<VehicleLink> link = LibreTuner::get()->getVehicleLink();
    if (!link) {
        return;
    }

    std::unique_ptr<DataLogger> logger;

    try {
        logger = link->logger();
        if (!logger) {
            QMessageBox(QMessageBox::Critical, "Logger error",
                        "Failed to create a usable datalogger. The datalink "
                        "may not support the needed protocol or there is no "
                        "log mode set in the definition file.")
                .exec();
            return;
        }
    } catch (const std::exception &e) {
        QMessageBox(QMessageBox::Critical, "Logger error",
                    QStringLiteral(
                        "An error occurred while creating the datalogger: ") +
                        e.what())
            .exec();
        return;
    }
    DataLogPtr log = std::make_shared<DataLog>();
    loggerWindow_ = std::make_unique<DataLoggerWindow>(
        log, std::move(logger), link->vehicle().definition);
    loggerWindow_->show();
}



void MainWindow::closeEvent(QCloseEvent * /*event*/) {
    canViewer_.close();
    interfacesWindow_.close();
}
