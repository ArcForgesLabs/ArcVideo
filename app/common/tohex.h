#ifndef TOHEX_H
#define TOHEX_H

#include <QString>
#include <QtGlobal>

#include "common/define.h"

namespace arcvideo {

inline QString ToHex(quint64 t) {
    return QStringLiteral("%1").arg(t, 0, 16);
}

}  // namespace arcvideo

#endif  // TOHEX_H
