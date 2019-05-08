#include "simplemeshexporter.h"

#include <QFile>
#include <QTextStream>
#include "OgreMesh2Serializer.h"
#include "OgreMesh2.h"
#include "objexporter.h"

SimpleMeshExporter::SimpleMeshExporter()
{
}

void SimpleMeshExporter::setOutputFolder(QString s)
{
    mOutputFolder = s;
}

void SimpleMeshExporter::save(Ogre::SceneNode* rootNode)
{
    QFile f(mOutputFolder + "pos.txt");
    f.open(QFile::WriteOnly);

    mFout.setDevice(&f);

    saveOneNode(rootNode);

    mFout.flush();
    f.close();
}

void SimpleMeshExporter::saveOneNode(Ogre::SceneNode* node)
{
    for (int i = 0; i < node->numAttachedObjects(); ++i)
    {
        Ogre::MovableObject* movable = node->getAttachedObject(i);
        Ogre::Item* item = dynamic_cast<Ogre::Item*>(movable);
        if (item)
        {
            const Ogre::MeshPtr& mesh = item->getMesh();

            QString name = QString::fromStdString(node->getName()) + ".mesh";
            name.replace("_", ".");
            name.replace(":PIV", "");
            QString outFile = mOutputFolder + name;
            qDebug() << outFile;

            Ogre::Vector3 pos = node->_getDerivedPosition();
            mFout << QString("file_list[\"%1\"] = {x=%2, y=%3, z=%4}\n").arg(name).arg(0).arg(0).arg(0);

            //saveOneMesh(mesh.get(), outFile);
            saveOneObj(mesh.get(), outFile.replace(".mesh", ".obj"), Ogre::Vector3(pos.x, pos.y, pos.z));
        }
    }

    for (int i = 0; i < node->numChildren(); ++i)
    {
        Ogre::Node* child = node->getChild(i);
        Ogre::SceneNode* childSceneNode = dynamic_cast<Ogre::SceneNode*>(child);
        if (childSceneNode)
        {
            saveOneNode(childSceneNode);
        }
    }
}

bool SimpleMeshExporter::saveOneMesh(Ogre::Mesh* mesh, QString path)
{
    Ogre::Root& root = Ogre::Root::getSingleton();
    Ogre::RenderSystem* renderSystem = root.getRenderSystem();
    Ogre::VaoManager* vaoManager = renderSystem->getVaoManager();

    bool ok = false;

    try
    {
        Ogre::MeshSerializer meshSerializer2(vaoManager);
        meshSerializer2.exportMesh(mesh, path.toStdString());
        ok = true;
    }
    catch (...)
    {
        ok = false;
    }
    //return (ok && QFile::exists(outFile));
    return ok;
}

bool SimpleMeshExporter::saveOneObj(Ogre::Mesh* mesh, QString path, const Ogre::Vector3& positionOffset)
{
    const QString& sObjFileName = path;

    ObjExporter objExporter;
    bool ok = objExporter.writeToFile(mesh, sObjFileName, positionOffset);
    qDebug() << "Export Obj:" << sObjFileName << ok;

    return ok;
}
