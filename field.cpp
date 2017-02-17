#include "field.h"
#include <QDebug>

Field::Field(QWidget* parent) : QGLWidget(parent) {
  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(update()));
  timer->setInterval(10);
}

Field::~Field() {}

void Field::start(int count) {
  this->initItems(count);
}

void Field::update() {
  qDebug() << "TICK";
  CombinationItem* currentCandidat = nullptr;
  for (int i = 0; i < itemsSelector.size(); i++) {
    if (itemsSelector.at(i)->link == -1) {
      currentCandidat = itemsSelector.at(i);
      break;
    }
    if (i == itemsSelector.size() - 1) {
      timer->stop();
      qDebug() << "STOP";
      return;
    }
  }
  if (currentCandidat->nowSelectPosition == -1)
    currentCandidat->nowSelectPosition = itemsSelector.size() - 1;
  while (true) {
    int w = currentCandidat->weight[currentCandidat->nowSelectPosition];
    if (itemsChoser.at(w)->link == -1) {
      itemsChoser.at(w)->link = currentCandidat->name;
      currentCandidat->link = itemsChoser.at(w)->name;
      for (int i = 0; i < itemsSelector.size(); i++) {
        if (itemsChoser.at(w)->weight[i] == currentCandidat->name) {
          itemsChoser.at(w)->nowSelectPosition = i;
          break;
        }
      }
      break;
    } else {
      if (itemsChoser.at(w)->weight[currentCandidat->name] >
          itemsChoser.at(w)->weight[itemsChoser.at(w)->link]) {
        itemsSelector.at(itemsChoser.at(w)->link)->link = -1;
        itemsSelector.at(itemsChoser.at(w)->link)->nowSelectPosition--;

        itemsChoser.at(w)->link = currentCandidat->name;
        currentCandidat->link = itemsChoser.at(w)->name;
        for (int i = 0; i < itemsSelector.size(); i++) {
          if (itemsChoser.at(w)->weight[i] == currentCandidat->name) {
            itemsChoser.at(w)->nowSelectPosition = i;
            break;
          }
        }
        break;
      } else {
        currentCandidat->nowSelectPosition--;
      }
    }
  }
  repaint();
}

void Field::initializeGL() {}

void Field::resizeGL(int w, int h) {}

void Field::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  float step = 1.f / ((float)itemsChoser.size()) * 2;

  glLineWidth(3);
  for (float i = 0; i < itemsSelector.size(); i++) {
    if (itemsSelector.at(i)->link == -1) {
      continue;
    }
    glColor3f(0.6117647059, 0.337254902, 0.768627451);
    glBegin(GL_LINES);
    glVertex2f(step * i + step * 0.5 - 1.f, startLine);
    glVertex2f(step * ((float)itemsSelector.at(i)->link) + step * 0.5f - 1.f,
               -startLine);
    glEnd();
  }
  float colorK;
  for (float i = 0; i < itemsSelector.size(); i++) {
    colorK = (((float)itemsChoser.at(i)->nowSelectPosition) /
              ((float)itemsChoser.size())) *
             0.5f;
    glColor3f(0.25 + colorK, 0.25 + colorK, 0.25 + colorK);
    glBegin(GL_QUADS);
    glVertex2f(step * i - 1.f + step * offsetPercent, startLine);
    glVertex2f(step * i - 1.f + step * offsetPercent, endLine);
    glVertex2f(step * (i + 1) - 1.f - step * offsetPercent, endLine);
    glVertex2f(step * (i + 1) - 1.f - step * offsetPercent, startLine);
    glEnd();

    colorK = (((float)itemsChoser.at(i)->nowSelectPosition) /
              ((float)itemsChoser.size())) *
             0.5f;
    glColor3f(0.25 + colorK, 0.25 + colorK, 0.25 + colorK);
    glBegin(GL_QUADS);
    glVertex2f(step * i - 1.f + step * offsetPercent, -startLine);
    glVertex2f(step * i - 1.f + step * offsetPercent, -endLine);
    glVertex2f(step * (i + 1) - 1.f - step * offsetPercent, -endLine);
    glVertex2f(step * (i + 1) - 1.f - step * offsetPercent, -startLine);
    glEnd();
  }
}

void Field::initItems(int count) {
  itemsSelector.clear();
  itemsChoser.clear();

  for (int i = 0; i < count; i++) {
    itemsChoser.append(new CombinationItem(i, count));
    itemsSelector.append(new CombinationItem(i, count));
  }
  timer->start();
}

CombinationItem::CombinationItem(int position, int maxCount) {
  this->name = position;
  this->weight = new int[maxCount];
  for (int i = 0; i < maxCount; i++) {
    int w = rand() % maxCount;
    for (int j = 0; j < i; j++) {
      if (w == this->weight[j]) {
        w = rand() % maxCount;
        j = -1;
      }
    }
    this->weight[i] = w;
  }
  //  float buf;
  //  for (int i = 0; i < maxCount - 1; i++) {
  //    if (this->weight[i] > this->weight[i + 1]) {
  //      buf = this->weight[i];
  //      this->weight[i] = this->weight[i + 1];
  //      this->weight[i + 1] = buf;
  //      i -= 2;
  //      if (i < -1)
  //        i = -1;
  //    }
  //  }
  this->link = -1;
  nowSelectPosition = -1;
}
