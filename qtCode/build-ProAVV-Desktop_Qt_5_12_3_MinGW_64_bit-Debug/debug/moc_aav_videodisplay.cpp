/****************************************************************************
** Meta object code from reading C++ file 'aav_videodisplay.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ProAVV/aav_videodisplay.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'aav_videodisplay.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_VideoDisplay_t {
    QByteArrayData data[16];
    char stringdata0[183];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VideoDisplay_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VideoDisplay_t qt_meta_stringdata_VideoDisplay = {
    {
QT_MOC_LITERAL(0, 0, 12), // "VideoDisplay"
QT_MOC_LITERAL(1, 13, 16), // "sloSetSliderDura"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 3), // "dur"
QT_MOC_LITERAL(4, 35, 15), // "sloSetSliderPos"
QT_MOC_LITERAL(5, 51, 13), // "sloPlayerMove"
QT_MOC_LITERAL(6, 65, 3), // "pos"
QT_MOC_LITERAL(7, 69, 14), // "sloPlayerPause"
QT_MOC_LITERAL(8, 84, 13), // "sloMenuUnfold"
QT_MOC_LITERAL(9, 98, 10), // "sloAdjRate"
QT_MOC_LITERAL(10, 109, 3), // "num"
QT_MOC_LITERAL(11, 113, 10), // "sloPreload"
QT_MOC_LITERAL(12, 124, 25), // "QMediaPlayer::MediaStatus"
QT_MOC_LITERAL(13, 150, 6), // "status"
QT_MOC_LITERAL(14, 157, 16), // "sloVolumeChanged"
QT_MOC_LITERAL(15, 174, 8) // "position"

    },
    "VideoDisplay\0sloSetSliderDura\0\0dur\0"
    "sloSetSliderPos\0sloPlayerMove\0pos\0"
    "sloPlayerPause\0sloMenuUnfold\0sloAdjRate\0"
    "num\0sloPreload\0QMediaPlayer::MediaStatus\0"
    "status\0sloVolumeChanged\0position"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VideoDisplay[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x0a /* Public */,
       4,    1,   57,    2, 0x0a /* Public */,
       5,    1,   60,    2, 0x0a /* Public */,
       7,    0,   63,    2, 0x0a /* Public */,
       8,    0,   64,    2, 0x0a /* Public */,
       9,    1,   65,    2, 0x0a /* Public */,
      11,    1,   68,    2, 0x0a /* Public */,
      14,    1,   71,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::LongLong,    3,
    QMetaType::Void, QMetaType::LongLong,    3,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void, QMetaType::Int,   15,

       0        // eod
};

void VideoDisplay::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VideoDisplay *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sloSetSliderDura((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 1: _t->sloSetSliderPos((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 2: _t->sloPlayerMove((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->sloPlayerPause(); break;
        case 4: _t->sloMenuUnfold(); break;
        case 5: _t->sloAdjRate((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->sloPreload((*reinterpret_cast< QMediaPlayer::MediaStatus(*)>(_a[1]))); break;
        case 7: _t->sloVolumeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QMediaPlayer::MediaStatus >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject VideoDisplay::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_VideoDisplay.data,
    qt_meta_data_VideoDisplay,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *VideoDisplay::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VideoDisplay::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VideoDisplay.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int VideoDisplay::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
