#ifndef SIMPLEMESHEXPORTER_H
#define SIMPLEMESHEXPORTER_H

#include <QString>

namespace Ogre
{
class SceneNode;
class Mesh;
}


class SimpleMeshExporter
{
public:
    SimpleMeshExporter();

    void setOutputFolder(QString s);
    void save(Ogre::SceneNode* rootNode);

private:
    void saveOneNode(Ogre::SceneNode* node);
    bool saveOneMesh(Ogre::Mesh* mesh, QString path);

    QString mOutputFolder;
    QTextStream mFout;
};

#endif // SIMPLEMESHEXPORTER_H