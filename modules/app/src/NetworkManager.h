/*
 * Copyright (c) 1995-2021, Mathesis Software <mad@mathesis.fun>
 *
 * Licensed under the Apache License, Version 2.0 the "License";
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __KE_QT_NETWORK_MANAGER_H__
#define __KE_QT_NETWORK_MANAGER_H__

#include <functional>

#include <QtCore/QString>
#include <QtNetwork/QNetworkAccessManager>

namespace KE::Qt {

class NetworkManager : public QNetworkAccessManager {

public:
	NetworkManager(QObject *parent) : QNetworkAccessManager(parent) {}

	void searchDiagram(const QString &code, int page, QObject *context, const std::function<void(int errorCode, const QByteArray &response)> callback);
};

}// namespace KE::Qt

#endif /* __KE_QT_NETWORK_MANAGER_H__ */
