/****************************************************************************
** Meta object code from reading C++ file 'aav_lineeditsearch.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ProAVV/aav_lineeditsearch.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'aav_lineeditsearch.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_LineEditSearch_t {
    QByteArrayData data[8];
    char stringdata0[96];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LineEditSearch_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LineEditSearch_t qt_meta_stringdata_LineEditSearch = {
    {
QT_MOC_LITERAL(0, 0, 14), // "LineEditSearch"
QT_MOC_LITERAL(1, 15, 20), // "sloHandleTextChanged"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 11), // "QCompleter*"
QT_MOC_LITERAL(4, 49, 9), // "completer"
QT_MOC_LITERAL(5, 59, 17), // "QStringListModel*"
QT_MOC_LITERAL(6, 77, 14), // "str_list_model"
QT_MOC_LITERAL(7, 92, 3) // "str"

    },
    "LineEditSearch\0sloHandleTextChanged\0"
    "\0QCompleter*\0completer\0QStringListModel*\0"
    "str_list_model\0str"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LineEditSearch[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    3,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5, QMetaType::QString,    4,    6,    7,

       0        // eod
};

void LineEditSearch::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LineEditSearch *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sloHandleTextChanged((*reinterpret_cast< QCompleter*(*)>(_a[1])),(*reinterpret_cast< QStringListModel*(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QStringListModel* >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject LineEditSearch::staticMetaObject = { {
    &QLineEdit::staticMetaObject,
    qt_meta_stringdata_LineEditSearch.data,
    qt_meta_data_LineEditSearch,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *LineEditSearch::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LineEditSearch::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LineEditSearch.stringdata0))
        return static_cast<void*>(this);
    return QLineEdit::qt_metacast(_clname);
}

int LineEditSearch::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLineEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
