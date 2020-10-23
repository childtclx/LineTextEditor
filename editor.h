#include <stdlib.h>
#include <math.h>

#define MAXSTRLEN 255
//#define MAXSTRLEN 5

typedef struct CharNode *PtrToChar;
typedef struct StringNode *PtrToString;
typedef struct LineNode *PtrToLine;
typedef struct TextNode *PtrToText;
typedef struct CursorNode *PtrToCursor;
typedef PtrToChar Char;
typedef PtrToString String;
typedef PtrToLine Line;
typedef PtrToText Text;
typedef PtrToCursor Cursor;



/**
 * 定义 char 的指针数据类型
 */
struct CharNode {
    char c;
    PtrToChar next;
};

/**
 * 定义 String 数据类型：带头节点的 char 链表
 */
struct StringNode {
    char command[3];
    int n;
    int m;
    int textstringLength;
    PtrToChar textstring;
};

/**
 * 定义文本每行结构体
 */
//struct Line {
//    int charCnt;
//    PtrToChar tail;
//    PtrToChar next;
//};
struct LineNode {
    int length;
    char line[MAXSTRLEN + 1];
    PtrToLine next;
};

/**
 * 定义文本结构体
 */
struct TextNode {
    int lineCnt;
    PtrToLine tail;  // 感觉有点多余
    PtrToLine next;
};


/**
 * 定义指针结构体
 */
struct CursorNode {
    int n;
    int m;
//    PtrToLine curLine;
};

String initString() {
    String string = (String)malloc(sizeof(struct StringNode));
    string->textstring = (Char)malloc(sizeof(struct CharNode));
    string->textstring->next = NULL;
    string->textstringLength = 0;
    string->command[0] = '\0';
    string->n = 0;
    string->m = 1;
    return string;
}

Text initText() {
    Text text = (Text)malloc(sizeof(struct TextNode));
    text->lineCnt = 0;
    text->tail = NULL;
    text->next = NULL;
    return text;
}

Cursor initCursor() {
    Cursor cursor = (Cursor)malloc(sizeof(struct CursorNode));
    cursor->n = 1;
    cursor->m = 1;
//    cursor->curLine = NULL;
    return cursor;
}

Line createLine(String string) {
    Line line = (Line)malloc(sizeof(struct LineNode));
    Char traverseChar = string->textstring->next;
    line->length = 0;
    while (traverseChar && line->length < MAXSTRLEN) {  // line->length < 255 输入字符串大于 255
        line->line[line->length] = traverseChar->c;
        line->length++;
        traverseChar = traverseChar->next;
    }
    line->line[line->length] = '\0';
    return line;
}

Line findPreLine(Text text, int n) {
    Line targetLine = text->next;
    n--;
    while (--n) {
        targetLine = targetLine->next;
    }
    return targetLine;
}

Line findCurLine(Text text, int n) {
    // 结构体的问题，导致 1 的时候不能找到前一个
    if (n == 1) {
        return text->next;
    }
    return findPreLine(text, n)->next;
}

void readString(String string);
void printStringCursor(String string, Cursor cursor);
void emptyString(String string);
void addLine(Text text, String string, Cursor cursor);
void insertLine(Text text, String string, Cursor cursor);
void deleteCurrentLine(Text text, Cursor cursor);
void deletetSetLine(Text text, String string, Cursor cursor);
void deleteLine(Text text, int n, Cursor cursor);
void insertString(Text text, String string, Cursor cursor);
void deleteChar(Text text, Cursor cursor);
void moveCursor(Text text, Cursor cursor, String string);
void printText(Text text);
int max(int a, int b) ;


