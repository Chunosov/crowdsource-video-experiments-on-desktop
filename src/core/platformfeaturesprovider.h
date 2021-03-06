#ifndef PLATFORMFEATURESPROVIDER_H
#define PLATFORMFEATURESPROVIDER_H

#include "appmodels.h"

#include <QObject>

class PlatformFeaturesProvider : public QObject
{
    Q_OBJECT

public:
    explicit PlatformFeaturesProvider(QObject *parent = 0) : QObject(parent) {}

    PlatformFeatures loadFromCache();
    void saveToCache(const PlatformFeatures& features) const;

    const PlatformFeatures& current() const { return _current; }
    void setCurrent(const PlatformFeatures& features) { _current = features; }

    void queryPlatformFeatures(const QString &sharedRepoUrl);

signals:
    void platformFeaturesReceived(PlatformFeatures features);

private:
    PlatformFeatures _current;
};

#endif // PLATFORMFEATURESPROVIDER_H
