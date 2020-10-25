#include "sprite_properties_dock.h"
#include "ui_sprite_properties_dock.h"

SpritePropertiesDock::SpritePropertiesDock(QWidget *parent) : QWidget(parent),
                                                              ui(new Ui::SpritePropertiesDock)
{
  ui->setupUi(this);
  ui->infoName->setVisible(false);
  ui->infoPath->setVisible(false);
  ui->infoSize->setVisible(false);
}

SpritePropertiesDock::~SpritePropertiesDock()
{
  delete ui;
}

void SpritePropertiesDock::SetCurrentProcessor(ImageProcessor *processor)
{
  /* disconnect previous processor */

  disconnect(current_processor, SIGNAL(positionChanged()), this, SLOT(updatePosition()));
  disconnect(current_processor, SIGNAL(frameChanged(int)), this, SLOT(setCurrentFrame(int)));

  current_processor = processor;
  ui->textureLabel->setPixmap(QPixmap::fromImage(current_processor->texture.scaled(ui->textureLabel->size(), Qt::KeepAspectRatio)));
  ui->infoName->setText(tr("Name: ") + processor->get_name());
  ui->infoPath->setText(tr("Path: ") + processor->m_fileName);
  ui->infoSize->setText(tr("Size: ") + QString::number(processor->texture.width()) + "x" + QString::number(processor->texture.height()));

  ui->scaleSpinBox->setValue(processor->get_zoom());
  ui->rotationSpinBox->setValue(processor->get_rotation());

  ui->tileCheckBox->setChecked(current_processor->get_tileable());

  processor->getFrameMode() == "Animation" ? ui->radioButton->setChecked(true) : ui->radioButton_2->setChecked(true);

  ui->hFramesSpinBox->setValue(processor->getHFrames());
  ui->vFramesSpinBox->setValue(processor->getVFrames());

  connect(current_processor, SIGNAL(positionChanged()), this, SLOT(updatePosition()));
  connect(current_processor, SIGNAL(frameChanged(int)), this, SLOT(setCurrentFrame(int)));
  updatePosition();
}

void SpritePropertiesDock::setCurrentFrame(int i)
{
  QImage frame;
  //  current_processor->get_current_frame()->get_image(TextureTypes::Diffuse, &frame);
  //  ui->textureLabel->setPixmap(QPixmap::fromImage(frame.scaled(ui->textureLabel->size(), Qt::KeepAspectRatio)));
}

void SpritePropertiesDock::updatePosition()
{
  ui->xPositionSpinBox->setValue(current_processor->get_position()->x());
  ui->yPositionSpinBox->setValue(current_processor->get_position()->y());
}

void SpritePropertiesDock::on_scaleSpinBox_valueChanged(double arg1)
{
  current_processor->set_zoom(arg1);
}

void SpritePropertiesDock::on_rotationSpinBox_valueChanged(double arg1)
{
  current_processor->set_rotation(arg1);
}

void SpritePropertiesDock::on_xPositionSpinBox_valueChanged(int arg1)
{
  current_processor->get_position()->setX(arg1);
  current_processor->processed();
}

void SpritePropertiesDock::on_yPositionSpinBox_valueChanged(int arg1)
{
  current_processor->get_position()->setY(arg1);
  current_processor->processed();
}

void SpritePropertiesDock::on_tileCheckBox_toggled(bool checked)
{
  current_processor->set_tileable(checked);
}

void SpritePropertiesDock::on_neighboursButton_pressed()
{
  neighboursButtonPressed();
}

void SpritePropertiesDock::on_heightmapButton_pressed()
{
  heightmapButtonPressed();
}

void SpritePropertiesDock::on_specularButton_pressed()
{
  specularButtonPressed();
}

void SpritePropertiesDock::on_hFramesSpinBox_valueChanged(int arg1)
{
  framesChanged(arg1, ui->vFramesSpinBox->value(), current_processor);
}

void SpritePropertiesDock::on_vFramesSpinBox_valueChanged(int arg1)
{
  framesChanged(ui->hFramesSpinBox->value(), arg1, current_processor);
}

void SpritePropertiesDock::on_radioButton_2_toggled(bool checked)
{
  if (!current_processor)
    return;
  current_processor->setFrameMode(checked ? "Sheet" : "Animation");
}
