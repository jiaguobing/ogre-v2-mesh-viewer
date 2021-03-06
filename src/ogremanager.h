/****************************************************************************
**
** Copyright (C) 2016 This file is generated by the Magus toolkit
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

#include <QObject>


namespace Ogre
{
class Root;
class Timer;
class RenderSystem;
class SceneManager;

namespace v1
{
class OverlaySystem;
}
}

class MyHlmsListener;
class OgreWidget;
class MeshLoader;

static const std::string OGRE_RENDERSYSTEM_DIRECTX11 = "Direct3D11 Rendering Subsystem";
static const std::string OGRE_RENDERSYSTEM_OPENGL3PLUS = "OpenGL 3+ Rendering Subsystem";


class OgreManager : public QObject
{
    Q_OBJECT

public:
    OgreManager();
    virtual ~OgreManager();
    void initialize();
    void render();

    void createScene();
    void clearScene();    

    int  registerOgreWidget(OgreWidget* ogreWidget);
    void unregisterOgreWidget(int id);

    Ogre::Root* ogreRoot() const { return mRoot; }
    Ogre::SceneManager* sceneManager() const { return mSceneManager; }
    Ogre::SceneNode* meshRootNode() const;
    MeshLoader* meshLoader() { return mMeshLoader; }

    bool isRenderSystemGL() const;
    HGLRC getGlContext() const;
    void setGlContext(HGLRC glContext);

    Ogre::Mesh* currentMesh(int index = 0);

    Q_SIGNAL void sceneCreated();

    void createSubcomponents();

    void setIrradianceBackground();
    void setEnvironmentBackground();
    void setBlackBackground();

private:
    void setupResources();
    void registerHlms();
    void createBall(int x, int y);

private:
    std::vector<OgreWidget*> mOgreWidgets;
    int mIdCount = 0;

    Ogre::Root* mRoot = nullptr;
    Ogre::Timer* mTimer = nullptr;
    Ogre::RenderSystem* mCurrentRenderSystem = nullptr;

    Ogre::SceneManager* mSceneManager = nullptr;
    Ogre::v1::OverlaySystem* mOverlaySystem = nullptr;

    std::string mResourcesCfg;
    std::string mPluginsCfg;

    HGLRC mGlContext = 0;

    Ogre::SceneNode* mMeshRootNode = nullptr;

    std::vector<Ogre::MeshPtr> mLoadedV2Meshes;
    std::vector<Ogre::v1::MeshPtr> mLoadedV1Meshes;

    MeshLoader* mMeshLoader = nullptr;
};
