/****************************************************************************
**
** Copyright (C) 2016
**
** This file is generated by the Magus toolkit
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
****************************************************************************/

// Include
#include "stdafx.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include <QFile>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QCloseEvent>
#include <QDockWidget>
#include <QSettings>
#include <QFileDialog>
#include <QStandardPaths>
#include <QSettings>
#include <QDebug>
#include <QMessageBox>

#include "lightwidget.h"
#include "ogremanager.h"
#include "ogrewidget.h"
#include "objimporter.h"
#include "objexporter.h"
#include "OgreMesh2Serializer.h"


MainWindow::MainWindow()
{
    ui = new Ui::MainWindow;
    ui->setupUi(this);

    mOgreManager = new OgreManager;
    mOgreWidget = new OgreWidget;
    setCentralWidget(mOgreWidget);

    mOgreManager->registerOgreWidget(mOgreWidget);
    mOgreWidget->createRenderWindow(mOgreManager);

    mOgreManager->initialize();
    mOgreWidget->createCompositor();

    mTimer = new QTimer(this);
    mTimer->setInterval(16);
    connect(mTimer, &QTimer::timeout, this, &MainWindow::Tick);

	createDockWindows();
	
	// Set the title
	setWindowTitle( QString( "Ogre v2 Mesh Viewer v%1" ).arg(APP_VERSION_NUMBER));
	readSettings();

    connect(mOgreManager, &OgreManager::sceneCreated, this, &MainWindow::onSceneLoaded);

    // actions
    connect(ui->actionOpenOgreMesh, &QAction::triggered, this, &MainWindow::actionOpenMesh);
    connect(ui->actionSaveOgreMesh, &QAction::triggered, this, &MainWindow::actionSaveMesh);
    connect(ui->actionImportObj, &QAction::triggered, this, &MainWindow::actionImportObj);
    connect(ui->actionExportObj, &QAction::triggered, this, &MainWindow::actionExportObj);

    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::doQuitMenuAction);
}

MainWindow::~MainWindow()
{
	delete mOgreManager;
    delete ui;
}

void MainWindow::closeEvent( QCloseEvent* event )
{
	mIsClosing = true;

    mTimer->stop();

	QSettings settings( "DisplaySweet", "OgreModelViewer" );
	settings.setValue( "geometry", saveGeometry() );
	settings.setValue( "windowState", saveState() );
	QMainWindow::closeEvent( event );
}

void MainWindow::readSettings()
{
	QSettings settings( "DisplaySweet", "OgreModelViewer" );
	restoreGeometry( settings.value( "geometry" ).toByteArray() );
	restoreState( settings.value( "windowState" ).toByteArray() );
}

void MainWindow::onSceneLoaded()
{
}

void MainWindow::Tick()
{
    if (mOgreManager)
        mOgreManager->renderOgreWidgetsOneFrame();
}

void MainWindow::createDockWindows()
{
    QDockWidget* pDockWidget1 = new QDockWidget("Lights", this);
    addDockWidget(Qt::LeftDockWidgetArea, pDockWidget1);
    
    mLightWidget = new LightWidget(this);
    pDockWidget1->setWidget(mLightWidget);

    mLightWidget->init(mOgreManager);
}

void MainWindow::startTimer()
{
    mOgreManager->createScene();
    mTimer->start();
}

void MainWindow::doQuitMenuAction()
{
	close();
}

void MainWindow::actionOpenMesh()
{
    mTimer->stop();
    ON_SCOPE_EXIT(mTimer->start());

    mOgreManager->clearScene();

    QString sUserDoc = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)[0];

    QSettings settings("OgreSpoooky", "OgreSpoooky");
    QString sLastOpenLocation = settings.value("actionOpenMesh", sUserDoc).toString();

    QString sMeshFileName = QFileDialog::getOpenFileName(this, "Open Ogre Mesh",
                                                        sLastOpenLocation,
                                                        "Ogre Mesh (*.mesh)");
    if (sMeshFileName.isEmpty())
    {
        return;
    }

    Q_ASSERT(QFile::exists(sMeshFileName));
    QFileInfo info(sMeshFileName);
    settings.setValue("actionOpenMesh", info.absolutePath());
    
    auto& manager = Ogre::ResourceGroupManager::getSingleton();
    manager.addResourceLocation(info.absolutePath().toStdString(), "FileSystem", "OgreSpooky");

    auto all_mtl = manager.findResourceNames("OgreSpooky", "*.material.json");
    for (std::string m : *all_mtl)
    {
        Ogre::Root::getSingleton().getHlmsManager()->loadMaterials(m, "OgreSpooky");
    }

    try
    {
        mOgreManager->loadMesh(sMeshFileName);
    }
    catch (...)
    {
        qDebug() << "Ogre mesh load failed";
        QMessageBox::information(this, "Error", "Filed to load ogre mesh");
    }
}