void readString(String string) {
    char inputChar;
    PtrToChar tempChar, tailChar;
    // 读取命令
    if ((inputChar = getchar()) == '$') {
        string->command[0] = getchar();
        string->command[1] = getchar();
        string->command[2] = '\0';
    } else {
        while (getchar() != '\n');
        return;
    }
    if (getchar() == '\n') {
        return;
    }
    if (!strcmp(string->command, "DL")) {
        scanf("%d", &string->n);
        // 吃 scanf 的回车触发……
        getchar();
    } else if (!strcmp(string->command, "MC")) {
        scanf("%d %d", &string->n, &string->m);
        getchar();
    } else {
        if (!strcmp(string->command, "IL")) {
            scanf("%d", &string->n);
        }
        // 读取字符串
        while ((inputChar = getchar()) != '\n') {
            // 消除字符串之间的空格
            if (string->textstring->next == NULL && inputChar == ' ') {
                continue;
            }
            string->textstringLength++;
            if (string->textstring->next == NULL) {
                tailChar = string->textstring;
            }
            tempChar = (Char)malloc(sizeof(struct CharNode));
            tempChar->c = inputChar;
            tempChar->next = NULL;
            tailChar->next = tempChar;
            tailChar = tempChar;
        }
    }
}

void printStringCursor(String string, Cursor cursor) {
    printf("cmd: %s\n", string->command);
    printf("cursor->n: %d, cursor->m: %d\n", cursor->n, cursor->m);
    printf("textstring: ");
    PtrToChar traverseChar = string->textstring->next;
    while (traverseChar != NULL) {
        printf("%c", traverseChar->c);
        traverseChar = traverseChar->next;
    }
    printf("\n");
    printf("textstring length: %d\n", string->textstringLength);
}

void testPrintText(Text text) {
    printText(text);
    printf("text lineCnt: %d\n", text->lineCnt);
}

void emptyString(String string) {
    // 清除命令数组
    string->command[0] = '\0';
    string->n = 0;
    string->m = 1;
    // 清除字符串缓存
    PtrToChar curChar = string->textstring->next;
    if (curChar == NULL) {
        return;
    }
    PtrToChar postChar = curChar->next;
    while (postChar != NULL) {
        free(curChar);
        curChar = postChar;
        postChar = postChar->next;
    }
    free(curChar);
    string->textstring->next = NULL;
    string->textstringLength = 0;
}

/**
 * $AL textstring
 * @param text
 * @param string
 * @param cursor
 */
void addLine(Text text, String string, Cursor cursor) {
    Line addLine = createLine(string);
    addLine->next = NULL;
    if (text->tail == NULL) {
        text->next = addLine;
    } else {
        text->tail->next = addLine;
    }
    text->tail = addLine;
    text->lineCnt++;
    cursor->n = text->lineCnt;
    cursor->m = 1;
}

/**
 * $IL n textstring
 * @param text
 * @param string
 * @param cursor
 */
void insertLine(Text text, String string, Cursor cursor) {
    if (string->n > text->lineCnt) {
        addLine(text, string, cursor);
    } else {
        Line addLine = createLine(string);
        if (string->n == 1) {
            addLine->next = text->next;
            text->next = addLine;
        } else {
            // 之前插入，需要定位到第 n 行的前一行
            Line targetLine = findPreLine(text, string->n);
            addLine->next = targetLine->next;
            targetLine->next = addLine;
        }
        text->lineCnt++;
        cursor->n = string->n;
        cursor->m = 1;
    }
}


void deleteLine(Text text, int n, Cursor cursor) {
    Line tempLine;
    if (n == 1) {
        tempLine = text->next;
        text->next = tempLine->next;
        if (text->next == NULL) {
            text->tail = NULL;
        }
        free(tempLine);
    } else {
        Line targetLine = findPreLine(text, n);
        tempLine = targetLine->next;
        targetLine->next = tempLine->next;
        free(tempLine);
        if (n == text->lineCnt) {
            cursor->m = 1;
        }
    }
    text->lineCnt--;
}

/**
 * $DD
 * @param text
 * @param cursor
 */
void deleteCurrentLine(Text text, Cursor cursor) {
    deleteLine(text, cursor->n, cursor);
    // 删除最后一行，光标上移
    if (text->lineCnt < cursor->n) {
        cursor->n--;
    }
    cursor->m = 1;
}

