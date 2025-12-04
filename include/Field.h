#ifndef FIELD_H
#define FIELD_H

#include "Figure.h"
#include <vector>

class Field {
protected:
    std::vector<bool> fieldmatrix;
    std::vector<std::string> fieldcolors;
    int fieldWidth = 22;
    int fieldHeight = 22;

public:
    Field();
    virtual ~Field() = default;

    virtual void placeFigure(Figure& figure);
    bool getch(int i, int j);
     void setch(int i, int j, bool value, const std::string& color = "");
    std::string getColor(int i, int j);
    virtual int getHeight();
    virtual int getWidth();
    virtual int getRowWidth(int row) { return fieldWidth; }
    virtual bool isValidPosition(int row, int col);
    virtual int clearFullLines();
};
class BucketField : public Field {
private:
    std::vector<int> rowWidths;

public:
    BucketField();
    virtual void placeFigure(Figure& figure) override;
    virtual int getRowWidth(int row) override;
    virtual int getWidth() override { return fieldWidth; }
    virtual int clearFullLines();
    virtual bool isValidPosition(int row, int col);
    
    
private:
    void initializeBucketShape();
};

#endif