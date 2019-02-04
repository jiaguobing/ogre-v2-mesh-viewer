/****************************************************************************
**
** Copyright (C) 2014 This file is generated by the Magus toolkit
** Copyright (C) 2016-2019 Matt Chiawen Chang
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

#pragma once

#include <QMainWindow>

class OgreManager;
class OgreWidget;
class QTimer;
class QMenu;
class QAction;
class QCloseEvent;
class LightWidget;
class Dock1DockWidget;
class Dock2DockWidget;

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

    bool mIsClosing = false;
    OgreManager* getOgreManager() const { return mOgreManager; }

    void startTimer();
    void doQuitMenuAction();

private:
    void createDockWindows();
    void closeEvent(QCloseEvent* event) override;
    void readSettings();

    void onSceneLoaded();
    void Tick();

private:
    void actionOpenMesh();
    void actionSaveMesh();
    void actionImportObj();
    void actionExportObj();
    void actionLoadFromFolder();
    void actionBatchConverter();

    OgreWidget* mOgreWidget = nullptr;
    OgreManager* mOgreManager = nullptr;

    LightWidget* mLightWidget = nullptr;
    Dock2DockWidget* mDock2DockWidget = nullptr;

    QTimer* mTimer = nullptr;

    Ui::MainWindow* ui = nullptr;
};
