#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QIcon>
#include <QPixmap>
#include <QSize>
#include <QString>
#include <QUrl>

namespace ResourceManager {

QPixmap *loadPixmap(const QString &relativePath, const QSize &scaledSize = QSize());
QIcon *loadIcon(const QString &relativePath);
QUrl audioUrl(const QString &relativePath);
QUrl *loadAudioUrl(const QString &relativePath);

}

#endif // RESOURCEMANAGER_H
