#include "DialogueRenderer.h"

#include "PathUtils.h"

#include <QColor>
#include <QDebug>
#include <QFont>
#include <QFontDatabase>
#include <QFontMetricsF>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QStringList>
#include <QVector>

namespace {

const QColor text_fill_color(245,238,224);
const QColor text_outline_color(55,42,35,210);

QString dialogueFontFamily()
{
    static const QString family = []() {
        const int fontId = QFontDatabase::addApplicationFont(
            PathUtils::resourcePath(Default::dialogue_font_path));
        if(fontId < 0){
            qWarning() << "Failed to load dialogue font:" << Default::dialogue_font_path;
            return QString();
        }
        const QStringList families = QFontDatabase::applicationFontFamilies(fontId);
        return families.isEmpty() ? QString() : families.first();
    }();
    return family;
}

QFont dialogueFont(int pixelSize)
{
    QFont font(dialogueFontFamily());
    font.setPixelSize(pixelSize);
    font.setHintingPreference(QFont::PreferNoHinting);
    return font;
}

void drawOutlinedText(QPainter& painter,const QFont& font,
                      const QPointF& baseline,const QString& text)
{
    QPainterPath path;
    path.addText(baseline,font,text);

    QPen outline(text_outline_color);
    outline.setWidthF(Default::dialogue_outline_width);
    outline.setJoinStyle(Qt::RoundJoin);

    painter.setPen(outline);
    painter.setBrush(text_fill_color);
    painter.drawPath(path);
}

double contentWidthForLine(int lineIndex,const QFontMetricsF& metrics)
{
    const double left = lineIndex == 0
        ? Default::dialogue_quote_left + metrics.horizontalAdvance(QStringLiteral("「 "))
        : Default::dialogue_content_left;
    return Default::dialogue_right_edge - left;
}

void ensureClosingQuoteFits(QVector<QString>& lines,const QFontMetricsF& metrics)
{
    if(lines.isEmpty()){
        lines.append(QString());
    }

    while(true){
        const int lastIndex = lines.size() - 1;
        const double width = metrics.horizontalAdvance(lines[lastIndex] + QStringLiteral(" 」"));
        if(width <= contentWidthForLine(lastIndex,metrics) || lines[lastIndex].isEmpty()){
            return;
        }

        QString last = lines.takeLast();
        QString carry;
        const double availableWidth = contentWidthForLine(lines.size(),metrics);
        while(!last.isEmpty()
               && metrics.horizontalAdvance(last + QStringLiteral(" 」")) > availableWidth){
            carry.prepend(last.at(last.size() - 1));
            last.chop(1);
        }
        if(!last.isEmpty()){
            lines.append(last);
        }
        lines.append(carry);
    }
}

QVector<QString> wrapBodyText(const QString& text,const QFontMetricsF& metrics)
{
    QVector<QString> lines;
    QString currentLine;
    QString normalizedText = text;
    normalizedText.replace(QStringLiteral("\r\n"),QStringLiteral("\n"));
    normalizedText.replace(QChar('\r'),QChar('\n'));

    const QStringList paragraphs = normalizedText.split(QChar('\n'));
    for(int paragraphIndex = 0; paragraphIndex < paragraphs.size(); ++paragraphIndex){
        if(paragraphIndex > 0){
            lines.append(currentLine);
            currentLine.clear();
        }

        const QString& paragraph = paragraphs[paragraphIndex];
        for(const QChar& character : paragraph){
            const QString candidate = currentLine + character;
            const int lineIndex = lines.size();
            if(!currentLine.isEmpty()
                && metrics.horizontalAdvance(candidate) > contentWidthForLine(lineIndex,metrics)){
                lines.append(currentLine);
                currentLine = QString(character);
            }
            else{
                currentLine = candidate;
            }
        }
    }

    lines.append(currentLine);
    ensureClosingQuoteFits(lines,metrics);
    return lines;
}

} // namespace

QPixmap DialogueRenderer::makeDialoguePixmap(const Default::DialogueLineConfig& line)
{
    return makeDialoguePixmap(line.speaker,line.text);
}

QPixmap DialogueRenderer::makeDialoguePixmap(const QString& speaker,const QString& text)
{
    QPixmap pixmap(PathUtils::resourcePath(Default::dialogue_base_path));
    if(pixmap.isNull()){
        qWarning() << "Failed to load dialogue base:" << Default::dialogue_base_path;
        pixmap = QPixmap(Default::text_size);
        pixmap.fill(Qt::transparent);
    }
    else if(pixmap.size() != Default::text_size){
        pixmap = pixmap.scaled(Default::text_size,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    }

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setRenderHint(QPainter::TextAntialiasing,true);

    const QFont speakerFont = dialogueFont(Default::dialogue_speaker_font_px);
    const QFont bodyFont = dialogueFont(Default::dialogue_body_font_px);
    const QFontMetricsF speakerMetrics(speakerFont);
    const QFontMetricsF bodyMetrics(bodyFont);

    const QPointF speakerBaseline(
        Default::dialogue_quote_left,
        Default::dialogue_speaker_top + speakerMetrics.ascent());
    drawOutlinedText(
        painter,
        speakerFont,
        speakerBaseline,
        QStringLiteral("【 ") + speaker + QStringLiteral(" 】"));

    const QVector<QString> bodyLines = wrapBodyText(text,bodyMetrics);
    for(int i = 0; i < bodyLines.size(); ++i){
        QString line = bodyLines[i];
        if(i == 0){
            line.prepend(QStringLiteral("「 "));
        }
        if(i == bodyLines.size() - 1){
            line.append(QStringLiteral(" 」"));
        }

        const QPointF baseline(
            i == 0 ? Default::dialogue_quote_left : Default::dialogue_content_left,
            Default::dialogue_body_top + Default::dialogue_line_gap * i + bodyMetrics.ascent());
        drawOutlinedText(painter,bodyFont,baseline,line);
    }

    return pixmap;
}
