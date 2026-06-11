#ifndef PATHUTILS_H
#define PATHUTILS_H

#include <QString>
#include <QUrl>

#include <string>

namespace PathUtils {

QString projectRoot();
QString dataPath(const QString &relativePath);
QString resourcePath(const QString &relativePath);
QUrl audioUrl(const QString &relativePath);

std::string nativeDataPath(const QString &relativePath);

}

#endif // PATHUTILS_H
