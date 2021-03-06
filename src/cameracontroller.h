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

#pragma once

#include "Ogre.h"
#include <limits>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>

enum CameraMode   // enumerator values for different styles of camera movement
{
    CM_BLENDER,
    CM_FLY,	    // WIP
    CM_ORBIT    // WIP
};

enum View
{
    VI_TOP,
    VI_LEFT,
    VI_BOTTOM,
    VI_RIGHT,
    VI_FRONT,
    VI_BACK,
    VI_USER
};

enum Direction
{
    DR_FORWARD,
    DR_BACKWARD,
    DR_LEFT,
    DR_RIGHT
};

/****************************************************************************
 * This class manages the ogre camera and transalates Qt events to camera
 * actions
***************************************************************************/
class CameraController
{
public:
    CameraController(Ogre::Camera* cam);
    ~CameraController();

    void reset();

    void setCamera(Ogre::Camera* cam);
    Ogre::Camera* getCamera() { return mCamera; }

    void setTarget(Ogre::SceneNode* target);
    Ogre::SceneNode* getTarget() { return mTarget; }

    void manualStop();
    Ogre::Real getDistanceFromTarget() { return mDistFromTarget; }

    void setYawPitchDist(Ogre::Radian yaw, Ogre::Radian pitch, Ogre::Real dist);

    void setMode(CameraMode mode);
    CameraMode getMode() { return mMode; }

    void setProjectionType(Ogre::ProjectionType pt);
    Ogre::ProjectionType getProjectionType() { return mCamera->getProjectionType(); }

    View getView() { return mCurrentView; }
    void setView(View newView);

    void rotatePerspective(Direction dir);
    void numpadViewSwitch(const QKeyEvent* evt);

    // Per-frame updates.
    bool frameRenderingQueued(const Ogre::FrameEvent& evt);

    void keyPress(const QKeyEvent* evt);
    void keyRelease(const QKeyEvent* evt);

    void mouseMove(Ogre::Vector2 mousePos);
    void mouseWheel(const QWheelEvent* evt);
    void mousePress(const QMouseEvent* evt);

    /*-----------------------------------------------------------------------------
    | Processes mouse releases.
    | Left button is for orbiting, and right button is for zooming.
    -----------------------------------------------------------------------------*/
    void mouseRelease(const QMouseEvent* evt); // Only applies for orbit style.

    void rotate(int x, int y);
    void pan(float x, float y);

private:
    Ogre::Camera* mCamera = nullptr;

    Ogre::SceneNode* mTarget = nullptr;
    Ogre::SceneNode* mCameraNode = nullptr;

    bool mOrbiting = false;
    bool mShiftDown = false;
    bool mGoingForward = false;
    bool mGoingBack = false;
    bool mGoingLeft = false;
    bool mGoingRight = false;
    bool mGoingUp = false;
    bool mGoingDown = false;

    Ogre::Real mDistFromTarget = 0.0;
    Ogre::Real mTopSpeed = 15;
    Ogre::Vector3 mVelocity = Ogre::Vector3::ZERO;
    int mMouseWheelDelta = 0;

    View mCurrentView = VI_USER;
    CameraMode mMode = CM_BLENDER;
};

