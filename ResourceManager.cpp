#include "ResourceManager.h"
#include "PathUtils.h"

#include <QDebug>
#include <QFileInfo>

namespace {

QString resolveResourcePath(const QString &relativePath)
{
    const QString path = PathUtils::resourcePath(relativePath);
    if (!relativePath.isEmpty() && !QFileInfo::exists(path)) {
        qWarning() << "Missing resource:" << path;
    }
    return path;
}

void warnIfPixmapInvalid(const QString &relativePath, const QPixmap *pixmap)
{
    if (!relativePath.isEmpty() && pixmap != nullptr && pixmap->isNull()) {
        qWarning() << "Failed to load pixmap:" << relativePath;
    }
}

void warnIfIconInvalid(const QString &relativePath, const QIcon *icon)
{
    if (!relativePath.isEmpty() && icon != nullptr && icon->isNull()) {
        qWarning() << "Failed to load icon:" << relativePath;
    }
}

}

namespace ResourceManager {

QPixmap *loadPixmap(const QString &relativePath, const QSize &scaledSize)
{
    QPixmap *pixmap = new QPixmap(resolveResourcePath(relativePath));
    warnIfPixmapInvalid(relativePath, pixmap);

    if (!scaledSize.isEmpty() && pixmap != nullptr && !pixmap->isNull()) {
        *pixmap = pixmap->scaled(scaledSize, Qt::KeepAspectRatio);
    }

    return pixmap;
}

QIcon *loadIcon(const QString &relativePath)
{
    QIcon *icon = new QIcon(resolveResourcePath(relativePath));
    warnIfIconInvalid(relativePath, icon);
    return icon;
}

QUrl audioUrl(const QString &relativePath)
{
    if (relativePath.isEmpty()) {
        return {};
    }

    return QUrl::fromLocalFile(resolveResourcePath(relativePath));
}

QUrl *loadAudioUrl(const QString &relativePath)
{
    return new QUrl(audioUrl(relativePath));
}

}
