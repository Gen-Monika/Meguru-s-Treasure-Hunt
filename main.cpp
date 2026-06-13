#include "widget.h"
#include "PathUtils.h"

#include <QApplication>
#include <QIcon>

namespace {
const QString application_title = "Meguru's Treasure Hunt";
const QString application_icon_path = "../../Resources/Pictures/Icon-s/AppIcon/MeguruTreasureHunt.ico";
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setApplicationDisplayName(application_title);

    const QIcon application_icon(PathUtils::resourcePath(application_icon_path));
    QApplication::setWindowIcon(application_icon);

    Widget w;
    w.setWindowTitle(application_title);
    w.setWindowIcon(application_icon);
    w.show();
    return a.exec();
}