void MainWindow::actionSaveMesh()
{
    mTimer->stop();
    ON_SCOPE_EXIT(mTimer->start());

    QString sUserDoc = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)[0];

    QSettings settings("OgreSpoooky", "OgreSpoooky");
    QString sLastOpenLocation = settings.value("actionSaveMesh", sUserDoc).toString();

    QString sMeshFileName = QFileDialog::getSaveFileName(this, "Save Ogre Mesh",
                                                         sLastOpenLocation + "/a.mesh",
                                                         "Ogre Mesh (*.mesh)");
    if (sMeshFileName.isEmpty())
    {
        return;
    }

    QFileInfo info(sMeshFileName);
    settings.setValue("actionSaveMesh", info.absolutePath());

    if (QFile::exists(sMeshFileName)) QFile::remove(sMeshFileName);
    Q_ASSERT(!QFile::exists(sMeshFileName));

    Ogre::Mesh* mesh = mOgreManager->currentMesh();
    if (mesh != nullptr)
    {
        Ogre::Root* root = Ogre::Root::getSingletonPtr();
        Ogre::MeshSerializer meshSerializer2(root->getRenderSystem()->getVaoManager());
        meshSerializer2.exportMesh(mesh, sMeshFileName.toStdString());

        if (!QFile::exists(sMeshFileName))
        {
            qDebug() << "Failed to export obj model.";
            QMessageBox::information(this, "Error", "Filed to export obj model");
        }
    }
}

void MainWindow::actionImportObj()
{
    mOgreManager->clearScene();

    mTimer->stop();
    ON_SCOPE_EXIT(mTimer->start());

    QString sUserDoc = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)[0];
    QSettings settings( "OgreSpoooky", "OgreSpoooky" );
    QString sLastOpenLocation = settings.value("actionImportObj", sUserDoc).toString();

    QString sObjFileName = QFileDialog::getOpenFileName(this, "Open Obj", 
                                                        sLastOpenLocation,
                                                        "Wavefront obj (*.obj)");
    if (sObjFileName.isEmpty())
    {
        return;
    }

    Q_ASSERT(QFile::exists(sObjFileName));

    QFileInfo info(sObjFileName);
    settings.setValue("actionImportObj", info.absolutePath());

    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(info.absolutePath().toStdString(), "FileSystem", "OgreSpooky");
    
    QString sOutFile = info.absolutePath() + "/" + info.baseName() + ".mesh";

    ObjImporter objImporter;
    bool b = objImporter.import(sObjFileName.toStdString(), sOutFile.toStdString() );

    qDebug() << "Obj=" << sObjFileName << ", Success=" << b;

    if (b)
    {
        try
        {
            mOgreManager->loadMesh(sOutFile);
        }
        catch (...)
        {
            qDebug() << "Failed to Load obj.";
        }
    }
    else
    {
        qDebug() << "Failed to import obj model.";
        QMessageBox::information(this, "Error", "Filed to import obj model");
    }
}

void MainWindow::actionExportObj()
{
    mTimer->stop();
    ON_SCOPE_EXIT(mTimer->start());

    QString sUserDoc = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)[0];
    QSettings settings("OgreSpoooky", "OgreSpoooky");
    QString sLastOpenLocation = settings.value("actionExportObj", sUserDoc).toString();

    QString sObjFileName = QFileDialog::getSaveFileName(this, "Export Obj",
                                                        sLastOpenLocation + "/a.obj",
                                                        "Wavefront obj (*.obj)");
    if (sObjFileName.isEmpty())
    {
        return;
    }

    //QString sObjFileName = "C:/Users/Matt/Desktop/a.obj";

    if (QFile::exists(sObjFileName)) QFile::remove(sObjFileName);

    Q_ASSERT(!QFile::exists(sObjFileName));

    QFileInfo info(sObjFileName);
    settings.setValue("actionExportObj", info.absolutePath());

    Ogre::Mesh* mesh = mOgreManager->currentMesh();
    if (mesh != nullptr)
    {
        ObjExporter objExporter;
        bool ok = objExporter.exportFile(mesh, sObjFileName);

        qDebug() << "Obj=" << sObjFileName << ", Success=" << ok;

        if (!ok)
        {
            qDebug() << "Failed to export obj model.";
            QMessageBox::information(this, "Error", "Filed to export obj model");
        }
    }
}
