/****************************************************************************
** Meta object code from reading C++ file 'aav_networkmanager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ProAVV/aav_networkmanager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'aav_networkmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_NetWorkManager_t {
    QByteArrayData data[12];
    char stringdata0[147];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NetWorkManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NetWorkManager_t qt_meta_stringdata_NetWorkManager = {
    {
QT_MOC_LITERAL(0, 0, 14), // "NetWorkManager"
QT_MOC_LITERAL(1, 15, 21), // "sloHandleDownloadData"
QT_MOC_LITERAL(2, 37, 8), // "QBuffer*"
QT_MOC_LITERAL(3, 46, 0), // ""
QT_MOC_LITERAL(4, 47, 14), // "QNetworkReply*"
QT_MOC_LITERAL(5, 62, 5), // "reply"
QT_MOC_LITERAL(6, 68, 19), // "sloHandleVideosInfo"
QT_MOC_LITERAL(7, 88, 13), // "onGetUserInfo"
QT_MOC_LITERAL(8, 102, 7), // "QLabel*"
QT_MOC_LITERAL(9, 110, 4), // "lab1"
QT_MOC_LITERAL(10, 115, 4), // "lab2"
QT_MOC_LITERAL(11, 120, 26) // "sloHandleRecommendFeatures"

    },
    "NetWorkManager\0sloHandleDownloadData\0"
    "QBuffer*\0\0QNetworkReply*\0reply\0"
    "sloHandleVideosInfo\0onGetUserInfo\0"
    "QLabel*\0lab1\0lab2\0sloHandleRecommendFeatures"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NetWorkManager[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   34,    3, 0x0a /* Public */,
       6,    1,   37,    3, 0x0a /* Public */,
       7,    3,   40,    3, 0x0a /* Public */,
      11,    1,   47,    3, 0x0a /* Public */,

 // slots: parameters
    0x80000000 | 2, 0x80000000 | 4,    5,
    0x80000000 | 2, 0x80000000 | 4,    5,
    QMetaType::Void, 0x80000000 | 8, 0x80000000 | 8, 0x80000000 | 4,    9,   10,    5,
    QMetaType::Void, 0x80000000 | 4,    5,

       0        // eod
};

void NetWorkManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<NetWorkManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { QBuffer* _r = _t->sloHandleDownloadData((*reinterpret_cast< QNetworkReply*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QBuffer**>(_a[0]) = std::move(_r); }  break;
        case 1: { QBuffer* _r = _t->sloHandleVideosInfo((*reinterpret_cast< QNetworkReply*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QBuffer**>(_a[0]) = std::move(_r); }  break;
        case 2: _t->onGetUserInfo((*reinterpret_cast< QLabel*(*)>(_a[1])),(*reinterpret_cast< QLabel*(*)>(_a[2])),(*reinterpret_cast< QNetworkReply*(*)>(_a[3]))); break;
        case 3: _t->sloHandleRecommendFeatures((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply* >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply* >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QLabel* >(); break;
            case 2:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply* >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply* >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject NetWorkManager::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_NetWorkManager.data,
    qt_meta_data_NetWorkManager,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *NetWorkManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NetWorkManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_NetWorkManager.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int NetWorkManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
