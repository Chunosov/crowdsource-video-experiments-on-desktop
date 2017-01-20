#include "experimentcontext.h"
#include "experimentpanel.h"
#include "framespanel.h"
#include "featurespanel.h"
#include "resultspanel.h"

#include <QBoxLayout>

ExperimentPanel::ExperimentPanel(ExperimentContext *context, QWidget *parent) : QFrame(parent)
{
    auto layout = new QHBoxLayout;
    layout->setSpacing(12);
    layout->addWidget(new FramesPanel);
    layout->addWidget(_featuresPanel = new FeaturesPanel(context));
    layout->addWidget(new ResultsPanel);

    setFrameShape(QFrame::StyledPanel);
    setLayout(layout);
}

void ExperimentPanel::updateExperimentConditions()
{
    _featuresPanel->updateExperimentConditions();
}
