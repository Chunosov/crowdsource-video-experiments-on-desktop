#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <QString>

QT_BEGIN_NAMESPACE
class QSettings;
QT_END_NAMESPACE

class AppConfig
{
public:
    static QString sharedResourcesUrl();

    static QString sharedRepoUrl();
    static void setSharedRepoUrl(const QString& url);

    static QString email();

    static QString platformFeaturesCacheFile();
    static QString scenariosCacheFile();
    static QString scenariosDataDir();
    static QString imagesDir();
    static QStringList imagesFilter();

    static bool checkScenarioFilesMd5();

    static int selectedScenarioIndex(int experimentIndex);
    static void setSelectedScenarioIndex(int experimentIndex, int scenarioIndex);
    static int batchSize(int experimentIndex);
    static void setBatchSize(int experimentIndex, int batchSize);

private:
    static QString configFileName();
    static QSettings& config();

    static int configValueInt(const QString& key, int defaultValue);
    static QString experimentKey(const QString& baseKey, int experimentIndex);
};

#endif // APPCONFIG_H