/**
 * $DL n
 * @param text
 * @param string
 * @param cursor
 */
void deletetSetLine(Text text, String string, Cursor cursor) {
    if (string->n > text->lineCnt) {
        printf("Invalid input!\n");
    } else {
        deleteLine(text, string->n, cursor);
        if (string->n <= cursor->n) {
            cursor->n--;
        }
        cursor->m = 1;
    }
}

/**
 * $IS textstring
 * @param text
 * @param string
 * @param cursor
 */
void insertString(Text text, String string, Cursor cursor) {
    Char traverseLine = string->textstring->next;
    Line curLine = findCurLine(text, cursor->n);
    int preLength = cursor->m;  // 在光标后一位插入，包括当前光标元素
    int postLength = curLine->length - preLength;
    if (string->textstringLength >= MAXSTRLEN - preLength) {  // textstring 长度超过光标到 255 的长度
        for (int i = 0; i < postLength; i++) {
            curLine->line[preLength + i] = traverseLine->c;
            traverseLine = traverseLine->next;
        }
        curLine->line[MAXSTRLEN] = '\0';
        curLine->length = MAXSTRLEN;
        cursor->m = MAXSTRLEN;
    } else {
        if (string->textstringLength > MAXSTRLEN - curLine->length) {  // 原光标后字符由于插入超过 255 字符
            for (int i = 0; i < max(string->textstringLength, postLength); i++) {
                // 注意先后
                if (i < postLength) {
                    curLine->line[preLength + i + string->textstringLength] = curLine->line[preLength + i];
                }
                if (i < string->textstringLength) {
                    curLine->line[preLength + i] = traverseLine->c;
                    traverseLine = traverseLine->next;
                }
            }
            curLine->line[MAXSTRLEN] = '\0';
            curLine->length = MAXSTRLEN;
        } else {
            for (int i = 0; i < string->textstringLength; i++) {
                curLine->line[preLength + i + string->textstringLength] = curLine->line[preLength + i];
                curLine->line[preLength + i] = traverseLine->c;
                traverseLine = traverseLine->next;
            }
            curLine->length = preLength + string->textstringLength + postLength;
            curLine->line[curLine->length] = '\0';
        }
        cursor->m += string->textstringLength;
    }


}

/**
 * $DC
 * @param text
 * @param cursor
 */
void deleteChar(Text text, Cursor cursor) {
    Line curLine = findCurLine(text, cursor->n);
    if (curLine->line[cursor->m - 1] == '\0') {  // \0 是字符
        if (cursor->m == 1) {
            deleteCurrentLine(text, cursor);
        } else {
            cursor->m--;
            curLine->line[cursor->m - 1] = '\0';
            curLine->length--;
        }
    } else {
        for (int i = cursor->m - 1; i < curLine->length + 1; i++) {
            curLine->line[i] = curLine->line[i + 1];
        }
        curLine->length--;
    }
}

/**
 * $MC n m
 * @param cursor
 * @param string
 */
void moveCursor(Text text, Cursor cursor, String string) {
    if (string->n > text->lineCnt) {
        cursor->n = text->lineCnt;
    } else {
        cursor->n = string->n;
        int l = findCurLine(text, string->n)->length;
        if (l == 0) {  // 空行移至列为 1
            cursor->m = 1;
        } else if (string->m > l) {
            cursor->m = l;
        } else {
            cursor->m = string->m;
        }
    }
}

/**
 * $PT
 * @param text
 */
void printText(Text text) {
    Line traverseLine = text->next;
    while (traverseLine) {
        for (int i = 0; (i < MAXSTRLEN) && (traverseLine->line[i] != '\0'); ++i) {
            printf("%c", traverseLine->line[i]);
        }
        printf("\n");
        traverseLine = traverseLine->next;
    }
}

int max(int a, int b) {
    return a > b ? a : b;
}