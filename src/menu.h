#ifndef MENU_H
#define MENU_H

#include <QGraphicsItem>
#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>
#include <QGraphicsWidget>
#include <QLabel>
#include <QPushButton>

#include "core/eventhandling.h"
#include "graphics/imageitem.h"
#include "util/utility.h"

//class ImageItem;
class GameScene;
class MainWindow;

class MainMenu : public QObject {
  Q_OBJECT

 public:
  MainMenu();
  ~MainMenu();

  void Draw();

 signals:
  void btnSettingsClick();
  void btnNewGameClick();
  void btnExitClick();

 private:
  friend class EventHandler::View;

  ImageItem* txt_total_space_;
  ImageItem* btn_new_game_;
  ImageItem* btn_exit_;
};

class PauseMenu : public QObject {
  Q_OBJECT

 public:
  PauseMenu();
  ~PauseMenu();

  void Draw();

 signals:
  void btnBackClick();
  void btnExitClick();

 private:
  friend class EventHandler::View;

  ImageItem* btn_exit_;
  ImageItem* btn_back_;
  QGraphicsRectItem* background_rect_;
};

class PlanetMenu : public QObject {
  Q_OBJECT

 public:
  PlanetMenu();
  ~PlanetMenu();

  void Draw();
  void Hide();
  void Show();

 signals:
  void btn1Click();
  void btn2Click();
  void btn3Click();

 private:
  friend class EventHandler::View;

  ImageItem* btn1_;
  ImageItem* btn2_;
  ImageItem* btn3_;

  double radius_;
};

class ShopItem : public QGraphicsObject {
    Q_OBJECT
    Q_INTERFACES()

public:
  ShopItem();
  ShopItem(QPointF pos, QSizeF size, QString name);

  int type() const override;
  ShopType GetType() const;

  void Remove();

public slots:
  void BuySomething();

private:
  QRectF boundingRect() const override;
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
             QWidget* widget) override;

  QSizeF size_;
  QRectF picture_rect_;
  QPixmap picture_;

  QGraphicsProxyWidget* proxy_btn_ = nullptr;
  QPushButton* buy_btn_;
  QRectF btn_rect_;

  QGraphicsProxyWidget* proxy_name_label_ = nullptr;
  QLabel* name_label_;
  QRectF name_label_rect_;

  QGraphicsProxyWidget* proxy_price_label_ = nullptr;
  QLabel* price_label_;
  QRectF price_label_rect_;

  ShopType type_;
  QString item_name_;
  Resources item_cost_;

public:
  enum {
    Type = UserType + TypeOffset::kShopItem,
  };
};

class UnitMenu : public QObject {
  Q_OBJECT

 public:
  UnitMenu();
  ~UnitMenu();

  void Draw();
  void Hide();
  void Show();

 private:
  friend class EventHandler::View;

  QPointF position_;
  qreal width_;
  qreal height_;
  QGraphicsRectItem* backgroung_;
  std::vector<ShopItem*> items_;
};
#endif  // MENU_H
