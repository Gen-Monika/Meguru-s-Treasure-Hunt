#include "PathUtils.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>

namespace {

bool hasResourcesDir(const QString &path)
{
    return QFileInfo::exists(QDir(path).filePath("Resources"));
}

QString findProjectRootFrom(const QString &startPath)
{
    QDir dir(startPath);
    if (!dir.exists()) {
        return {};
    }

    const QString appName = QCoreApplication::applicationName();
    for (int depth = 0; depth < 8; ++depth) {
        if (hasResourcesDir(dir.absolutePath())) {
            return dir.absolutePath();
        }

        if (!appName.isEmpty()) {
            const QString siblingProject = QDir(dir.absolutePath()).filePath(appName);
            if (hasResourcesDir(siblingProject)) {
                return QDir(siblingProject).absolutePath();
            }
        }

        if (!dir.cdUp()) {
            break;
        }
    }

    return {};
}

QString stripLegacyRelativePrefix(QString path)
{
    path = QDir::fromNativeSeparators(path);

    while (path.startsWith("./")) {
        path.remove(0, 2);
    }

    while (path.startsWith("../")) {
        path.remove(0, 3);
    }

    return path;
}

std::string toNativeString(const QString &path)
{
    const QByteArray bytes = QDir::toNativeSeparators(path).toLocal8Bit();
    return std::string(bytes.constData(), bytes.size());
}

}

namespace PathUtils {

QString projectRoot()
{
    static const QString root = [] {
        const QString fromApp = findProjectRootFrom(QCoreApplication::applicationDirPath());
        if (!fromApp.isEmpty()) {
            return fromApp;
        }

        const QString fromCurrent = findProjectRootFrom(QDir::currentPath());
        if (!fromCurrent.isEmpty()) {
            return fromCurrent;
        }

        return QDir::currentPath();
    }();

    return root;
}

QString dataPath(const QString &relativePath)
{
    if (relativePath.isEmpty() || QFileInfo(relativePath).isAbsolute()) {
        return QDir::cleanPath(relativePath);
    }

    return QDir(projectRoot()).filePath(stripLegacyRelativePrefix(relativePath));
}

QString resourcePath(const QString &relativePath)
{
    if (relativePath.isEmpty() || QFileInfo(relativePath).isAbsolute()) {
        return QDir::cleanPath(relativePath);
    }

    return QDir(projectRoot()).filePath(stripLegacyRelativePrefix(relativePath));
}

QUrl audioUrl(const QString &relativePath)
{
    if (relativePath.isEmpty()) {
        return {};
    }

    return QUrl::fromLocalFile(resourcePath(relativePath));
}

std::string nativeDataPath(const QString &relativePath)
{
    return toNativeString(dataPath(relativePath));
}

}
