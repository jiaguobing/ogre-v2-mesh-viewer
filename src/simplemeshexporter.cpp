#include "simplemeshexporter.h"

#include <QFile>
#include <QTextStream>
#include "OgreMesh2Serializer.h"
#include "OgreMesh2.h"
#include "objexporter.h"
#include "OgreHlmsJsonPbs.h"
#include "OgreSubMesh2.h"


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

            qDebug() << "Num Subitems=" << item->getNumSubItems();
            for (int i = 0; i < item->getNumSubItems(); ++i)
            {
                Ogre::SubItem* sub = item->getSubItem(i);
                Ogre::HlmsDatablock* block = sub->getDatablock();
                mesh->getSubMesh(i)->setMaterialName(*block->getNameStr());

                saveHlmsJson(block, mOutputFolder + QString::fromStdString(*block->getNameStr()));
            }

            saveOneMesh(mesh.get(), outFile);
            //saveOneObj(mesh.get(), outFile.replace(".mesh", ".obj"), Ogre::Vector3(pos.x, pos.y, pos.z));
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
        for (Ogre::SubMesh* sub : mesh->getSubMeshes())
        {
            qDebug() << "sub-material=" << sub->getMaterialName().c_str();
        }

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

bool SimpleMeshExporter::saveHlmsJson(Ogre::HlmsDatablock* datablock, QString path)
{
    auto hlmsManager = Ogre::Root::getSingleton().getHlmsManager();
    Ogre::HlmsJson hlmsJson(hlmsManager, nullptr);
    std::string outJson;
    hlmsJson.saveMaterial(datablock, outJson, "");

    if (!path.endsWith(".material.json"))
    {
        path += ".material.json";
    }

    std::ofstream fout(path.toStdString(), std::ofstream::out);
    fout << outJson;
    fout.close();
    return true;
}
