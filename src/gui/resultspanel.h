#ifndef RESULTSPANEL_H
#define RESULTSPANEL_H

#include <QWidget>

class ExperimentContext;
class InfoLabel;

class ResultsPanel : public QWidget
{
    Q_OBJECT
public:
    explicit ResultsPanel(ExperimentContext* context, QWidget *parent = 0);

private slots:
    void experimentStarted();
    void experimentResultReady();

private:
    ExperimentContext* _context;
    InfoLabel *_infoTimePerFrame, *_infoTimePerBatch, *_infoMemoryUsage;
};

#endif // RESULTSPANEL_H
