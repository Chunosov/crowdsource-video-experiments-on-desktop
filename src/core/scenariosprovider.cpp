#include "appevents.h"
#include "remotedataaccess.h"
#include "scenariosprovider.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrl>

ScenariosProvider::ScenariosProvider(RemoteDataAccess *network, QObject *parent) : QObject(parent), _network(network)
{
}

void ScenariosProvider::queryScenarios(const QString& url, const PlatformFeatures& features)
{
    AppEvents::info(tr("Query recognition scenarios from %1").arg(url));

    QJsonObject json;
    json["action"] = "get";
    json["module_uoa"] = "experiment.scenario.mobile"; // TODO: which module?
    json["platform_features"] = features.json();
    auto postData = "ck_json=" + QUrl::toPercentEncoding(QJsonDocument(json).toJson());

    auto reply = _network->post(url, postData);
    connect(reply, SIGNAL(finished()), this, SLOT(queryRecognitionScenarios_finished()));
}

void ScenariosProvider::queryRecognitionScenarios_finished()
{
    auto reply = qobject_cast<QNetworkReply*>(sender());
    if (reply->error())
        return AppEvents::error(tr("Unable to load recognition scenarios list: %1").arg(reply->errorString()));

    RecognitionScenarios scenarios;
    scenarios.parseJson(reply->readAll());
    if (!scenarios.isEmpty())
        emit scenariosReceived(scenarios);
}
