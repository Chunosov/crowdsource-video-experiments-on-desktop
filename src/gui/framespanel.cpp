#include "appconfig.h"
#include "appevents.h"
#include "experimentcontext.h"
#include "framespanel.h"
#include "framewidget.h"
#include "scenariorunner.h"

#include <QApplication>
#include <QBoxLayout>
#include <QDebug>
#include <QDir>
#include <QTimer>

ImagesBank::ImagesBank()
{
    auto imagesDir = AppConfig::imagesDir();
    qDebug() << "Images directory:" << imagesDir;

    auto imagesFilter = AppConfig::imagesFilter();
    qDebug() << "Images filter:" << imagesFilter.join(";");

    QDir dir(imagesDir);
    for (const QString& entry: dir.entryList(imagesFilter, QDir::Files))
        _images << imagesDir + QDir::separator() + entry;

    qDebug() << "Images found:" << _images.size();
}

//-----------------------------------------------------------------------------

BatchItem::BatchItem(int index, int imageOffset, const ScenarioRunParams& params, ImagesBank *images) : QObject(0)
{
    _index = index;
    _images = images;
    _imageIndex = imageOffset;
    _frame = new FrameWidget;
    _runner = new ScenarioRunner(params);
    connect(_runner, &ScenarioRunner::scenarioFinished, this, &BatchItem::scenarioFinished);
}

BatchItem::~BatchItem()
{
    delete _frame;
    delete _runner;
}

void BatchItem::run()
{
    _isStopped = false;
    _stopFlag = false;
    runInternal();
}

void BatchItem::runInternal()
{
    _runner->run(_images->imageFile(_imageIndex));
}

void BatchItem::scenarioFinished(const QString &error)
{
    if (error.isEmpty())
    {
        _frame->loadImage(_images->imageFile(_imageIndex));
        _frame->showInfo(parseOutput(_runner->getStdout()));
        emit finished(_runner->readProbe());
    }
    else
    {
        _stopFlag = true;
        AppEvents::error(error);
    }

    if (_stopFlag)
    {
        qDebug() << "Batch item stopped" << _index;
        _isStopped = true;
        emit stopped();
        return;
    }

    _imageIndex++;
    if (_imageIndex == _images->size())
        _imageIndex = 0;
    run();
}

QString BatchItem::parseOutput(const QString& text) const
{
    // 0.9835 - "n03793489 mouse, computer mouse"
    static QLatin1String predictionMarker("0.");

    QStringList results;
    for (const QStringRef& line: text.splitRef('\n', QString::SkipEmptyParts))
        if (line.startsWith(predictionMarker))
            results << line.toString().remove('"');
    return results.join("\n");
}

//-----------------------------------------------------------------------------

FramesPanel::FramesPanel(ExperimentContext *context, QWidget *parent) : QWidget(parent)
{
    _context = context;
    connect(_context, SIGNAL(experimentStarted()), this, SLOT(experimentStarted()));
    connect(_context, SIGNAL(experimentStopping()), this, SLOT(experimentStopping()));

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    auto layout = new QHBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    setLayout(layout);
}

FramesPanel::~FramesPanel()
{
    clearBatch();
    if (_images) delete _images;
}

void FramesPanel::experimentStarted()
{
    if (!_context->currentScenarioExists())
    {
        AppEvents::error(tr("No scenario selected"));
        QTimer::singleShot(200, _context, SIGNAL(experimentFinished()));
        return;
    }

    auto scenario = _context->currentScenario();
    qDebug() << "Start experiment for scenario" << scenario.title();

    if (!scenario.allFilesAreLoaded())
    {
        AppEvents::error(tr("Not all files of scenario were loaded"));
        QTimer::singleShot(200, _context, SIGNAL(experimentFinished()));
        return;
    }

    if (!prepareImages()) return;

    ScenarioRunParams params(scenario);

    clearBatch();
    prepareBatch(params);

    qDebug() << "Start batch processing";
    for (auto item: _batchItems) item->run();
}

void FramesPanel::experimentStopping()
{
    qDebug() << "Stopping batch processing";
    for (auto item: _batchItems) item->stop();
}

void FramesPanel::clearBatch()
{
    for (auto item: _batchItems)
        delete item;
    _batchItems.clear();
}

void FramesPanel::prepareBatch(const ScenarioRunParams& params)
{
    qDebug() << "Prepare batch items";
    for (int i = 0; i < _context->batchSize(); i++)
    {
        int offset = qRound(_images->size()/double(_context->batchSize())*i);
        auto item = new BatchItem(i, offset, params, _images);
        connect(item, &BatchItem::stopped, this, &FramesPanel::batchStopped);
        connect(item, &BatchItem::finished, _context, &ExperimentContext::recognitionFinished);
        _batchItems.append(item);
        layout()->addWidget(item->frame());
    }
}

bool FramesPanel::prepareImages()
{
    qDebug() << "Prepare images";
    if (!_images)
        _images = new ImagesBank();
    if (_images->size() < 1)
    {
        AppEvents::error(tr("No images for processing"));
        return false;
    }
    return true;
}

bool FramesPanel::allItemsStopped()
{
    for (auto item: _batchItems)
        if (!item->isStopped())
            return false;
    return true;
}

void FramesPanel::batchStopped()
{
    if (allItemsStopped())
    {
        qDebug() << "Batch processing finished";
        emit _context->experimentFinished();
    }
}
