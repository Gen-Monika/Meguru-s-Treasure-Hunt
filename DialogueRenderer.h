#ifndef DIALOGUERENDERER_H
#define DIALOGUERENDERER_H

#include <QPoint>
#include <QPixmap>
#include <QSize>
#include <QString>

#include "config.h"

class DialogueRenderer
{
public:
    static QPixmap makeDialoguePixmap(const Default::DialogueLineConfig& line);
    static QPixmap makeDialoguePixmap(const QString& speaker,const QString& text);
    static QPixmap makeDialoguePixmap(const QString& speaker,
                                      const QString& text,
                                      int visibleCharacters);
    static int dialogueTextLength(const QString& text);
    static QPoint voiceReplayButtonPos(const QString& speaker,const QSize& buttonSize);
};

#endif // DIALOGUERENDERER_H
