#ifndef ZDIFFAREA
#define ZDIFFAREA

class ZDiffArea
{
public:
    ZDiffArea();
    ~ZDiffArea();

    float startX() const;
    void setStartX(float startX);

    float startY() const;
    void setStartY(float startY);

    float endX() const;
    void setEndX(float endX);

    float endY() const;
    void setEndY(float endY);

private:
    float mStartX;
    float mStartY;
    float mEndX;
    float mEndY;
};

#endif // ZDIFFAREA

