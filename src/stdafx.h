
#pragma once

// std headers
#include <string>
#include <vector>
#include <set>
#include <functional>

// windows header
#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif 

// common ogre headers
#include "OgrePrerequisites.h"
#include "OgreString.h"
#include "OgreRoot.h"
#include "OgreCamera.h"
#include "OgreItem.h"

#include "OgreHlmsManager.h"

// common qt headers
#include <QString>
#include <QWidget>
#include <QDebug>

#include "scopeguard.h"
