#include "RfFrontEndGuiWidget.h"
#include "graphics/qt/common/Spectrogramplot.h"
#include "graphics/qt/common/Lineplot.h"
#include "graphics/qt/common/Events.h"
#include "KnobSpin.h"

#include <QLayout>
#include <QDoubleSpinBox>
#include <QShortcut>

RfFrontEndGuiWidget::RfFrontEndGuiWidget(FEGuiParams params,
                                         FEGuiCallback* callback,
                                         QWidget *parent)
  :QWidget(parent)
{
  params_ = params;
  callback_ = callback;

  QHBoxLayout* hLayout1 = new QHBoxLayout(this);

  if(params_.knobFreq == true) {
      knob1_ = new KnobSpin("Frequency (MHz)", params_.minFreq, params_.maxFreq,
                            params_.stepFreq, this);
      QShortcut* s11_ = new QShortcut(QKeySequence("Q"), this);
      connect(s11_, SIGNAL(activated()), knob1_->spin(), SLOT(stepUp()));
      QShortcut* s12_ = new QShortcut(QKeySequence("A"), this);
      connect(s12_, SIGNAL(activated()), knob1_->spin(), SLOT(stepDown()));

      connect(knob1_, SIGNAL(valueChanged(double)),
              this, SLOT(setFrequency(double)));

      hLayout1->addWidget(knob1_);
  }

  if(params_.knobBW == true) {
      knob2_ = new KnobSpin("Bandwidth (MHz)", params_.minBW, params_.maxBW,
                            params_.stepBW, this);
      QShortcut* s21_ = new QShortcut(QKeySequence("W"), this);
      connect(s21_, SIGNAL(activated()), knob2_->spin(), SLOT(stepUp()));
      QShortcut* s22_ = new QShortcut(QKeySequence("S"), this);
      connect(s22_, SIGNAL(activated()), knob2_->spin(), SLOT(stepDown()));

      connect(knob2_, SIGNAL(valueChanged(double)),
              this, SLOT(setBandwidth(double)));

      hLayout1->addWidget(knob2_);
  }

  if(params_.knobGain == true) {
      knob3_ = new KnobSpin("Gain", params_.minGain, params_.maxGain,
                            params_.stepGain, this);
      QShortcut* s31_ = new QShortcut(QKeySequence("E"), this);
      connect(s31_, SIGNAL(activated()), knob3_->spin(), SLOT(stepUp()));
      QShortcut* s32_ = new QShortcut(QKeySequence("D"), this);
      connect(s32_, SIGNAL(activated()), knob3_->spin(), SLOT(stepDown()));

      connect(knob3_, SIGNAL(valueChanged(double)),
              this, SLOT(setGain(double)));

      hLayout1->addWidget(knob3_);
  }

  if(params_.knobMargin == true) {
      knob4_ = new KnobSpin("Sync Tx Margin", params_.min_sync_margin_tx, params_.max_sync_margin_tx,
                            params_.step_sync_margin_tx, this);
      QShortcut* s41_ = new QShortcut(QKeySequence("R"), this);
      connect(s41_, SIGNAL(activated()), knob4_->spin(), SLOT(stepUp()));
      QShortcut* s42_ = new QShortcut(QKeySequence("F"), this);
      connect(s42_, SIGNAL(activated()), knob4_->spin(), SLOT(stepDown()));

      connect(knob4_, SIGNAL(valueChanged(double)),
              this, SLOT(setSyncTxMargin(double)));

      hLayout1->addWidget(knob4_);
  }


  setPalette( QPalette( QColor( 192, 192, 192 ) ) );
  updateGradient();
}

RfFrontEndGuiWidget::~RfFrontEndGuiWidget()
{
}

void RfFrontEndGuiWidget::setFrequency(double frequency)
{
  double delta = 0.05;
  if(frequency > frequency_+delta || frequency < frequency_-delta)
  {
    frequency_ = frequency;
    callback_->setFrequency(frequency_);
  }
}

void RfFrontEndGuiWidget::setBandwidth(double bandwidth)
{
  double delta = 0.05;
  if(bandwidth > bandwidth_+delta || bandwidth < bandwidth_-delta)
  {
    bandwidth_ = bandwidth;
    callback_->setBandwidth(bandwidth_);
  }
}

void RfFrontEndGuiWidget::setGain(double gain)
{
  double delta = 0.05;
  if(gain > gain_+delta || gain < gain_-delta)
  {
    gain_ = gain;
    callback_->setGain(gain_);
  }
}

void RfFrontEndGuiWidget::setSyncTxMargin(double margin)
{
  double delta = 0.02;
  if(margin > margin_+delta || margin < margin_-delta)
  {
    margin_ = margin;
    callback_->setSyncTxMargin(margin_);
  }
}

void RfFrontEndGuiWidget::updateGradient()
{
    QPalette pal = palette();

    const QColor buttonColor = pal.color( QPalette::Button );
    const QColor lightColor = pal.color( QPalette::Light );
    const QColor midLightColor = pal.color( QPalette::Midlight );

#ifdef Q_WS_X11
    // Qt 4.7.1: QGradient::StretchToDeviceMode is buggy on X11

    QLinearGradient gradient( rect().topLeft(), rect().topRight() );
#else
    QLinearGradient gradient( 0, 0, 1, 0 );
    gradient.setCoordinateMode( QGradient::StretchToDeviceMode );
#endif

    gradient.setColorAt( 0.0, midLightColor );
    gradient.setColorAt( 0.7, buttonColor );
    gradient.setColorAt( 1.0, buttonColor );

    pal.setBrush( QPalette::Window, gradient );
    setPalette( pal );
}
