#ifndef SCENEQUICKITEM_H
#define SCENEQUICKITEM_H

#include <QQuickFramebufferObject>

class SceneQuickItem : public QQuickFramebufferObject
{
    Q_OBJECT
    Q_PROPERTY(QColor background READ background WRITE setBackground NOTIFY backgroundChanged)
public:
    explicit SceneQuickItem(QQuickItem *parent = nullptr);
    ~SceneQuickItem();

    QColor background();
    void setBackground(QColor color);

protected:
    virtual Renderer *createRenderer() const override;

signals:
    void backgroundChanged();

private:
    QColor m_background;
};

#endif // !SCENEQUICKITEM_H