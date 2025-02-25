#ifndef CONNECTCHECK_H
#define CONNECTCHECK_H

#include <QObject>


class QObject;

namespace oc {
static void disconnect(const QObject *sender, const char *signal,
                const QObject *receiver, const char *member) {
    if (sender != nullptr) {
        QObject::disconnect(sender, signal, receiver, member);
    }
}
}

#endif // CONNECTCHECK_H
