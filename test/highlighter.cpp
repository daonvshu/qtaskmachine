#include "highlighter.h"

CppHighlighter::CppHighlighter(QTextDocument *parent) : QSyntaxHighlighter(parent) {

    Rule rule;

    const auto keywords = "alignas alignof and and_eq asm atomic_cancel atomic_commit atomic_noexcept auto "
                          "bitand bitor break case catch class compl concept const constexpr const_cast "
                          "continue co_await co_return co_yield decltype default delete do dynamic_cast "
                          "else enum explicit export extern false for friend goto if import inline module "
                          "mutable namespace new noexcept not not_eq nullptr operator or or_eq private "
                          "protected public reflexpr register(2) reinterpret_cast requires return sizeof "
                          "static static_assert static_cast struct switch synchronized template this "
                          "thread_local throw true try typedef typeid typename union using virtual volatile "
                          "while xor xor_eq bool char char16_t char32_t double float int long short signed "
                          "unsigned void wchar_t";

    //keywords
    auto keywordsList = QString(keywords).split(" ");
    rule.format.setForeground(QColor(0x0033B3));
    for (const auto& k : keywordsList) {
        rule.pattern = QRegularExpression(QString(R"(\b%1\b)").arg(k));
        rules << rule;
    }

    //include
    rule.format.setForeground(QColor(0x9E880D));
    rule.pattern = QRegularExpression(R"(^\s*#\s*(include)\s*([<"][^:?"<>\|]+[">]))");
    rules << rule;

    //function
    rule.format.setForeground(Qt::black);
    rule.pattern = QRegularExpression(R"(\b([_a-zA-Z][_a-zA-Z0-9]*\s+)?((?:[_a-zA-Z][_a-zA-Z0-9]*\s*::\s*)*[_a-zA-Z][_a-zA-Z0-9]*)(?=\s*\())");
    rules << rule;

    //class
    rule.format.setForeground(QColor(0x008080));
    rule.pattern = QRegularExpression(R"(\bQ[A-Za-z]+\b)");
    rules << rule;

    //string
    rule.format.setForeground(QColor(0x6AAB73));
    rule.pattern = QRegularExpression(R"("[^\n"]*")");
    rules << rule;

    //preprocessor
    rule.format.setForeground(QColor(0x9E880D));
    rule.pattern = QRegularExpression(R"(#[a-zA-Z_]+)");
    rules << rule;

    //q function
    rule.format.setForeground(QColor(0x9E880D));
    rule.pattern = QRegularExpression(R"(q[a-zA-Z]+)");
    rules << rule;

    //number
    rule.format.setForeground(QColor(0x2AACB8));
    rule.pattern = QRegularExpression(R"((?<=\b|\s|^)(?i)(?:(?:(?:(?:(?:\d+(?:'\d+)*)?\.(?:\d+(?:'\d+)*)(?:e[+-]?(?:\d+(?:'\d+)*))?)|(?:(?:\d+(?:'\d+)*)\.(?:e[+-]?(?:\d+(?:'\d+)*))?)|(?:(?:\d+(?:'\d+)*)(?:e[+-]?(?:\d+(?:'\d+)*)))|(?:0x(?:[0-9a-f]+(?:'[0-9a-f]+)*)?\.(?:[0-9a-f]+(?:'[0-9a-f]+)*)(?:p[+-]?(?:\d+(?:'\d+)*)))|(?:0x(?:[0-9a-f]+(?:'[0-9a-f]+)*)\.?(?:p[+-]?(?:\d+(?:'\d+)*))))[lf]?)|(?:(?:(?:[1-9]\d*(?:'\d+)*)|(?:0[0-7]*(?:'[0-7]+)*)|(?:0x[0-9a-f]+(?:'[0-9a-f]+)*)|(?:0b[01]+(?:'[01]+)*))(?:u?l{0,2}|l{0,2}u?)))(?=\b|\s|$))");
    rules << rule;

    //comment
    rule.format.setForeground(QColor(0x8C8C8C));
    rule.pattern = QRegularExpression("//[^\n]*");
    rules << rule;

    commentStart = QRegularExpression(R"(/\*)");
    commentEnd = QRegularExpression(R"(\*/)");
}

void CppHighlighter::highlightBlock(const QString &text) {
    for (const auto &rule : rules) {
        auto matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            auto match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStart);

    while (startIndex >= 0) {
        auto match = commentEnd.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex + match.capturedLength();
        }
        setFormat(startIndex, commentLength, QColor(0x8C8C8C));
        startIndex = text.indexOf(commentStart, startIndex + commentLength);
    }
}
