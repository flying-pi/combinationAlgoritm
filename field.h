#ifndef FIELD_H
#define FIELD_H

#include <QGLWidget>
#include <QList>
#include <QTimer>

struct CombinationItem {
  CombinationItem(int position, int maxCount);
  int name;
  int link;
  int* weight;
  int nowSelectPosition;
};

class Field : public QGLWidget {
  Q_OBJECT

 public:
  Field(QWidget* parent = 0);
  ~Field();
  void start(int count);

 private slots:
  void update();

 protected:
  void initializeGL() Q_DECL_OVERRIDE;
  void resizeGL(int w, int h) Q_DECL_OVERRIDE;
  void paintGL() Q_DECL_OVERRIDE;

  void initItems(int count);

  QList<CombinationItem*> itemsSelector;
  QList<CombinationItem*> itemsChoser;

  QTimer* timer;

  float startLine = 0.55f;
  float endLine = 1.f;
  float offsetPercent = 0.15f;
};

#endif  // FIELD_H
