#ifndef DIALOGUERENDERER_H
#define DIALOGUERENDERER_H

#include <QPixmap>
#include <QString>

#include "config.h"

class DialogueRenderer
{
public:
    static QPixmap makeDialoguePixmap(const Default::DialogueLineConfig& line);
    static QPixmap makeDialoguePixmap(const QString& speaker,const QString& text);
};

#endif // DIALOGUERENDERER_H
