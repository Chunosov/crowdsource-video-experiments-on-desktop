#ifndef APPMODELS_H
#define APPMODELS_H

#include <QString>
#include <QJsonObject>
#include <QList>

class SharedRepoInfo
{
public:
    QString url() const { return _url; }
    QString note() const { return _note; }
    int weight() const { return _weight; }

    bool isEmpty() const { return _url.isEmpty(); }
    QString str() const;

    void parseJson(const QByteArray& text);

    void loadFromConfig();
    void saveToConfig();

private:
    QString _url;
    QString _note;
    int _weight = 0;
};

//-----------------------------------------------------------------------------

class PlatformFeatures
{
public:
    void parseJson(const QByteArray& text);
    const QJsonObject& json() const { return _json; }

    bool isEmpty() const { return _json.isEmpty(); }

    QString str() const;
    QString html() const;

    void loadFromFile(const QString& path);
    void saveToFile(const QString& path) const;

    QString osName() const { return _osName; }

private:
    QJsonObject _json;
    QString _osName;
};

//-----------------------------------------------------------------------------

class RecognitionScenarioFileItem
{
public:
    bool parseJson(const QJsonObject& json);

    QString path() const { return _path; }
    QString name() const { return _name; }
    QString md5() const { return _md5; }
    QString url() const { return _url; }
    bool isLibrary() const { return _isLibrary; }
    bool isExecutable() const { return _isExecutable; }
    bool isDefaultImage() const { return _isDefaultImage; }

    QString fullPath() const;
    QString fullFileName() const;

    bool isExists() const;
    bool checkMD5() const;

private:
    QString _path, _name, _md5, _url;
    bool _isLibrary, _isExecutable, _isDefaultImage;
};

//-----------------------------------------------------------------------------

class RecognitionScenario
{
public:
    bool parseJson(const QJsonObject& json);
    const QJsonObject& json() const { return _json; }

    bool isEmpty() const { return _json.isEmpty(); }

    QString str() const;
    QString html() const;

    long fileSizeBytes() const { return _fileSizeBytes; }
    QString fileSizeMB() const { return _fileSizeMB; }
    QString title() const { return _title; }
    QString cmd() const { return _cmd; }

    const QList<RecognitionScenarioFileItem> files() const { return _files; }
    bool allFilesAreLoaded() const;

private:
    long _fileSizeBytes = 0;
    QString _fileSizeMB, _title, _cmd;
    QList<RecognitionScenarioFileItem> _files;
    QJsonObject _json;
};

//-----------------------------------------------------------------------------

class RecognitionScenarios
{
public:
    const QList<RecognitionScenario>& items() const { return _items; }

    bool isEmpty() const { return _items.isEmpty(); }
    QString str() const;

    void parseJson(const QByteArray& text);

    void loadFromFile(const QString& path);
    void saveToFile(const QString& path) const;

private:
    QList<RecognitionScenario> _items;
};

//-----------------------------------------------------------------------------

class ExperimentProbe
{
public:
    double time;
    double memory;

    void parseJson(const QByteArray& text);
};

//-----------------------------------------------------------------------------

class ExperimentResult
{
public:
    int imagesCount;
    double totalTime;
    double timePerImage;
    double imagesPerSecond;
    double timePerBatch;
    double memoryPerImage;

    void reset();
    void accumulate(const ExperimentProbe& p);
};

#endif // APPMODELS_H
