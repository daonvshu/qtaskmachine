#pragma once

#include <qsyntaxhighlighter.h>
#include <qregularexpression.h>

class CppHighlighter : public QSyntaxHighlighter {
public:
    explicit CppHighlighter(QTextDocument *parent);

private:
    struct Rule {
        QTextCharFormat format;
        QRegularExpression pattern;
    };
    QList<Rule> rules;

    QRegularExpression commentStart, commentEnd;

    void highlightBlock(const QString &text) override;
};
